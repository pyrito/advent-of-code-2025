#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>

constexpr static int64_t MAX_DIAL = 100;

int64_t mod(int64_t a, int64_t m) {
    int64_t r = a % m;
    return (r < 0) ? r + m : r;
}

uint64_t count(int64_t start, int64_t delta, int64_t n) {
    // If we start at pos, move in the direction of delta
    uint64_t c = 0;
    int64_t pos = start;
    if (delta > 0) {
        for (int i = 0; i < delta; i++) {
            int prev_pos = pos;
            pos = (pos + 1) % n;
            if (prev_pos == 0 && i != 0) c++; // crossed 0
        }
    } else if (delta < 0){
        for (int i = 0; i < -delta; i++) {
            int prev_pos = pos;
            pos = (pos - 1 + n) % n;
            // if we move from 0 to n-1, we crossed 0
            if (prev_pos == 0 && pos == n-1 && i != 0) c++;
        }
    }

    return c;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    const char* filename = argv[1];
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file\n";
        return 1;
    }

    std::string line;
    uint64_t pos = 50;
    uint64_t pass = 0;
    while (std::getline(file, line)) {
        std::cout << line << "\n";
        char direction = line[0];
        auto distance = std::stoull(line.substr(1));
        int64_t apos = pos;
        if (direction == 'L') {
            int64_t wraps = count(apos, -distance, MAX_DIAL);
            apos -= distance;
            std::cout << "wraps: " << wraps << std::endl;
            pass += wraps;
        } else if (direction == 'R') {
            int64_t wraps = count(apos, distance, MAX_DIAL);
            apos += distance;
            std::cout << "wraps: " << wraps << std::endl;
            pass += wraps;
        }

        int64_t m = mod(apos, MAX_DIAL);
        pos = m;
        std::cout << apos << " " << pos << std::endl;
        if (pos == 0) {
            pass += 1;
        }
    }

    std::cout << pass << std::endl;

    return 0;
}

