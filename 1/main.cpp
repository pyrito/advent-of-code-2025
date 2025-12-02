#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_set>

#include <cstdint>

constexpr static int64_t MAX_DIAL = 100;


std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        result.push_back(token);
    }
    return result;
}

std::vector<std::string> splitIntoChunks(const std::string& s, size_t numChunks) {
    std::vector<std::string> result;
    size_t chunkSize = s.length() / numChunks;
    for (size_t i = 0; i < s.length(); i += chunkSize) {
        result.push_back(s.substr(i, chunkSize));
    }
    return result;
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
    std::vector<std::string> ranges;
    while (std::getline(file, line)) {
        std::cout << line << "\n";
        ranges = split(line, ',');
    }

    uint64_t sum = 0;
    for (const auto& range : ranges) {
        auto r = split(range, '-');
        std::string start = r[0];
        std::string end = r[1];

        auto starti = std::stoull(start);
        auto endi = std::stoull(end);
        std::unordered_set<uint64_t> seen;
        for ( ; starti <= endi; starti++ ) {
            std::string s = std::to_string(starti);
            for (size_t i = 2; i <= s.length(); i++) {
                if (s.length() % i != 0 || seen.contains(starti)) {
                    continue;
                }
                
                // valid chunk size
                bool equal = true;
                auto chunks = splitIntoChunks(s, i);
                for (const auto& chunk : chunks) {
                    // std::cout << "chunk: " << chunk << " chunks[0]" << chunks[0] << std::endl;
                    if (chunk != chunks[0]) {
                        equal = false;
                        break;
                    }
                }

                if (equal) {
                    std::cout << starti << std::endl;
                    sum += starti;
                    seen.insert(starti);
                }
            }
        }

        // std::cout << start << " " << end << std::endl;
    }
    
    std::cout << sum << std::endl;

    return 0;
}

