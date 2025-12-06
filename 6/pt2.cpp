#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_set>

#include <cstdint>


void printVector(const std::vector<std::vector<uint64_t>>& vec) {
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

    // Treat the problems as a grid
    std::vector<std::string> grid;
    while(std::getline(file,line)) {
        grid.push_back(line);
    }

    size_t numCols = 0;
    for (size_t i = 0; i < grid.size() - 1; i++ ) {
        numCols = std::max(numCols, grid[i].size());
    }

    // Iterate over the grid col wise
    std::vector<std::vector<uint64_t>> newVals;
    std::vector<uint64_t> curVals;
    for (size_t j = 0; j < numCols; j++) {
        std::string s;
        for (size_t i = 0; i < grid.size() - 1; i++) {
            auto cur = grid[i][j];
            std::cout << "cur: " << cur << std::endl;
            if ( cur != ' ' ) {
                s += grid[i][j];
            }
        }

        
        // Empty col
        if (s == "") {
            newVals.push_back(curVals);
            curVals.clear();
        } else {
            curVals.push_back(std::stoull(s));
        }
    }
    newVals.push_back(curVals);

    printVector(newVals);

    std::vector<std::string> opsVec;
    std::string ops = grid.back();
    std::string token;
    std::istringstream s(ops);
    while(s >> token) {
        opsVec.push_back(token);
    }

    std::cout << opsVec.size() << std::endl;
    std::cout << newVals.size() << std::endl;

    uint64_t total = 0;
    for (size_t i = 0; i < newVals.size(); i++) {
        const auto& op = opsVec[i];
        uint64_t localVal = (op == "*") ? 1 : 0;
        for (const auto v : newVals[i]) {
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

