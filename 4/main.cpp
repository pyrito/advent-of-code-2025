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
    std::vector<std::string> grid;
    while (std::getline(file, line)) {
        // std::cout << line << "\n";
        grid.push_back(line);
    }

    auto checkBounds = [&]( size_t a, size_t b ) {
        return ( ( a < grid.size() && a >= 0 ) &&
             ( b < grid[0].length() && b >= 0 ) );
    };

    bool done = false;
    uint64_t rolls = 0;
    uint64_t totalRolls = 0;
    std::vector<int> directions = {-1, 0, 1};
    std::vector<std::pair<int, int>> toRemove;
    while (!done) {
        rolls = 0;

        for( int i = 0; i < grid.size(); i++ ) {
            for(int j = 0; j < grid[0].length(); j++ ) {
                // Check to see if we are a roll
                if ( grid[i][j] != '@') {
                    continue;
                }
                // Look through all directions
                uint32_t rollCount = 0;
                for( size_t a = 0; a < directions.size(); a++ ) {
                    for (size_t b = 0; b < directions.size(); b++ ) {
                        int new_i = i+directions[a];
                        int new_j = j+directions[b];
                        if ( checkBounds( new_i, new_j ) && !(new_i == i && new_j == j ) ) {
                            if ( grid[new_i][new_j] == '@' ) {
                                rollCount += 1;
                            }
                        }
                    }
                }

                if ( rollCount < 4 ) {
                    rolls += 1;
                    toRemove.push_back({i,j});
                }
            }
        }

        for ( const auto& r : toRemove ) {
            grid[r.first][r.second] = '.';
        }
        totalRolls += rolls;

        if ( rolls == 0 ) {
            done = true;
        }
    }
    
    
    std::cout << totalRolls << std::endl;

    return 0;
}

