#include <iostream>

class Tempo
{
public:
    // Constructor: has the class name and no return type.
    // Called when an object is created; explicit prevents implicit conversion
    // from a BPM value to a Tempo object. See Example below this class.
    explicit Tempo(double initialBpm = 120.0)
        : bpm(initialBpm)
    {
        // The member initializer list above initializes bpm before this body runs.
        // If there are several members, they are initialized in declaration order.
        // No additional work is needed in the constructor body here.
    }

    // Destructor: has the class name prefixed with ~ and no return type.
    // Called when an object's lifetime ends. This class needs no custom cleanup;
    // it is shown here for illustration and could be omitted.
    ~Tempo() {}

    // const means this method cannot modify the object's ordinary member data.
    double getBpm() const { return bpm; }

    double beatDurationMs() const { return 60000.0 / bpm; }

    void setBpm(double newBpm)
    {
        if (newBpm > 0.0)
            bpm = newBpm;
    }

private:
    double bpm; // Initialized by the constructor using initialBpm.
};

/*
    Explicit / Implicit Constructor Example 
    
    setTempo (Tempo tempo) {}

    Calling this function using setTempo (25.0) while the class Tempo has an
    explicit Constructor will produce a compile error. We'd need to explicitly
    call the correct constructor using setTempo (Tempo (25.0)). When Tempo has
    an implicit Constructor (without the keyword explicit) we can call
    setTempo (25.0) which will directly construct an instance of Tempo.
*/
void setTempo (Tempo tempo) {}

void printTempo(const char* label, const Tempo& tempo)
{
    std::cout << label << ": " << tempo.getBpm() << " BPM, "
              << tempo.beatDurationMs() << " ms per beat\n";
}

int main()
{
    Tempo first; // Omitting the argument uses the constructor's default of 120 BPM.
    Tempo second(60.0); // Try 80.0 here. Only the second object's output changes.
    printTempo("First", first);
    printTempo("Second", second);

    // One minute has 60000 ms. At 100 beats per minute, each beat lasts 600 ms.
    Tempo third(100.0);
    printTempo("Third", third);

    first.setBpm(150.0);
    printTempo("First after setting 150", first);
    first.setBpm(0.0);
    printTempo("First after trying 0", first);
    first.setBpm(-20.0);
    printTempo("First after trying -20", first);
    printTempo("Second after changing first", second);

    return 0;
}
