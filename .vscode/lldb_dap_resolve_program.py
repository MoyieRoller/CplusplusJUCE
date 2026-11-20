#!/usr/bin/env python3
"""Resolve the selected executable before passing a launch request to LLDB DAP.

macOS derives NSBundle.mainBundle from the launch path. Launching our selection
symlink directly loses the .app bundle, even though it points to its executable.
Keep the real path in the DAP request, including LLDB's runInTerminal request.
"""
from __future__ import annotations

import json
from pathlib import Path
import subprocess
import sys
import threading
from typing import BinaryIO


def forward_requests(source: BinaryIO, destination: BinaryIO) -> None:
    while True:
        headers = {}
        while line := source.readline():
            if line in (b'\r\n', b'\n'):
                break
            name, value = line.decode('ascii').split(':', 1)
            headers[name.lower()] = value.strip()
        if not headers:
            return
        length = int(headers['content-length'])
        payload = source.read(length)
        if len(payload) != length:
            raise EOFError('Incomplete DAP request')
        message = json.loads(payload)
        if message.get('type') == 'request' and message.get('command') == 'launch':
            arguments = message.get('arguments', {})
            if program := arguments.get('program'):
                arguments['program'] = str(Path(program).resolve())
            payload = json.dumps(message).encode('utf-8')
        destination.write(f'Content-Length: {len(payload)}\r\n\r\n'.encode('ascii'))
        destination.write(payload)
        destination.flush()


def main() -> int:
    # Adapter output (including reverse requests) goes directly to VS Code.
    process = subprocess.Popen(sys.argv[1:], stdin=subprocess.PIPE)

    def pump() -> None:
        try:
            forward_requests(sys.stdin.buffer, process.stdin)
        except BrokenPipeError:
            pass
        except Exception as error:
            print(f'LLDB launch-path adapter: {error}', file=sys.stderr)
            process.terminate()
        finally:
            try:
                process.stdin.close()
            except BrokenPipeError:
                pass

    threading.Thread(target=pump, daemon=True).start()
    try:
        return process.wait()
    except KeyboardInterrupt:
        process.terminate()
        return process.wait()


if __name__ == '__main__':
    raise SystemExit(main())
