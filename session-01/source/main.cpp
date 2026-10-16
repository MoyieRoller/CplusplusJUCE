#include <iostream>

int main (int argc, char* argv[])
{
    // type
    double bpm = 120.0;
    // double bpm{ 120.0 };
    double ms_per_quarter_note = 60000 / 120;
    double ms_per_eighth_note = ms_per_quarter_note / 2;
    double ms_per_quarter_note_dotted = ms_per_quarter_note * 1.5;
    double ms_per_eighth_note_dotted = ms_per_eighth_note * 1.5;
    
    std::cout << "Duration quarter note: " << ms_per_quarter_note << "ms" << "\n";
    std::cout << "Duration eighth note: " << ms_per_eighth_note << "ms" << "\n";
    std::cout << "Duration quarter note dotted: " << ms_per_quarter_note_dotted << "ms" << "\n";
    std::cout << "Duration eighth note dotted: " << ms_per_eighth_note_dotted << "ms" << "\n";
    return 0;
}
