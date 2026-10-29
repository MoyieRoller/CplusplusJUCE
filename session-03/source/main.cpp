#include <array>
#include <cstddef>
#include <iostream>

int main()
{
    // Keep this array non-const so task 5 can change one of its values.
    std::array<int, 4> notes { 60, 62, 64, 65 };

    // Index-based iteration: index is available only inside this for loop.
    for (std::size_t index = 0; index < notes.size(); ++index)
    {
        std::cout << "Note at index " << index << ": " << notes[index] << "\n";
    }

    // Uncommenting this line causes an error because index is out of scope here.
    // std::cout << index << "\n";

    // In C++, braces define a block. Indentation alone does not affect scope.
    std::cout << "=========\n\n";

    // Count backward from one past the last index.
    for (std::size_t i = notes.size(); i > 0; --i)
    {
        const std::size_t index = i - 1;
        std::cout << "Note at index " << index << ": " << notes[index] << "\n";
    }

    std::cout << "=========\n\n";

    // Here i starts one past the end and is decremented at the start of the body.
    for (std::size_t i = notes.size(); i > 0;)
    {
        --i;
        std::cout << "Note at index " << i << ": " << notes[i] << "\n";
    }

    std::cout << "=========\n\n";

    // An empty for loop condition repeats forever; break exits it immediately.
    for (;;)
    {
        std::cout << "This loop runs once, then break exits it.\n";
        break;

        std::cout << "This line is never reached.\n";
    }

    std::cout << "=========\n\n";

    // Range-based iteration visits each value in the array.
    for (int note : notes)
    {
        std::cout << note << " ";
    }

    std::cout << "\n=========\n\n";

    // Print each note transposed up one octave without changing the array.
    for (int note : notes)
    {
        std::cout << note + 12 << " ";
    }

    std::cout << "\n=========\n\n";

    // Start at 1 and continue while i is less than or equal to 4.
    // The loop therefore prints 1, 2, 3, and 4.
    for (int i = 1; i <= 4; ++i)
    {
        std::cout << "Beat number: " << i << "\n";
    }

    std::cout << "=========\n\n";

    // Change the first note, then show that the loop still visits all four.
    notes[0] = 67;
    std::cout << "After changing the first note: ";
    for (int note : notes)
    {
        std::cout << note << " ";
    }
    std::cout << "\n=========\n\n";

    // Count down from 4 to 1. The loop stops before printing 0.
    int countdown = 4;
    while (countdown > 0)
    {
        std::cout << "Countdown: " << countdown << "\n";
        --countdown;
    }

    std::cout << "=========\n\n";

    // Practice after class: print eight notes and count transposed notes >= 76.
    const std::array<int, 8> extendedNotes { 60, 62, 64, 65, 67, 69, 71, 72 };
    std::size_t index = 0;
    int countTransposedAtLeast76 = 0;

    while (index < extendedNotes.size())
    {
        const int transposed = extendedNotes[index] + 12;
        std::cout << "Original note: " << extendedNotes[index]
                  << ", transposed note: " << transposed << "\n";

        if (transposed >= 76)
        {
            ++countTransposedAtLeast76;
        }

        ++index;
    }

    std::cout << "Transposed notes at least 76: "
              << countTransposedAtLeast76 << "\n";
}
