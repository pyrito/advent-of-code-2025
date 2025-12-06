#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_set>

#include <cstdint>


void printVector(const std::vector<std::vector<std::string>>& vec) {
    // Helper
    for ( const auto& col : vec ) {
        for ( const auto& c : col ) {
            std::cout << c << std::endl;
        }
        std::cout << " " << std::endl;
    }
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
    // Read first line to infer cols
    (void)std::getline(file, line);

    
    std::vector<std::string> firstRow;
    std::string token;

    std::istringstream firstStream(line);
    while(firstStream >> token) {
        firstRow.push_back(token);
    }

    size_t numCols = firstRow.size();
    std::vector<std::vector<std::string>> cols(numCols);
    for (size_t i = 0; i < numCols; i++) {
        cols[i].push_back(firstRow[i]);
    }

    while(std::getline(file,line)) {
        std::istringstream s(line);
        for (size_t i = 0; i < numCols; i++) {
            if (s >> token) {
                cols[i].push_back(token);
            }
        }
    }

    printVector(cols);

    uint64_t total = 0;
    for ( const auto& col : cols ) {
        const auto& op = col.back();
        uint64_t localVal = (op == "*") ? 1 : 0;
        for (size_t i = 0; i < col.size() - 1; i++) {
            auto v = std::stoull(col[i]);
            if (op == "+") {
                localVal += v;
            } else if (op == "*") {
                localVal *= v;
            }
        }
        total += localVal;
    }

    std::cout << total << std::endl;

    return 0;
}

