#include <iostream>
#include <cassert>

double beatDurationsMS(double bpm)
{
   // assert (bpm > 0.0);

    if (bpm <= 0) {
        return -1;
    }

    double dur = 60000.0 / bpm;
    return dur;

}

double twoBeatsMs(double bpm)
{
    double doubledDur = beatDurationsMS(bpm) * 2;
    return doubledDur;
}

double fourBeatsMs(double bpm)
{
    double quadroupledDur = twoBeatsMs(bpm) * 2;
    return quadroupledDur;
}

double barDurationMs(double bpm, int beatsPerBar)
{
    double barDur = beatDurationsMS(bpm) * beatsPerBar;
    return barDur;
}

int main()
{
    std::cout << "BPMs in ms:\n";
    std::cout << beatDurationsMS(60.0) << " ms\n";
    std::cout << beatDurationsMS(120.0) << " ms\n";
    std::cout << beatDurationsMS(150.0) << " ms\n";

    std::cout << "\nBPMs for two bars in ms:\n";
    std::cout << twoBeatsMs(60.0) << " ms\n";
    std::cout << twoBeatsMs(120.0) << " ms\n";
    std::cout << twoBeatsMs(150.0) << " ms\n";

    std::cout << "\nBPMs for four bars in ms:\n";
    std::cout << fourBeatsMs(60.0) << " ms\n";
    std::cout << fourBeatsMs(120.0) << " ms\n";
    std::cout << fourBeatsMs(150.0) << " ms\n";

    std::cout << "\nBPMs for n beats in ms:\n";
    std::cout << barDurationMs(120.0, 3) << " ms\n";
    std::cout << barDurationMs(120.0, 4) << " ms\n";

    return 0;
}
