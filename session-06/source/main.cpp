#include <iostream>
#include "Tempo.h"

#pragma message("Compiling main.cpp")

int main()
{
    std::cout << beatDurationMs(120.0) << "\n";
    std::cout << twoBeatsMs(120.0) << "\n";

    return 0;
}