#include <cassert>
#include <iostream>
#include <memory>

struct Tempo {
  explicit Tempo(double initialBpm) : bpm(initialBpm) {
    std::cout << "Construct " << bpm << "\n";
  }
  ~Tempo() { std::cout << "Destroy " << bpm << "\n"; }
  double bpm;
};

void setTempo(Tempo value) { value.bpm = 60.0; }

// What does std::unique_ptr do? Example class for managing instances of Tempo
// on heap using an own simple smart pointer.
//
// Work in progress (WIP) with many problems, we'll speak about that next class.
// Copying this wrapper copies the address. Both wrappers would then try to
// delete the same Tempo. Copy assignment also loses the destination's old
// address.
class ManagedTempoPointer {
public:
  ManagedTempoPointer() {
    // automatically create a new instance of Tempo on heap
    tempoPointer = new Tempo(100.0);
  }

  ManagedTempoPointer(ManagedTempoPointer &other) = delete;

  ManagedTempoPointer &operator=(const ManagedTempoPointer &other) = delete;

  ~ManagedTempoPointer() {
    // delete destroys the owned Tempo. Deleting nullptr is harmless.
    // Deleting an address whose object was already deleted is undefined
    // behavior.
    delete tempoPointer;
  }

  // This function takes ownership. The caller may still have a copy of
  // the address, but only this wrapper should delete the object.
  // What if newTempo is the address we already own? The delete below
  // destroys that object, and we then store its dangling address.
  // Passing the address of a local Tempo would also make a later delete
  // invalid.
  void reset(Tempo *newTempo) {
    assert(newTempo != tempoPointer);
    delete tempoPointer;
    tempoPointer = newTempo;
  }

private:
  Tempo *tempoPointer;
};

int main() {
  // A new object on the stack is created and is destroyed at the
  // end of its scope which is the end of function body of main ()
  Tempo original(120.0);

  // setTempo (original, Tempo (25.0)); // uncomment, to see if this changes
  // anything
  std::cout << "Original: " << original.bpm << "\n";

  {
    // secondOriginals scope ends at the next '}' one line later
    Tempo secondOriginal{75.0};
  } // secondOriginal goes out of scope

  // create a new Tempo object on the heap and store a pointer to it
  Tempo *newTempo = new Tempo(90.0);

  // when newTempo goes out of scope at the end of main, the object
  // it points to will NOT be deleted. We have to do it manually:
  delete newTempo; // delete the object and free the memory

  // BUT: newTempo still holds the old address. Using it to read the BPM or
  // deleting it a second time would be undefined behavior.

  {
    // For not thinking about that too much let's use smart pointers such
    // as std::unique_ptr which will manage the lifetime of the object
    // it points to. When tempoPtr goes out of scope it automatically
    // deletes the instance of Tempo it's currently managing.
    std::unique_ptr<Tempo> tempoPtr = std::make_unique<Tempo>(75.0);

    // calling the reset function with a new instance of Tempo will
    // delete the last managed instance and reset tempoPtr to reset
    // the new Tempo (76.)
    tempoPtr.reset(new Tempo(76.));
  }

  // Example with an own type of smart pointer managing only the lifetime
  // of instances of Tempo
  ManagedTempoPointer mtp;
  mtp.reset(new Tempo(12));

  // Current problems with our class ManagedTempoPointer
  // Try one example at a time.

  // What happens to the 11 BPM Tempo when we pass the same address to reset a
  // second time?
  auto tempoAssignedTwice = new Tempo(11);
  mtp.reset(tempoAssignedTwice);
  mtp.reset(tempoAssignedTwice);

  // When we copy mtp, do we get a second Tempo or just a copy of its address?
  // What happens when both wrappers go out of scope?
  // ManagedTempoPointer copyOfMtp = mtp;
  // std::unique_ptr<Tempo> tUnique = std::make_unique<Tempo>(12.0);
  // std::unique_ptr<Tempo> copyOfTUnique = tUnique;

  // What happens to the 100 BPM Tempo that secondMtp already owns?
  ManagedTempoPointer secondMtp;
  secondMtp = mtp;

  // std::unique_ptr<Tempo> tUnique = std::make_unique<Tempo>(12.0);
  // std::unique_ptr<Tempo> secondTUnique;
  // secondTUnique = tUnique;
  // Check both addresses in the debugger. Did we see "Destroy 100" during
  // assignment?

  // How often do we see "Destroy 12" when we reset to nullptr twice?
  mtp.reset (nullptr);
  mtp.reset (nullptr);

  // What happens if we try to copy a std::unique_ptr?
  // auto owner = std::make_unique<Tempo> (80.0);
  // auto copyOfOwner = owner;

  // Now comment out the copy, keep owner above and try moving it.
  // For std::move, add #include <utility> at the top.
  // auto nextOwner = std::move (owner);
  // Check both pointers. Did moving the pointer call the Tempo constructor or
  // destructor?
}
