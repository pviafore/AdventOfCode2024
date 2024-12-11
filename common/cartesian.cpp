#include <algorithm>

#include "cartesian.h"
namespace cartesian {

    std::optional<char> Grid::at(Point p) const{
        if(!grid.contains(p)) return {};

        return grid.at(p);
    }

    std::map<Point, char>::const_iterator Grid::begin() const {
        return grid.begin();
    }
    
    std::map<Point, char>::const_iterator Grid::end() const {
        return grid.end();
    }

    std::vector<Point> getDirectionOffsets(){
        return std::views::cartesian_product(std::vector{-1, 0, 1}, std::vector{-1,0,1}) | 
            std::views::transform([](const auto& t){ return Point{std::get<0>(t), std::get<1>(t)};}) | 
            std::ranges::to<std::vector<Point>>();
    }
}