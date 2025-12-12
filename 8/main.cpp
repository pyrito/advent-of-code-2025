#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_set>
#include <set>
#include <cstdint>
#include <queue>
#include <numeric>



struct Junction {
    uint64_t id;
    uint64_t x;
    uint64_t y;
    uint64_t z;
};

struct JunctionEntry {
    uint64_t a;
    uint64_t b;
    double dist;
};

struct UnionFind {
    std::vector<uint64_t> parent;

    UnionFind( uint64_t size ) : parent(size) {
        std::iota( parent.begin(), parent.end(), 0);
    }

    uint64_t find( uint64_t x ) {
        if( parent[x] != x ) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unite( uint64_t x, uint64_t y ) {
        x = find(x);
        y = find(y);
        if (x==y) return;
        parent[y] = x;
    }
};

void printVector(const std::vector<std::vector<std::string>>& vec) {
    // Helper
    for ( const auto& col : vec ) {
        for ( const auto& c : col ) {
            std::cout << c << std::endl;
        }
        std::cout << " " << std::endl;
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

double distance( const Junction& a, const Junction& b ) {
    double dx = double(a.x) - double(b.x);
    double dy = double(a.y) - double(b.y);
    double dz = double(a.z) - double(b.z);
    return std::sqrt(dx*dx + dy*dy + dz*dz);
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

    // Keep track of the coordinates
    std::vector<Junction> junctions;
    uint64_t id = 0;
    while(std::getline(file,line)) {
        auto v = split( line, ',' );
        junctions.emplace_back(Junction{id++, std::stoull(v[0]), std::stoull(v[1]),std::stoull(v[2])});
    }

    auto cmp = []( JunctionEntry& a, JunctionEntry& b ) { return a.dist > b.dist; };
    std::priority_queue<JunctionEntry, std::vector<JunctionEntry>, decltype(cmp)> pq(cmp);
    for ( size_t i = 0; i < junctions.size(); i++ ) { 
        for (size_t j = i + 1; j < junctions.size(); j++) {
            auto dist = distance(junctions[i], junctions[j]);
            pq.push( JunctionEntry{ i,j, dist } );
        }
    }

    std::vector<JunctionEntry> entries;
    for ( size_t i = 0; i < 1000; i++ ) {
        if (pq.empty()) break;
        entries.push_back(pq.top());
        pq.pop();
    }

    std::vector<std::pair<uint64_t, uint64_t>> edges;
    for ( auto e : entries ) {
        std::cout << e.a << " " << e.b << " " <<  e.dist << std::endl;
        edges.push_back(std::make_pair(e.a,e.b));
    }

    UnionFind uf(junctions.size());

    for ( auto e : edges ) {
        uf.unite( e.first, e.second );
    }

    std::vector<uint64_t> components( junctions.size(), 0);
    // Iterate over junctions
    for ( size_t i = 0; i < junctions.size(); i++ ) {
        components[uf.find(i)] += 1;
    }

    std::priority_queue<uint64_t, std::vector<uint64_t>> sz;
    for ( auto c : components ) {
        if ( c > 0 ) {
            sz.push(c);
        }
    }

    uint64_t total = 1;
    for ( int i = 0; i < 3; i++) {
        std::cout << sz.top() << std::endl;
        total *= sz.top();
        sz.pop();
    }

    std::cout << total << std::endl;

    
    
    
    return 0;
}

