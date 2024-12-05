#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <print>
#include <ranges>
#include <span>
#include <sstream>
#include <vector>

#include "common/input.h"
size_t calculateSimilarityScore(std::span<size_t> first, std::span<size_t>second ){
    auto scores = std::views::transform(first, [&second](auto val) { return val * std::ranges::count(second, val); });
    return std::ranges::fold_left(scores, 0u, std::plus<>{});
}

struct Line{
    size_t first;
    size_t second;

    friend std::istream& operator>>(std::istream& is, Line& line){
        is >> line.first >> line.second;
        return is;
    }
};


int main(){
    std::ifstream in{"input/day1.txt"};
    auto data = input::readInput<Line>(in);
    std::vector<size_t> first = data | std::views::transform([](auto d){  return d.first; }) | std::ranges::to<std::vector<size_t>>(); 
    std::vector<size_t> second = data | std::views::transform([](auto d){  return d.second; }) | std::ranges::to<std::vector<size_t>>(); 
    std::ranges::sort(first);
    std::ranges::sort(second);
    auto calculateDifference = [](auto first, auto second){ return abs(first - second); };
    auto differences = std::views::zip_transform(calculateDifference, first, second) | std::ranges::to<std::vector<size_t>>();
    auto answer1 = std::ranges::fold_left(differences, 0u, std::plus<>{});
    std::println("The difference between lists is {}", answer1);

    auto answer2 = calculateSimilarityScore(first, second);
    
    std::println("The similarity score between lists is {}", answer2);
    return 0;
}