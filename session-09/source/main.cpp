#include <cassert>
#include <cstddef>
#include <iostream>
#include <memory>
struct Tempo {
  explicit Tempo(double initialBpm) : bpm(initialBpm) {
    std::cout << "Construct " << bpm << "\n";
  }
  ~Tempo() { std::cout << "Destroy " << bpm << "\n"; }
  double bpm;
};

void setTempo(Tempo value, double newBpm) { value.bpm = newBpm; }

// What does std::unique_ptr do? Example class for managing instances of Tempo
// on heap using an own simple smart pointer.
//
// Work in progress (WIP) with many problems, we'll speak about that next class.
// Without the deleted copy operations below, copying would copy only the
// address. Both wrappers would then try to delete the same Tempo. Copy
// assignment would also lose the destination's old address. Try copying now: it
// fails to compile.
class ManagedTempoPointer {
public:
  ManagedTempoPointer() {}

  // deleted copy constructor. we can't copy an instance of
  // ManagedTempoPointer anymore!
  ManagedTempoPointer(ManagedTempoPointer &other) = delete;

  // deleted copy assignment operator
  ManagedTempoPointer &operator=(const ManagedTempoPointer &other) = delete;

  ~ManagedTempoPointer() {
    // delete destroys the owned Tempo. Deleting nullptr is harmless.
    // Deleting an address whose object was already deleted is undefined
    // behavior.
    delete tempoPointer;
  }

  Tempo *get() { return tempoPointer; }

  Tempo *operator->() { return tempoPointer; }

  operator bool() { return tempoPointer != nullptr; }

  // This function takes ownership. The caller may still have a copy of
  // the address, but only this wrapper should delete the object.
  // What if newTempo is the non-null address we already own? Our assertion
  // catches this when enabled. Without it, delete destroys the object and
  // we then store its dangling address.
  // Passing the address of a local Tempo would also make a later delete
  // invalid.
  void reset(Tempo *newTempo = nullptr) {
    // Reject resetting to the same non-null pointer while assertions are
    // enabled. To disable assertions, define NDEBUG before including <cassert>,
    // or build with -DNDEBUG (GCC/Clang) or /DNDEBUG (MSVC). The check then
    // disappears; it does not make resetting to the same pointer safe.
    assert(newTempo == nullptr || newTempo != tempoPointer);

    delete tempoPointer;
    tempoPointer = newTempo;
  }

private:
  Tempo *tempoPointer;
};

template <class T> class ManagedPointer {
public:
  T *ptr;

  T *get() { return ptr; }

  T *operator->() { return ptr; }

  operator bool() { return ptr != nullptr; }
};

int main() {
  ManagedPointer<Tempo> mp;
  // A new object on the stack is created and is destroyed at the
  // end of its scope which is the end of function body of main ()
  Tempo original(120.0);

  // setTempo (original, 25.0); // Uncomment: does passing original by value
  // change its BPM?
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

    // reset deletes the previously managed Tempo and takes ownership
    // of the new Tempo (76.0).
    tempoPtr.reset(new Tempo(76.));
  }

  // Example with an own type of smart pointer managing only the lifetime
  // of instances of Tempo
  ManagedTempoPointer mtp;
  mtp.reset(new Tempo(12));

  // Ownership problems and the protections we have added so far.
  // Try one example at a time; some deliberately fail to compile or assert.

  // What happens to the 11 BPM Tempo when we pass the same address to reset a
  // second time? Our assertion now stops the second reset when assertions are
  // enabled. auto tempoAssignedTwice = new Tempo (11); mtp.reset
  // (tempoAssignedTwice); // Deletes the old 12 BPM Tempo and takes ownership
  // of the 11 BPM Tempo. mtp.reset (tempoAssignedTwice); // Asserts; without
  // assertions, deletes the 11 BPM Tempo and leaves a dangling pointer.

  // Try this separately with a fresh 11 BPM allocation and without the mtp
  // resets. unique_ptr does not protect against resetting to its own pointer:
  // the second reset deletes the object, leaving a dangling pointer and a later
  // invalid delete. std::unique_ptr<Tempo> tUnique; tUnique.reset
  // (tempoAssignedTwice); tUnique.reset (tempoAssignedTwice);

  // Without our deleted copy constructor, would copying mtp create a second
  // Tempo or just copy its address? What would happen when both wrappers die?
  // With the deleted copy constructor, the following copy fails to compile.
  // ManagedTempoPointer copyOfMtp = mtp;
  // std::unique_ptr<Tempo> tUnique = std::make_unique<Tempo> (12.0);
  // std::unique_ptr<Tempo> copyOfTUnique = tUnique;

  // unique_ptr cannot be copied. We also prevent copy construction of
  // ManagedTempoPointer by deleting its copy constructor. Tempo itself is still
  // copyable. std::unique_ptr<Tempo> tUnique = std::make_unique<Tempo> (12.0);
  // std::unique_ptr<Tempo> copyOfTUnique = tUnique;

  // This attempts copy construction of a new wrapper and fails to compile
  // because ManagedTempoPointer's copy constructor is deleted.
  // ManagedTempoPointer copyOfMtp = mtp;

  // Copy construction creates a new object; copy assignment changes an existing
  // one. secondMtp already owns a 100 BPM Tempo. Without our deleted copy
  // assignment operator, assignment would overwrite its pointer and leak that
  // Tempo. Deleting the copy constructor alone does not prevent assignment, so
  // we delete the copy assignment operator too. The assignment below fails to
  // compile. ManagedTempoPointer secondMtp; secondMtp = mtp;

  // unique_ptr cannot be copied, but ownership can be transferred by moving.
  // std::move enables move assignment here; the assignment transfers ownership.
  // The Tempo object itself stays at the same address.
  // Can our ManagedTempoPointer transfer ownership like this yet? What special
  // member functions might we need, and what should happen to each wrapper's
  // pointer? std::unique_ptr<Tempo> tUnique = std::make_unique<Tempo> (12.0);
  // // tUnique points to lets say 0x1234 std::unique_ptr<Tempo> secondTUnique;
  // // Empty: holds nullptr. secondTUnique = std::move (tUnique); //
  // secondTUnique now owns the Tempo; tUnique holds nullptr.

  // Check both pointers in the debugger. secondTUnique was empty, so move
  // assignment destroys no previous Tempo. If it had owned one, that object
  // would have been deleted before it took ownership of tUnique's Tempo.
  // Back to mtp: how often do we see "Destroy 12" when we reset it to nullptr
  // twice? mtp.reset (nullptr); mtp.reset (nullptr);

  // The default argument nullptr in reset's declaration lets us omit the
  // argument. mtp.reset (); mtp.reset (nullptr);

  // What happens if we try to copy a std::unique_ptr?
  // auto owner = std::make_unique<Tempo> (80.0);
  // auto copyOfOwner = owner;

  // Now comment out the copy, keep owner above and try moving it.
  // For std::move, add #include <utility> at the top.
  // auto nextOwner = std::move (owner);
  // Check both pointers. Did moving the pointer call the Tempo constructor or
  // destructor?

  std::unique_ptr<Tempo> uniqueTempo;
  uniqueTempo.reset(new Tempo(5));
  mtp.reset(new Tempo(14.0));

  uniqueTempo.get()->bpm;

  mtp.get()->bpm;
  mtp->bpm;

  std::cout << "operator->" << mtp->bpm << "\n";
  mtp.reset();
}