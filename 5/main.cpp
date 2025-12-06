#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_set>

#include <cstdint>

constexpr static int64_t MAX_DIAL = 100;

template <typename T>
void printVector(const std::vector<T>& vec) {
    for (const auto& s : vec) {
        if constexpr( std::is_same_v<T, std::pair<uint64_t,uint64_t>> ) {
            std::cout << s.first << " " << s.second << std::endl;
        } else {
            std::cout << s << std::endl;
        }
    }
}

std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        result.push_back(token);
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

    bool rangeMode = true;
    std::string line;
    std::vector<std::pair<uint64_t, uint64_t>> ranges;
    std::vector<uint64_t> ingredients;
    while (std::getline(file, line)) {
        if (line == "") {
            rangeMode = false;
            continue;
        }

        if (rangeMode) {
            auto range = split(line,'-');
            ranges.emplace_back(std::stoull(range[0]), std::stoull(range[1]));
        } else {
            ingredients.push_back(std::stoull(line));
        }
    }

    printVector(ingredients);
    
    // Sort the ranges
    std::sort(ranges.begin(), ranges.end(), 
                [](const auto& a, const auto& b) {
                    return a.first < b.first;
                }
    );

    printVector(ranges);


    // merge the ranges
    std::vector<std::pair<uint64_t, uint64_t>> mergedRanges;
    for (const auto& r : ranges) {
        // If we are the first or the current start is greater than the last
        if (mergedRanges.empty() || r.first > mergedRanges.back().second ) {
            mergedRanges.push_back(r);
        } else {
            // There is overlap, so extend the range
            auto& cur = mergedRanges.back();
            cur.second = std::max(cur.second, r.second);
        }
    }

    printVector(mergedRanges);

    // inefficient, but good enough
    uint64_t allFresh = 0;
    for (const auto& r : mergedRanges) {
        allFresh += (r.second - r.first + 1);
    }

    std::cout << allFresh << std::endl;
    
    
    

    return 0;
}

