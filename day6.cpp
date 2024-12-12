#include <fstream>
#include <print>
#include <set>
#include <utility>

#include "common/cartesian.h"

cartesian::Point getNextPoint(cartesian::Point p, char guard) {
    cartesian::Point offset{0, -1};
    if(guard == '<'){
        offset = {-1, 0};
    }
    else if(guard == '>'){
        offset = {1, 0};
    }
    else if(guard == 'v'){
        offset = {0,1};
    }
    return p + offset;
}

char turnRight(char guard){
    switch (guard) {
        case '^': return '>';
        case 'v': return '<';
        case '<': return '^';
        case '>': return 'v';
    }
    std::unreachable();
}

std::set<cartesian::Point> visited;
cartesian::Grid walkTheGrid(const cartesian::Grid& original) {
    visited = {};
    auto grid = original; 
    auto point = grid.find('^').value();
    char guard = '^';
    std::set<std::pair<cartesian::Point, char>> seen {{point, guard}};
    while(grid.contains(point)){
        visited.insert(point);
        auto nextPoint = getNextPoint(point, guard);
        if(grid.at(nextPoint).value_or(' ') == '#'){
            guard = turnRight(guard);
            grid[point] = guard;
        }
        else{
            grid[point] = 'X';
            if(grid.contains(nextPoint)){

                grid[nextPoint] = guard;
            }
            point = nextPoint;
        }
        if(seen.contains({point, guard})){
            break;
        }
        seen.emplace(point, guard);
    }

    return grid;
}

bool doesGridContainGuard(const cartesian::Grid& grid){
    return grid.find('^').has_value() || grid.find('v').has_value() || grid.find('>').has_value() || grid.find('<').has_value();
}


cartesian::Grid addObstacle(const cartesian::Grid& grid, cartesian::Point p) {
    auto copy = grid;
    copy[p] = '#';
    return copy;
}

size_t getAllGridsWithLoop(const cartesian::Grid& grid) {
    auto savedVisited = visited;
    auto grids =  savedVisited | 
        std::views::filter([grid](auto p){return grid.at(p).value() != '#' && grid.at(p).value() != '^';}) |
        std::views::transform([grid](auto p){auto g = addObstacle(grid, p); auto newGrid = walkTheGrid(g); return newGrid;}) |
        std::views::transform([](auto g){return doesGridContainGuard(g);}) |
        std::ranges::to<std::vector<bool>>();
    return std::ranges::count(grids, true);
}

int main(){
    std::ifstream in{"input/day6.txt"};
    cartesian::Grid grid;
    in >> grid;

    auto walkedGrid = walkTheGrid(grid);
    std::println("The guard walked {}", walkedGrid.count('X'));



    auto numberOfGridsWithLoop = getAllGridsWithLoop(grid);
    std::println("{} grids could induce a loop with an obstacle", numberOfGridsWithLoop);
}