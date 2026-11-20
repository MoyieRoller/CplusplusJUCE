# Stack and heap

Look at the two objects in Session 09 called `secondOriginal` and `new Tempo(90.0)`. Both are instances of the same class. The difference is how long each one lives and who is responsible for cleaning it up.

## Stack

An ordinary local variable typically lives on the stack. In the example, `Tempo secondOriginal{ 75.0 };` creates a local object inside a pair of braces. At the closing brace, C++ calls its destructor automatically. That is why you see `Destroy 75` while `main` is still running.

## Heap

The heap is memory a program can request while it runs. In `Tempo* newTempo = new Tempo(90.0);`, there are two things to keep track of. The local variable `newTempo` holds an address. The `Tempo` object itself lives on the heap.

The heap object can outlive the local pointer. In this example, `delete newTempo;` calls the object's destructor and releases its memory. The pointer variable stays in scope and still holds the old address. The object at that address is gone.

## Who cleans up?

An *owner* is responsible for an object's lifetime. In the inner block with `tempoPtr`, the local `std::unique_ptr<Tempo>` owns a heap object. Calling `reset` with a replacement destroys the previously owned object. At the closing brace, the smart pointer is destroyed and releases the object it owns at that point.

## Why this matters for audio

Allocating or deallocating heap memory can take a little time, and the time can vary from one call to the next. An audio callback has a deadline for each block of samples. If it misses that deadline, you may hear a click or dropout. A common approach is to allocate buffers before playback, reuse them during the callback, and release them after processing stops. A smart pointer manages cleanup, but that cleanup still takes time on the thread where it happens.
