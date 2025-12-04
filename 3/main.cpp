#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_set>

#include <cstdint>

constexpr static int64_t MAX_DIAL = 100;


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
    std::vector<std::string> banks;
    while (std::getline(file, line)) {
        // std::cout << line << "\n";
        banks.push_back(line);
    }

    uint64_t sum = 0;
    for ( const auto& bank : banks ) {
        std::string res;
        uint64_t pos = 0;
        for ( int64_t c = 11; c >= 0; c-- ) {
            uint64_t max = 0;
            for ( size_t i = pos; i < bank.size()-c; i++ ) {
                uint64_t val = bank[i] - '0';
                if (val > max) {
                    max = val;
                    pos = i+1;
                }
            }
            res += std::to_string(max);
        }

        std::cout << res << std::endl;

        sum += std::stoull(res);
    }
    
    std::cout << sum << std::endl;

    return 0;
}

