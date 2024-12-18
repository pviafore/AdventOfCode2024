#include <algorithm>
#include <fstream>
#include <print>
#include <ranges>
#include <unordered_map>

#include "common/input.h"

std::vector<size_t> transformStone(size_t stone){
    if(stone == 0) {
        return {1};
    }
    std::string stoneText = std::to_string(stone);
    if(stoneText.size() % 2 == 0){
        auto midpoint = stoneText.size() / 2;
        return {std::stoull(std::string(stoneText.begin(), stoneText.begin()+midpoint)), std::stoull(std::string(stoneText.begin()+midpoint, stoneText.end()))};
    }
    return {stone*2024};
}
class Stones {
public:
    friend std::istream& operator>>(std::istream& in, Stones& stones){
        stones.stoneCount.clear();
        auto stoneNumbers = input::readInput<size_t>(in) | std::views::transform([](auto s){ return std::pair{s, 1ull}; });
        stones.stoneCount.insert(stoneNumbers.begin(), stoneNumbers.end()); 
        return in;
    }

    void blink(size_t numberOfTimes){
        for(size_t i = 0ull; i < numberOfTimes; ++i){
            std::unordered_map<size_t, size_t> nextRound;
            for(auto p: stoneCount){
                for(auto transformed: transformStone(p.first)){
                    if(!nextRound.contains(transformed)){
                        nextRound[transformed] = 0;
                    }
                    nextRound[transformed] += p.second;
                }
            }

            std::swap(nextRound, stoneCount);
        }
    }

    size_t getTotalStones() const {
        return std::ranges::fold_left(std::views::values(stoneCount), 0ull, std::plus<>{});
    }
private:
    std::unordered_map<size_t, size_t> stoneCount;
};


int main(){
    Stones stones;
    std::ifstream in{"input/day11.txt"};
    in >> stones;

    stones.blink(25);
    std::println("The total number of stones are: {}", stones.getTotalStones());
    
    stones.blink(50);
    std::println("The total number of stones are: {}", stones.getTotalStones());

    return 0;
}