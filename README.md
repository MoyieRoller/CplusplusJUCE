# JUCE C++ 1 — Musikhochschule Karlsruhe

A practical introduction to C++ and JUCE for Music Informatics students. The course roadmap covers 17 sessions; topics and pacing may be adjusted as we learn what the group needs.

## Start here

We will set up VS Code and the C++ tools together in class. Bring your laptop. Use the [setup sheet](setup/README.md) during class, then begin with [Session 01](session-01/README.md). The full list of planned topics is in the [student roadmap](COURSE_PLAN.md).

Use the [Git introduction](GIT_INTRO.md) to review cloning, branches, staging, commits, and sharing your work.

JUCE itself is introduced later, so you do not need to install it before the first class.

## How the course works

- You will build your own small programs and keep your own code changes. Use the teacher's course files as a reference; your solution does not need to match them line for line.
- The course begins with console programs and later builds one modest JUCE application: one window, one or two controls, and one result.

To build and debug setup or Sessions 01–09 from the course root, choose **Build and launch a course session — your platform** in Run and Debug and press **F5**. Select the session; its console program builds and runs without host or plugin-format prompts. Sessions 01–06 use direct compiler commands; Sessions 07–09 use CMake. On macOS/Linux the launcher runs the original executable through a launch link rather than copying it. These root launchers require Python 3; Windows users should start VS Code from a Developer Command Prompt. You can also open an individual session folder and use its own Run and Debug configuration.

[Session 10](session-10/README.md) introduces a minimal CMake JUCE plugin that paints “Hello JUCE”. Follow [JUCE setup](JUCE_SETUP.md), then choose **Build and Launch a JUCE Plugin — your platform**. Its native VS Code dropdowns select **plugin session → app to launch → target to build**, for example **session-10 → Standalone → App**. This launcher is for Session 10 and later plugin sessions; only Session 10 is currently available in its session list. Hosted launch supports compatible VST3, AU, and AAX hosts on macOS. Audio processing remains outside the required scope.
