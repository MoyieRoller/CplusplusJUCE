import io
import json
from pathlib import Path
import sys
import tempfile
import unittest

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import lldb_dap_resolve_program as adapter


def frame(payload):
    return f'Content-Length: {len(payload)}\r\n\r\n'.encode('ascii') + payload


def encode(message):
    return json.dumps(message, ensure_ascii=False).encode('utf-8')


def decode_frames(data):
    stream = io.BytesIO(data)
    results = []
    while header := stream.readline():
        name, length = header.decode('ascii').strip().split(':', 1)
        if name != 'Content-Length' or stream.readline() != b'\r\n':
            raise AssertionError('Invalid DAP frame header')
        payload = stream.read(int(length))
        if len(payload) != int(length):
            raise AssertionError('Content-Length does not match payload bytes')
        results.append((payload, json.loads(payload)))
    return results


class ResolveProgramTests(unittest.TestCase):
    def test_launch_resolves_bundle_executable_with_spaces_and_unicode(self):
        with tempfile.TemporaryDirectory(prefix='hfm-dap-') as tmp:
            executable = Path(tmp) / 'Hörbeispiel JUCE.app/Contents/MacOS/Hello JUCE'
            executable.parent.mkdir(parents=True)
            executable.write_bytes(b'placeholder executable')
            selected = Path(tmp) / 'launch bin/main'
            selected.parent.mkdir()
            selected.symlink_to(executable)
            message = {
                'seq': 42,
                'type': 'request',
                'command': 'launch',
                'arguments': {
                    'program': str(selected),
                    'args': ['Grüße', '--label', 'Hello JUCE'],
                    'cwd': str(Path(tmp)),
                    'console': 'integratedTerminal',
                },
            }
            output = io.BytesIO()
            adapter.forward_requests(io.BytesIO(frame(encode(message))), output)
            frames = decode_frames(output.getvalue())
            self.assertEqual(len(frames), 1)
            actual = frames[0][1]
            expected = json.loads(encode(message))
            expected['arguments']['program'] = str(executable.resolve())
            self.assertEqual(actual, expected)
            self.assertNotIn(str(selected), actual['arguments']['program'])

    def test_non_launch_request_and_reverse_response_are_byte_preserved(self):
        payloads = [
            b'{ "type": "request", "seq": 9, "command": "evaluate", "arguments": {"expression": "program"} }',
            encode({'type': 'response', 'seq': 10, 'request_seq': 7,
                    'command': 'runInTerminal', 'success': True,
                    'body': {'processId': 123, 'label': 'Grüße'}}),
        ]
        for payload in payloads:
            with self.subTest(payload=payload):
                output = io.BytesIO()
                adapter.forward_requests(io.BytesIO(frame(payload)), output)
                self.assertEqual(output.getvalue(), frame(payload))

    def test_multiple_frames_and_eof_after_last_frame(self):
        messages = [
            {'seq': 1, 'type': 'request', 'command': 'initialize', 'arguments': {}},
            {'seq': 2, 'type': 'request', 'command': 'launch',
             'arguments': {'program': '/tmp/project/../example'}},
            {'seq': 3, 'type': 'response', 'request_seq': 11,
             'command': 'runInTerminal', 'success': True, 'body': {'processId': 456}},
            {'seq': 4, 'type': 'request', 'command': 'configurationDone'},
        ]
        payloads = [encode(message) for message in messages]
        output = io.BytesIO()
        adapter.forward_requests(io.BytesIO(b''.join(map(frame, payloads))), output)
        results = decode_frames(output.getvalue())
        self.assertEqual(len(results), 4)
        self.assertEqual([result[1]['seq'] for result in results], [1, 2, 3, 4])
        self.assertEqual(results[1][1]['arguments']['program'], str(Path('/tmp/example').resolve()))
        for index in (0, 2, 3):
            self.assertEqual(results[index][0], payloads[index])

    def test_empty_input_is_clean_eof(self):
        output = io.BytesIO()
        adapter.forward_requests(io.BytesIO(), output)
        self.assertEqual(output.getvalue(), b'')

    def test_truncated_payload_raises_without_forwarding_partial_frame(self):
        payload = encode({'seq': 1, 'type': 'request', 'command': 'launch'})
        output = io.BytesIO()
        with self.assertRaisesRegex(EOFError, 'Incomplete DAP request'):
            adapter.forward_requests(io.BytesIO(frame(payload)[:-1]), output)
        self.assertEqual(output.getvalue(), b'')


if __name__ == '__main__':
    unittest.main()
