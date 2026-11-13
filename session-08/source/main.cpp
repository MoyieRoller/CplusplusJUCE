#include <iostream>

class Tempo
{
public:
    // Constructor: has the class name and no return type.
    // Called when an object is created; explicit prevents implicit conversion
    // from a BPM value to a Tempo object.
    explicit Tempo(double initialBpm)
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

    // TODO: add a const method that returns beat duration in milliseconds.
    const double getBeatDurationMs() const { return 60000.0 / bpm; }
    
private:
    double bpm;
};

int main()
{
    // Type: Tempo; object name: first; constructor argument: 120.0.
    Tempo first(120.0);
    Tempo second(60.0);

    std::cout << first.getBpm() << " BPM\n";
    std::cout << second.getBpm() << " BPM\n";

    return 0;
}
