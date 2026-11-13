# Session 10 — A first JUCE plugin with CMake

Set up a minimal plugin that paints **Hello JUCE**. There are no controls, parameters, or audio effects yet: audio passes through unchanged.

## What you will learn

Add JUCE to CMake, create a plugin target, generate `JuceHeader.h`, and distinguish the audio processor from its editor.

## Setup and build

Follow [JUCE setup](../JUCE_SETUP.md) first (the course-local JUCE submodule, CMake 3.22 or newer). Open **session-10** in VS Code.

Configure on macOS/Linux:

```bash
cmake -S . -B build -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
```

On Windows, launch VS Code from a Developer Command Prompt and configure with:

```powershell
cmake -S . -B build -G "NMake Makefiles" -DCMAKE_BUILD_TYPE=Debug
```

JUCE must be in `../dependencies/JUCE`. If it is missing, configuration stops with an error. Run `git submodule update --init --recursive` from the course root.

```bash
cmake --build build --config Debug
```

## Build and debug with F5

Open this session folder, select **Lesson — your platform**, and press **F5**. Alternatively, open the course root, select **Build and Launch a JUCE Plugin — your platform**, press **F5**, then select **session-10**. For a standalone run, the complete selection is **session-10 → Standalone → App**.

Two native VS Code dropdowns always appear: first **Standalone / REAPER / Cubase 15 / Logic / Pro Tools Developer**, then **App / VST3 / AU / AAX**. Incompatible combinations stop with a clear error. Use these combinations:

| Format | Host |
| --- | --- |
| App | Standalone |
| VST3 | REAPER or Cubase 15 |
| AU | REAPER or Logic |
| AAX | Pro Tools Developer |

The launcher builds only the selected target and prepares the Standalone application or host for VS Code to launch under the debugger. Cancel stops the launch. Close an existing host instance before starting a new debug session. Once the host opens, insert **Hello JUCE** on an audio track; the launcher does not create or modify a DAW project. Hosts may require a plugin rescan. If a host runs plugins in a separate process, debugging that plugin requires attaching to its process.

Hosted launch is configured for macOS. Windows/Linux retain Standalone debugging. All selections use VS Code dropdowns; no external dialogs or terminal prompts are opened.

AAX requires an SDK path before launching. Set the `AAX_SDK_PATH` environment variable or configure `-DAAX_SDK_PATH="/absolute/path/to/AAX-SDK"`; CMake remembers it. AAX builds require the SDK and use Pro Tools Developer for testing the development plugin.

Build products are under `build/HelloJUCE_artefacts/Debug/`:

- macOS: `Standalone/Hello JUCE.app`
- Windows: `Standalone/Hello JUCE.exe`
- Linux: `Standalone/Hello JUCE`
- VST3: `VST3/Hello JUCE.vst3`
- AU (macOS): `AU/Hello JUCE.component`
- AAX (SDK required): `AAX/Hello JUCE.aaxplugin`

For hosted launch, the launcher links the selected build into `~/Library/Audio/Plug-Ins/VST3`, `~/Library/Audio/Plug-Ins/Components`, or `/Library/Application Support/Avid/Audio/Plug-Ins` for AAX. The AAX directory must be writable. Existing unrelated installations are not overwritten. Plain CMake builds do not install plugins.

For a noninteractive launch preparation from the course root:

```bash
python3 .vscode/build_session.py session-10 --format VST3 --host REAPER
python3 .vscode/build_session.py session-10 --format AU --host Logic --dry-run
python3 .vscode/build_session.py session-10 --format AAX --host "Pro Tools Developer"
```

These commands prepare the launch target; F5 starts the debugger.

## Tasks

1. Locate `add_subdirectory`, `juce_add_plugin`, `target_sources`, and `target_link_libraries`. Explain what each supplies.
2. Find `juce_generate_juce_header(HelloJUCE)`. It generates `JuceHeader.h` from the linked JUCE modules; do not create or edit this generated header yourself.
3. Build and open the Standalone application. Confirm the editor shows **Hello JUCE**.
4. Follow `createPluginFilter` → processor → `createEditor` → editor. The processor handles audio; the editor paints the interface.
5. Find `paint`. Change the text colour or font size, rebuild, and observe the result.
6. Explain why `processBlock` leaves the audio buffer unchanged. Leave audio processing for a later session.

## Check your result

Both Standalone and VST3 build. The editor has a dark background and centred white **Hello JUCE** text. There are no controls. In a host, mono/stereo input passes through unchanged.

## Individual checkpoint

Explain the difference between a generated header, a C++ source file, and a linked module. Identify which class owns the painting code.

## Practice after class

Recreate the CMake setup in a separate folder using these four source files. Build it and explain each CMake command used.
