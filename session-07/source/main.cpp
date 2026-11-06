#include <iostream>
#include "Tempo.h"

#pragma message("Compiling main.cpp")

int main()
{
    std::cout << "One beat: " << beatDurationMs(120.0) << " ms\n";
    std::cout << "Two beats: " << twoBeatsMs(120.0) << " ms\n";
}