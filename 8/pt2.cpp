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

uint64_t dfs( const std::vector<std::string>& grid, int x, int y,
         std::vector<std::vector<uint64_t>>& cache ) {
    auto checkBounds = [&]( int a, int b ) {
        return ( ( a < grid.size() && a >= 0 ) &&
             ( b < grid[0].length() && b >= 0 ) );
    };

    if ( !checkBounds( x, y ) ) {
        return 0;
    }

    // Reached bottom
    if ( x == grid.size() - 1 ) {
        return 1;
    }

    // Check cache
    if ( cache[x][y] != -1 ) { 
        return cache[x][y];
    }

    uint64_t result = 0;
    if ( grid[x][y] == '^' ) {
        uint64_t left = dfs(grid, x, y-1, cache);
        uint64_t right = dfs(grid, x, y+1, cache);

        result = left + right;
    } else {
        result = dfs(grid, x+1,y, cache);
    }

    cache[x][y] = result;
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

    std::cout << start.first << std::endl;
    std::cout << start.second << std::endl;

    // Set the cache
    std::vector<std::vector<uint64_t>> cache(grid.size(), std::vector<uint64_t>(grid[0].size(), -1));
    uint64_t total = dfs(grid, start.first, start.second, cache);

    std::cout << total << std::endl;

    return 0;
}

