#include <iostream>

double beatDurationMs(int bpm)
{
    double result = 60000.0 / bpm;
    return result;
}

double twoBeatsMs(int bpm)
{
    double oneBeat = beatDurationMs(bpm);
    return 2.0 * oneBeat;
}

double threeBeatsMs(int bpm)
{
    double oneBeat = beatDurationMs(bpm);
    return 3.0 * oneBeat;
}

int main()
{
    int bpm = 137;
    double durTwo = twoBeatsMs(bpm);
    double durThree = threeBeatsMs(bpm);
    std::cout << durTwo << " ms\n";
    std::cout << durThree << " ms\n";
}
