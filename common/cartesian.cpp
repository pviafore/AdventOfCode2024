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
    size_t Grid::count(char c) const {
        return std::ranges::count_if(grid, [c](auto p){ return p.second == c; });
    }

    std::optional<Point> Grid::find(char c) const {
        auto iter = std::ranges::find_if(grid, [c](auto p){ return p.second == c; });
        return iter == grid.end() ? std::optional<Point>{} : std::optional<Point>(iter->first); 
    }

    bool Grid::contains(Point p) const { 
        return grid.contains(p);
    }

    char& Grid::operator[](Point p){
        return grid[p];
    }

    size_t Grid::size() const{
        return grid.size();
    }
    
    size_t manhattanDistance(const Point& p1, const Point& p2){
        return abs(p1.x - p2.x) + abs(p1.y - p2.y);
    }

}