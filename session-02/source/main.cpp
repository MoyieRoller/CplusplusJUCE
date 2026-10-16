#include <iostream>

double calculateDuration(double bpm)
{
    auto ms_per_quarter_note = 60000 / bpm;
    return ms_per_quarter_note;
}

int main()
{
    const auto bpm = 0;

    if (bpm <= 0) {
        std::cout << "ERROR: Invalid tempo!" << std::endl;
        return 1;
    } else if (bpm < 60) {
        auto dur = calculateDuration(bpm);
        std::cout << "Tempo: " << bpm << " bpm\n";
        std::cout << "BPM Classification: slow\n";
        std::cout << "Duration of one beat: " << dur << "ms" << std::endl;
    } else if (bpm < 120) {
        auto dur = calculateDuration(bpm);
        std::cout << "Tempo: " << bpm << " bpm\n";
        std::cout << "BPM Classification: moderate\n";
        std::cout << "Duration of one beat: " << dur << "ms" << std::endl;
    } else if (bpm <= 180) {
        auto dur = calculateDuration(bpm);
        std::cout << "Tempo: " << bpm << " bpm\n";
        std::cout << "BPM Classification: fast\n";
        std::cout << "Duration of one beat: " << dur << "ms" << std::endl;
    } else if (bpm > 180) {
        auto dur = calculateDuration(bpm);
        std::cout << "Tempo: " << bpm << " bpm\n";
        std::cout << "BPM Classification: high\n";
        std::cout << "Duration of one beat: " << dur << "ms" << std::endl;
        if (bpm > 300) {
            std::cout << "WARNING: Tempo is above 300 bpm!\n";
        }
    }

    return 0;
}