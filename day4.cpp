#include <algorithm>
#include <fstream>
#include <print>

#include "common/cartesian.h"
#include "common/input.h"

bool doesSpellXmas(const cartesian::Grid& grid, cartesian::Point p, cartesian::Point direction){
    return grid.at(p+direction).value_or(' ') == 'M' && 
           grid.at(p+direction+direction).value_or(' ') == 'A' &&
           grid.at(p+direction+direction+direction).value_or(' ') == 'S';
}

bool doesSpellX_Mas(const cartesian::Grid& grid, cartesian::Point p){
    if(grid.at(p + cartesian::Point{-1,-1}).value_or(' ') == 'M'){
        return (grid.at(p + cartesian::Point{-1, 1}).value_or(' ') == 'M' && grid.at(p+cartesian::Point(1, 1)).value_or(' ') == 'S' && grid.at(p + cartesian::Point(1, -1)).value_or(' ') == 'S') ||
               (grid.at(p + cartesian::Point{1, -1}).value_or(' ') == 'M' && grid.at(p+cartesian::Point(1, 1)).value_or(' ') == 'S' && grid.at(p + cartesian::Point(-1, 1)).value_or(' ') == 'S');
    }
    else if(grid.at(p + cartesian::Point{1,1}).value_or(' ') == 'M'){
        return (grid.at(p + cartesian::Point{-1, 1}).value_or(' ') == 'M' && grid.at(p+cartesian::Point(-1, -1)).value_or(' ') == 'S' && grid.at(p + cartesian::Point(1, -1)).value_or(' ') == 'S') ||
               (grid.at(p + cartesian::Point{1, -1}).value_or(' ') == 'M' && grid.at(p+cartesian::Point(-1, -1)).value_or(' ') == 'S' && grid.at(p + cartesian::Point(-1, 1)).value_or(' ') == 'S');
    }
    return false;
}

size_t getNumberOfXmasStrings(const cartesian::Grid& grid){
    auto xCharacters = std::views::filter(grid, [](std::pair<cartesian::Point, char> p){ return p.second == 'X';});
    auto directions = cartesian::getDirectionOffsets();
    return std::ranges::count_if(std::views::cartesian_product(xCharacters, directions), [&grid](const auto& t){ return doesSpellXmas(grid, std::get<0>(t).first, std::get<1>(t));});
}

size_t getNumberOfX_MasStrings(const cartesian::Grid& grid){
    auto aCharacters = std::views::filter(grid, [](std::pair<cartesian::Point, char> p){ return p.second == 'A';});
    auto directions = cartesian::getDirectionOffsets();
    return std::ranges::count_if(aCharacters, [&grid](const auto& p){ return doesSpellX_Mas(grid, p.first);});
}

int main() {
    std::ifstream in("input/day4.txt");
    cartesian::Grid grid;
    in >> grid;

    std::println("The number of XMAS strings is {}", getNumberOfXmasStrings(grid));
    std::println("The number of X-MAS strings is {}", getNumberOfX_MasStrings(grid));
}