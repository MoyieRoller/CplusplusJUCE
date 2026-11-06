#pragma once

#pragma message("Including Tempo.h")

// The declaration of a function specifies:
// - its return type
// - its name
// - what parameters it takes
double beatDurationMs(double bpm);
double twoBeatsMs(double bpm);
double barDurationMs(double bpm, int beatsPerBar);
