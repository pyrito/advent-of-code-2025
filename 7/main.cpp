#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_set>
#include <set>
#include <cstdint>
#include <queue>



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
    std::vector<std::string> grid;
    while(std::getline(file,line)) {
        grid.push_back(line);
    }
    
    using Coord_t = std::pair<int64_t, int64_t>;

    Coord_t start;
    for (uint64_t i = 0; i < grid[0].length(); i++) {
        char c = grid[0][i];
        if (c == 'S') {
            start = { 0, i };
        }
    }

    
    auto checkBounds = [&]( int a, int b ) {
        return ( ( a < grid.size() && a >= 0 ) &&
             ( b < grid[0].length() && b >= 0 ) );
    };

    std::set<Coord_t> visited;
    std::queue<Coord_t> q;
    q.push( start);
    uint64_t totalSplitters = 0;
    while (!q.empty()) {
        auto [x,y] = q.front();
        q.pop();
        // Only go in one direction unless we see a ^
        auto nx = x + 1;
        auto ny = y;

        if ( !checkBounds( nx, ny) ) continue;

        Coord_t newxy { nx, ny };
        // Check for ^
        if ( grid[nx][ny] == '^' ) {
            totalSplitters += 1;

            std::vector<int> dirs = { -1, 1 };
            for ( auto d : dirs ) {
                int nny = y + d;
                if ( checkBounds( nx, nny) ) {
                    Coord_t nnxy { nx, nny };
                    if (!visited.contains(nnxy)) {
                        visited.insert(nnxy);
                        q.push(nnxy);
                    }
                }
            }
        } else {
            if (!visited.contains(newxy)) {
                visited.insert(newxy);
                q.push(newxy);
            }
        }
    }




    std::cout << totalSplitters << std::endl;

    return 0;
}

