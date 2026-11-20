#include "Tempo.h"

#pragma message("Compiling Tempo.cpp")

// The definition of a function specifies:
// - what it does
double beatDurationMs(double bpm)
{
    return 60000.0 / bpm;
}

double twoBeatsMs(double bpm)
{
    return 2.0 * beatDurationMs(bpm);
}

double barDurationMs(double bpm, int beatsPerBar)
{
    return beatsPerBar * beatDurationMs(bpm);
}
