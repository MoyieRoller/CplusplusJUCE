#include <iostream>
#include <typeinfo>

int main()
{
    // bpm has type const double: a floating-point value that cannot be reassigned.
    const double bpm = 137.0;

    // Uncomment this line to see how the compiler reports an assignment to const.
    // bpm = 136.0;

    std::cout << "Tempo: " << bpm << " BPM\n";

    if (bpm <= 0.0)
    {
        std::cout << "Invalid tempo.\n";
        return 1; // Exit main early with an error status, before dividing by bpm.
    }

    if (bpm < 60.0)
    {
        std::cout << "Slow tempo.\n";
    }
    else if (bpm < 120.0)
    {
        std::cout << "Moderate tempo.\n";
    }
    else if (bpm <= 180.0)
    {
        std::cout << "Fast tempo.\n";
    }
    else
    {
        std::cout << "High tempo.\n";
    }

    // Uncomment the next two lines to compare integer and floating-point division.
    // std::cout << "60000 / 137 = " << 60000 / 137 << '\n';
    // std::cout << "60000.0 / 137 = " << 60000.0 / 137 << '\n';

    // auto asks the compiler to deduce the type from the expression on the right.
    // bpm is double, so even 60000 / bpm uses floating-point division.
    auto msPerBeat = 60000.0 / bpm;

    // msPerBeat is double. The type name is compiler-dependent; it may be "d"
    // rather than "double". This output is only a demonstration.
    std::cout << "Type of msPerBeat: " << typeid(msPerBeat).name() << '\n';

    // Print the duration of a beat in milliseconds.
    std::cout << "Duration per beat: " << msPerBeat << " ms\n";

    return 0;
}
