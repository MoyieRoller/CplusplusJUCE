# Require CMake 3.22 or newer to configure this project.
cmake_minimum_required(VERSION 3.22)

# Name the project Session07, give it version 1.0.0, and enable C++ (CXX).
project(Session07 VERSION 1.0.0 LANGUAGES CXX)

# Put executables in the bin folder inside the build directory.
# CMAKE_BINARY_DIR is the build directory selected with cmake -B.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Define an executable target named main and list the source files to compile.
add_executable(main
    source/main.cpp  # Contains main(), where the program starts.
    source/Tempo.cpp # Contains the implementation of beatDurationMs().
) # End the executable's source list; both object files will be linked together.

# Require at least C++17 for main. PRIVATE applies this requirement to this target
# without passing it on to other targets that might depend on it.
target_compile_features(main PRIVATE cxx_std_17)
