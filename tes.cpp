#include <iostream>
using namespace std;

// Fungsi countdown
#include <ctime>
int hitungCountdown(int durasiDetik, time_t waktuMulai) {
    time_t waktuSekarang = time(0);
    int selisih = static_cast<int>(waktuSekarang - waktuMulai);
    int sisa = durasiDetik - selisih;

    // Pastikan tidak negatif
    if (sisa < 0) sisa = 0;

    return sisa;
}

// masukkan fungsi hitungCountdown di sini

int main() {
    int detik = 30; // countdown 30 detik
    time_t mulai = time(0); // simpan waktu mulai

    std::string input;
    std::cout << "Countdown dimulai. Tekan ENTER untuk cek sisa waktu, 'x' untuk keluar.\n";

    while (true) {
        std::getline(std::cin, input);
        if (input == "x") break;

        int sisa = hitungCountdown(detik, mulai);
        std::cout << "Sisa waktu: " << sisa << " detik\n";

        if (sisa == 0) {
            std::cout << "Waktu habis!\n";
            break;
        }
    }

    return 0;
}
