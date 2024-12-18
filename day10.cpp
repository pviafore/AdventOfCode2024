#include <algorithm>
#include <fstream>
#include <print>
#include <ranges>
#include <set>
#include <vector>

#include "common/cartesian.h"

void insert(std::vector<cartesian::Point>& c, std::ranges::range auto r){
    c.insert(c.end(), r.begin(), r.end());
}

void insert(std::set<cartesian::Point>& c, std::ranges::range auto r){
    c.insert(r.begin(), r.end());
}

template<typename Container=std::set<cartesian::Point>>
size_t getTrailheadScore(const cartesian::Grid& grid, cartesian::Point p){
    std::vector<cartesian::Point> trails = {p};
    for(auto level = '1'; level <= '9'; ++level){
        Container candidates;
        for(auto t: trails){
            auto neighbors = cartesian::getNeighbors(t);
            auto next = std::views::filter(neighbors, [level, &grid](auto n){ return grid.at(n).value_or('.') == level;});
            insert(candidates, next);
        }

        trails.clear();
        trails.insert(trails.end(), candidates.begin(), candidates.end());
    }
    return trails.size();
}

template<typename Container=std::set<cartesian::Point>>
size_t getTotalScore(const cartesian::Grid& grid){
    auto scores = grid.findAll('0') |
        std::views::transform([&grid](auto p) { return getTrailheadScore<Container>(grid, p); });
    
    return std::ranges::fold_left(scores, 0u, std::plus<>{});
}
int main() {

    std::ifstream in{"input/day10.txt"};
    cartesian::Grid grid;
    in >> grid;

    std::println("The total score is {}", getTotalScore(grid));
    std::println("The total rating is {}", getTotalScore<std::vector<cartesian::Point>>(grid));

    return 0;
}