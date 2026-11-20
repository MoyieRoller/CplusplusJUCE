# Before session 10: add JUCE

Sessions 1–9 do not need JUCE. Session 10 uses the official [JUCE repository](https://github.com/juce-framework/JUCE) as a Git submodule in `dependencies/JUCE`, tracking `master`. The course repository records the exact submodule commit for reproducible checkouts.

## Obtain the source

For an existing course checkout, run from the course root:

```bash
git submodule update --init --recursive
```

For a new checkout, use `git clone --recurse-submodules <course-repository-url>`.

```text
my-course/
  dependencies/
    JUCE/
      CMakeLists.txt
      modules/
  session-10/
    CMakeLists.txt
    source/
```

Session 10 uses this course-local directory by default. Configuration does not download JUCE. To deliberately update to the latest official `master` commit:

```bash
git submodule update --remote dependencies/JUCE
```

Build and test before committing the updated submodule pointer. Normal `git submodule update --init --recursive` restores the commit recorded by the course repository.

## Linux GUI prerequisites

On Ubuntu/Debian, the GUI lessons also need development packages:

```bash
sudo apt update
sudo apt install pkg-config libfontconfig1-dev libfreetype-dev libx11-dev libxcomposite-dev libxcursor-dev libxext-dev libxinerama-dev libxrandr-dev libxrender-dev
```

If your distribution provides `libfreetype6-dev` instead of `libfreetype-dev`, use that package. Other distributions need equivalents. Use a desktop session with a display to run GUI applications. The Session 10 Standalone target also needs ALSA development headers (`libasound2-dev` on Ubuntu/Debian). A web browser is not enabled.

## Build

Open the session folder and follow its README. The first JUCE configuration/build can take longer than the earlier console exercises because JUCE also builds its helper tools and modules. Let it finish before diagnosing a failure.

Session 10 always uses `dependencies/JUCE` in the course root. There is no configurable JUCE path or cached path override. If the submodule is missing, configuration stops and prints the command to initialize it. Never commit the build directory.

## What the project supplies

The CMake file adds the JUCE source directory, creates a plugin, console, or GUI target, lists the lesson sources, and links the required module. A C++ `#include` supplies declarations; it does not replace those build steps. Session 10 uses `juce_audio_utils` and builds Standalone and VST3 targets, plus AU on macOS and AAX when an SDK path is supplied. Every JUCE CMake project calls `juce_generate_juce_header` so its source can include `<JuceHeader.h>`.

References: [JUCE CMake guide](https://github.com/juce-framework/JUCE/blob/master/docs/CMake%20API.md) and [Linux dependencies](https://github.com/juce-framework/JUCE/blob/master/docs/Linux%20Dependencies.md).
