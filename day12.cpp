#include <algorithm>
#include <cassert>
#include <fstream>
#include <print>
#include <set>

#include "common/cartesian.h"

struct Region {
    char symbol;
    std::set<cartesian::Point> points;

    size_t getPerimeter(const cartesian::Grid& grid) {
        
        auto numberOfBorders = std::views::transform(points, [this, &grid](auto p){ 
            auto neighbors = cartesian::getNeighbors(p); 
            return std::ranges::count_if(neighbors, [this, &grid](auto n){return grid.at(n).value_or(' ') != this->symbol; });
        });
        return std::ranges::fold_left(numberOfBorders, 0ull, std::plus<>{});
    }

    size_t getNumberOfSides(const cartesian::Grid& grid){
        auto outerPoints = std::views::filter(points, [this, &grid](auto p){ 
            auto neighbors = cartesian::getNeighbors(p); 
            return std::ranges::any_of(neighbors, [this, &grid](auto n){return grid.at(n).value_or(' ') != this->symbol; });
        });
        auto pointsWithSides = std::views::cartesian_product(outerPoints, std::vector{cartesian::left, cartesian::right, cartesian::above, cartesian::below}) | 
            std::ranges::to<std::vector<std::pair<cartesian::Point, cartesian::Point>>>();
        size_t total = 0;
        while(pointsWithSides.begin() != pointsWithSides.end()){
            auto [point, side] = *pointsWithSides.begin();
            std::vector<std::pair<cartesian::Point, cartesian::Point>> pointsToErase { {point, side} };
            if(grid.at(point).value() != grid.at(point+side).value_or(' ')){
                total += 1;
                if(side == cartesian::left || side == cartesian::right){
                    // move up
                    auto nextPoint = point+cartesian::above;
                    while(std::ranges::find(pointsWithSides, std::pair{nextPoint, side}) != pointsWithSides.end() && grid.at(nextPoint).value_or(' ') == grid.at(point).value() && grid.at(nextPoint+side).value_or(' ') != grid.at(point).value()){
                        pointsToErase.emplace_back(nextPoint, side);
                        nextPoint = nextPoint + cartesian::above;
                    }
                    //move down
                    nextPoint = point + cartesian::below;
                    while(std::ranges::find(pointsWithSides, std::pair{nextPoint, side}) != pointsWithSides.end() && grid.at(nextPoint).value_or(' ') == grid.at(point).value() && grid.at(nextPoint + side).value_or(' ') != grid.at(point).value()){
                        pointsToErase.emplace_back(nextPoint, side);
                        nextPoint = nextPoint + cartesian::below;
                    }
                }
                if(side == cartesian::above || side == cartesian::below){
                    //move left
                    auto nextPoint = point + cartesian::left;
                    while(std::ranges::find(pointsWithSides, std::pair{nextPoint, side}) != pointsWithSides.end() && grid.at(nextPoint).value_or(' ') == grid.at(point).value() && grid.at(nextPoint + side).value_or(' ') != grid.at(point).value()){
                        pointsToErase.emplace_back(nextPoint, side);
                        nextPoint = nextPoint + cartesian::left;
                    }
                    //move right
                    nextPoint = point + cartesian::right;
                    while(std::ranges::find(pointsWithSides, std::pair{nextPoint, side}) != pointsWithSides.end() && grid.at(nextPoint).value_or(' ') == grid.at(point).value() && grid.at(nextPoint + side).value_or(' ') != grid.at(point).value()){
                        pointsToErase.emplace_back(nextPoint, side);
                        nextPoint = nextPoint + cartesian::right;
                    }
                }
            }
            for(auto pToDelete: pointsToErase){
                auto match = std::ranges::find(pointsWithSides, pToDelete);
                assert(match != pointsWithSides.end());
                pointsWithSides.erase(match);
            }
        }
        return total;
    }
    size_t getPricing(const cartesian::Grid& grid) {
        return getPerimeter(grid) * points.size();
    }
    
    size_t getBulkPricing(const cartesian::Grid& grid) {
        return getNumberOfSides(grid) * points.size();
    }

    bool isAdjacentTo(const cartesian::Point& p) const{
        auto neighbors = cartesian::getNeighbors(p);
        return std::ranges::find_first_of(points, neighbors) != points.end();
    }

    friend auto operator<=>(const Region& lhs, const Region &rhs) noexcept = default;


};

auto getRegions(cartesian::Grid& grid){
    std::vector<Region> regions;
    for(auto [point, c]: grid){
        auto belongsToRegion = [point, c](const auto& r){return r.symbol == c && r.isAdjacentTo(point);};
        auto adjacentRegions = std::views::filter(regions, belongsToRegion);
        if(adjacentRegions.begin() != adjacentRegions.end()) {
            std::vector<std::vector<Region>::iterator> toBeDeleted;
            auto firstRegion = adjacentRegions.begin();
            for(auto adjacentRegion = std::next(adjacentRegions.begin()); adjacentRegion != adjacentRegions.end(); ++adjacentRegion){
                firstRegion->points.insert(adjacentRegion->points.begin(), adjacentRegion->points.end());
                toBeDeleted.push_back(adjacentRegion.base());
            }
            firstRegion->points.insert(point);
            for(const auto& r: toBeDeleted){
                regions.erase(r);
            }
        }
        else {
            regions.emplace_back(c, std::set{point});
        }
    }

    return regions;
}

int main(){
    std::ifstream in{"input/day12.txt"};
    cartesian::Grid map;
    in >> map;

    auto regions = getRegions(map);

    auto totalPrices = std::views::transform(regions, [&map](auto r){ return r.getPricing(map); });
    std::println("The total price is {}", std::ranges::fold_left(totalPrices, 0ull, std::plus<>{}));

    auto totalBulkPrices = std::views::transform(regions, [&map](auto r){ return r.getBulkPricing(map); });
    std::println("The total bulk price is {}", std::ranges::fold_left(totalBulkPrices, 0ull, std::plus<>{}));


}