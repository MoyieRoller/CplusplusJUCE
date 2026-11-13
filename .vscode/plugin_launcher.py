"""Build Session 10's selected format and prepare the executable for VS Code."""
from __future__ import annotations

import os
import plistlib
import subprocess
import sys
from pathlib import Path

FORMATS = ('Standalone', 'VST3', 'AU', 'AAX')
HOSTS = {
    'VST3': ('REAPER', 'Cubase 15'),
    'AU': ('REAPER', 'Logic'),
    'AAX': ('Pro Tools Developer',),
}
APPS = {
    'REAPER': 'REAPER.app',
    'Cubase 15': 'Cubase 15.app',
    'Logic': 'Logic Pro.app',
    'Pro Tools Developer': 'Pro Tools Developer.app',
}


def select(format_name: str | None, host: str | None) -> tuple[str, str | None]:
    # Selection happens in VS Code's native inputs, never in external dialogs.
    if not format_name:
        raise RuntimeError('Select a format in the Session 10 launch configuration, or pass --format.')
    if format_name == 'App':
        format_name = 'Standalone'
    if host == 'Standalone':
        host = None
        if format_name != 'Standalone':
            raise RuntimeError('Standalone requires App. Select a compatible host for a plugin format.')
    if format_name not in FORMATS:
        raise RuntimeError(f'Unknown format: {format_name}')
    if format_name == 'Standalone':
        if host:
            raise RuntimeError('App requires Standalone as the selected app/host.')
        return format_name, None
    if host not in HOSTS[format_name]:
        raise RuntimeError(f'{host or "No host"} cannot load {format_name}. Choose {", ".join(HOSTS[format_name])}.')
    if sys.platform != 'darwin':
        raise RuntimeError('Hosted launch is currently configured for macOS. Use Standalone / App on this platform.')
    return format_name, host


def host_executable(host: str) -> Path:
    for folder in (Path('/Applications'), Path.home() / 'Applications'):
        app = folder / APPS[host]
        plist = app / 'Contents/Info.plist'
        if plist.is_file():
            with plist.open('rb') as stream:
                executable = app / 'Contents/MacOS' / plistlib.load(stream)['CFBundleExecutable']
            if executable.is_file():
                return executable
    raise RuntimeError(f'{host} is not installed in /Applications or ~/Applications.')


def sdk_path(session: Path, explicit: str | None) -> Path:
    value = explicit or os.environ.get('AAX_SDK_PATH')
    cache = session / 'build/CMakeCache.txt'
    if not value and cache.is_file():
        for line in cache.read_text().splitlines():
            if line.startswith('AAX_SDK_PATH:PATH='):
                value = line.split('=', 1)[1]
    if not value or not (Path(value).expanduser() / 'Interfaces/AAX.h').is_file():
        raise RuntimeError('AAX needs an SDK folder containing Interfaces/AAX.h. Set AAX_SDK_PATH or pass --aax-sdk.')
    return Path(value).expanduser().resolve()


def plugin_path(session: Path, format_name: str) -> Path:
    extension = {'VST3': '.vst3', 'AU': '.component', 'AAX': '.aaxplugin'}[format_name]
    return session / 'build/HelloJUCE_artefacts/Debug' / format_name / ('Hello JUCE' + extension)


def install_plugin(source: Path, format_name: str) -> None:
    if not source.is_dir():
        raise RuntimeError(f'Plugin build product missing: {source}')
    if format_name == 'AAX':
        folder = Path('/Library/Application Support/Avid/Audio/Plug-Ins')
    else:
        folder = Path.home() / 'Library/Audio/Plug-Ins' / {'VST3': 'VST3', 'AU': 'Components'}[format_name]
    folder.mkdir(parents=True, exist_ok=True)
    destination = folder / source.name
    # A link tracks rebuilds without copying or modifying an installed bundle.
    if destination.is_symlink():
        if destination.resolve() != source.resolve():
            raise RuntimeError(f'A different plugin already occupies {destination}; move it before launching.')
    elif destination.exists():
        raise RuntimeError(f'A plugin already exists at {destination}; move it before launching.')
    else:
        destination.symlink_to(source.resolve(), target_is_directory=True)
    print(f'Plugin available at: {destination}', flush=True)


def prepare(session: Path, format_name: str, host: str | None,
            aax_sdk: str | None = None, dry_run: bool = False) -> Path:
    executable = host_executable(host) if host else session / 'build/HelloJUCE_artefacts/Debug/Standalone' / (
        'Hello JUCE.app/Contents/MacOS/Hello JUCE' if sys.platform == 'darwin'
        else 'Hello JUCE.exe' if sys.platform == 'win32' else 'Hello JUCE')
    configure = ['cmake', '-S', str(session), '-B', str(session / 'build'),
                 '-G', 'NMake Makefiles' if sys.platform == 'win32' else 'Unix Makefiles',
                 '-DCMAKE_BUILD_TYPE=Debug']
    if format_name == 'AAX':
        configure.append(f'-DAAX_SDK_PATH={sdk_path(session, aax_sdk)}')
    build = ['cmake', '--build', str(session / 'build'), '--config', 'Debug',
             '--target', f'HelloJUCE_{format_name}']
    for command in (configure, build):
        print(subprocess.list2cmdline(command), flush=True)
        if not dry_run:
            subprocess.run(command, check=True)
    if host and not dry_run:
        install_plugin(plugin_path(session, format_name), format_name)
        print(f'Ready to debug {host}. Insert Hello JUCE on an audio track after the host opens. '
              'Close an existing host instance before starting a new debug session.', flush=True)
    return executable
