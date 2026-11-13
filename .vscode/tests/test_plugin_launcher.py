import json
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import plugin_launcher as launcher
import build_session


class PluginLauncherTests(unittest.TestCase):
    def test_native_dropdown_values(self):
        self.assertEqual(launcher.select('App', 'Standalone'), ('Standalone', None))
        with patch.object(launcher.sys, 'platform', 'darwin'):
            for fmt, hosts in launcher.HOSTS.items():
                for host in hosts:
                    self.assertEqual(launcher.select(fmt, host), (fmt, host))
        for fmt, host in [('VST3', 'Standalone'), ('App', 'REAPER')]:
            with self.assertRaises(RuntimeError):
                launcher.select(fmt, host)

    def test_vscode_inputs_are_host_then_format(self):
        root = Path(__file__).resolve().parents[2]
        for base in (root, root / 'session-10'):
            tasks = json.loads((base / '.vscode/tasks.json').read_text())
            label = 'Build selected JUCE plugin' if base == root else 'Prepare plugin launch'
            task = next(t for t in tasks['tasks'] if t['label'] == label)
            self.assertLess(task['args'].index('${input:pluginHost}'), task['args'].index('${input:pluginFormat}'))
            inputs = {i['id']: i for i in tasks['inputs']}
            self.assertEqual(inputs['pluginHost']['type'], 'pickString')
            self.assertEqual(inputs['pluginFormat']['options'], ['App', 'VST3', 'AU', 'AAX'])

    def test_console_and_plugin_launchers_have_separate_inputs_and_destinations(self):
        root = Path(__file__).resolve().parents[2]
        data = json.loads((root / '.vscode/tasks.json').read_text())
        tasks = {t['label']: t for t in data['tasks']}
        expected = {
            'Build selected session': ['${input:session}'],
            'Build selected JUCE plugin': ['${input:pluginSession}', '${input:pluginHost}', '${input:pluginFormat}'],
        }
        for label, selections in expected.items():
            task = tasks[label]
            for args in (task['args'], task['windows']['args']):
                self.assertEqual([arg for arg in args if arg.startswith('${input:')], selections)
        inputs = {i['id']: i for i in data['inputs']}
        self.assertEqual(inputs['session']['options'], ['setup', *[f'session-{i:02}' for i in range(1, 10)]])
        self.assertEqual(inputs['pluginSession']['options'], ['session-10'])
        configs = json.loads((root / '.vscode/launch.json').read_text())['configurations']
        # Additional user entries (for example a visual separator) are allowed.
        for platform in ('macOS', 'Linux', 'Windows'):
            console = next(c for c in configs if c['name'] == f'Build and launch a course session — {platform}')
            plugin = next(c for c in configs if c['name'] == f'Build and Launch a JUCE Plugin — {platform}')
            self.assertEqual(console['preLaunchTask'], 'Build selected session')
            self.assertEqual(plugin['preLaunchTask'], 'Build selected JUCE plugin')
            self.assertIn('/launch-bin/console/', console['program'])
            self.assertIn('/launch-bin/plugin/', plugin['program'])

    def test_console_sessions_build_without_plugin_options(self):
        for session in ('setup', 'session-09'):
            with patch.object(sys, 'argv', ['build_session.py', session, '--dry-run']), patch.object(build_session, 'run_task') as run_task, patch.object(launcher, 'prepare') as prepare:
                self.assertEqual(build_session.main(), 0)
                run_task.assert_called_once()
                self.assertEqual(run_task.call_args.args[0], 'Build setup' if session == 'setup' else 'Build lesson')
                prepare.assert_not_called()

    def test_console_sessions_reject_plugin_options(self):
        for session in ('setup', 'session-09'):
            with patch.object(sys, 'argv', ['build_session.py', session, '--host', 'Logic', '--format', 'AU', '--dry-run']), patch.object(build_session, 'run_task') as run_task, patch.object(launcher, 'prepare') as prepare:
                with self.assertRaises(RuntimeError):
                    build_session.main()
                run_task.assert_not_called()
                prepare.assert_not_called()

    def test_later_existing_session_uses_plugin_launcher(self):
        with tempfile.TemporaryDirectory() as tmp:
            root = Path(tmp)
            session = root / 'session-11'
            session.mkdir()
            with patch.object(build_session, 'ROOT', root), patch.object(sys, 'argv', ['build_session.py', 'session-11', '--host', 'Standalone', '--format', 'App', '--dry-run']), patch.object(launcher, 'prepare') as prepare, patch.object(build_session, 'run_task') as run_task:
                self.assertEqual(build_session.main(), 0)
                prepare.assert_called_once_with(session, 'Standalone', None, None, True)
                run_task.assert_not_called()

    def test_incompatible_hosts_are_rejected(self):
        for fmt, host in [('VST3', 'Logic'), ('AU', 'Cubase 15'), ('AAX', 'REAPER'), ('Standalone', 'Logic')]:
            with self.subTest(fmt=fmt, host=host), self.assertRaises(RuntimeError):
                launcher.select(fmt, host)

    def test_missing_selections_fail_without_opening_a_dialog(self):
        with self.assertRaisesRegex(RuntimeError, 'Select a format'):
            launcher.select(None, None)
        with self.assertRaisesRegex(RuntimeError, 'No host'):
            launcher.select('VST3', None)

    def test_install_links_only_selected_plugin_and_preserves_existing_bundle(self):
        with tempfile.TemporaryDirectory() as tmp, patch.object(launcher.Path, 'home', return_value=Path(tmp)):
            source = Path(tmp) / 'build/Hello JUCE.vst3'
            source.mkdir(parents=True)
            launcher.install_plugin(source, 'VST3')
            destination = Path(tmp) / 'Library/Audio/Plug-Ins/VST3/Hello JUCE.vst3'
            self.assertEqual(destination.resolve(), source.resolve())
            launcher.install_plugin(source, 'VST3')
            destination.unlink()
            destination.mkdir()
            with self.assertRaisesRegex(RuntimeError, 'already exists'):
                launcher.install_plugin(source, 'VST3')
            self.assertTrue(destination.is_dir())

    def test_builds_only_selected_target_and_dry_run_does_not_install(self):
        with patch.object(launcher, 'host_executable', return_value=Path('/Applications/Host.app/Contents/MacOS/Host')), patch.object(launcher.subprocess, 'run') as run, patch.object(launcher, 'install_plugin') as install:
            launcher.prepare(Path('/course/session-10'), 'AU', 'Logic')
            self.assertEqual(run.call_args_list[1].args[0][-2:], ['--target', 'HelloJUCE_AU'])
            install.assert_called_once_with(Path('/course/session-10/build/HelloJUCE_artefacts/Debug/AU/Hello JUCE.component'), 'AU')
            run.reset_mock(); install.reset_mock()
            launcher.prepare(Path('/course/session-10'), 'AU', 'Logic', dry_run=True)
            run.assert_not_called(); install.assert_not_called()

    def test_switching_back_to_console_does_not_overwrite_host(self):
        with tempfile.TemporaryDirectory() as tmp, patch.object(build_session.sys, 'platform', 'darwin'):
            base = Path(tmp)
            host = base / 'Host.app/Contents/MacOS/Host'
            host.parent.mkdir(parents=True); host.write_text('host')
            console = base / 'console'; console.write_text('console')
            destination = base / 'launch/main'
            build_session.prepare_debug_target(host, destination)
            self.assertTrue(destination.is_symlink())
            build_session.prepare_debug_target(console, destination)
            self.assertEqual(host.read_text(), 'host')
            self.assertTrue(destination.is_symlink())
            self.assertEqual(destination.resolve(), console.resolve())
            self.assertEqual(destination.read_text(), 'console')

    def test_console_launch_link_tracks_original_executable(self):
        with tempfile.TemporaryDirectory() as tmp:
            base = Path(tmp)
            source = base / 'session-09/build/bin/main'
            source.parent.mkdir(parents=True)
            source.write_text('first build')
            destination = base / 'launch-bin/console/main'
            build_session.prepare_debug_target(source, destination)
            self.assertTrue(destination.is_symlink())
            self.assertEqual(destination.resolve(), source.resolve())
            source.write_text('rebuilt')
            self.assertEqual(destination.read_text(), 'rebuilt')


if __name__ == '__main__':
    unittest.main()
