#include <algorithm>
#include <numeric>
#include <fstream>
#include <print>
#include <set>

#include "common/algo.h"
#include "common/cartesian.h"

bool isAntinodePosition(const cartesian::Point& p, const cartesian::Point& p1, const cartesian::Point &p2){
    auto distanceToFirstAntenna = cartesian::manhattanDistance(p, p1);
    auto distanceToSecondAntenna = cartesian::manhattanDistance(p, p2);
    return (distanceToFirstAntenna == 2*distanceToSecondAntenna || distanceToSecondAntenna == 2*distanceToFirstAntenna);
 
}

size_t getUniqueLocations(const cartesian::Grid& grid, bool useResonance=false){
    auto frequencies = grid.values() |
        std::views::filter([](auto c) { return c != '.'; }) |
        std::ranges::to<std::set<char>>();

    std::set<cartesian::Point> antinodes;
    for(auto frequency: frequencies){
        auto points = grid.findAll(frequency); 
        for(auto [p1, p2]: algo::unique_pairs(points)){
            auto difference = p1 - p2;
            auto gcd = std::gcd(difference.x, difference.y);
            // reduced is the slope 
            cartesian::Point reduced {difference.x/gcd, difference.y/gcd};
            for(auto p = p1; grid.contains(p); p = p + reduced) {
                if(isAntinodePosition(p, p1, p2) || useResonance){
                    antinodes.insert(p);
                }
            }
            for(auto p = p1; grid.contains(p); p = p - reduced) {
                if(isAntinodePosition(p, p1, p2) || useResonance){
                    antinodes.insert(p);
                }
            }

        }
    }
    return antinodes.size();

}

int main(){
    std::ifstream in{"input/day8.txt"};
    cartesian::Grid grid;
    in >> grid;

    std::println("The number of unique locations are {}", getUniqueLocations(grid));
    std::println("The number of unique locations w/ resonance are {}", getUniqueLocations(grid, true));
    return 0;
}
