#include <cassert>
#include <fstream>
#include <limits>
#include <print>
#include <set>
#include <queue>

#include "common/input.h"


struct Machine {
    size_t aMoveX;
    size_t aMoveY;
    size_t bMoveX;
    size_t bMoveY;
    size_t prizeX;
    size_t prizeY;


    friend std::istream& operator>>(std::istream& in, Machine& machine){
        std::string text;
        getline(in, text);
        if(text.empty()){
            return in;
        }
        auto plus = text.find('+');
        auto comma = text.find(',', plus);
        machine.aMoveX = std::stoull(text.substr(plus+1, comma-plus-1));
        plus = text.find('+', comma);
        machine.aMoveY = std::stoull(text.substr(plus+1));

        getline(in, text);
        plus = text.find('+');
        comma = text.find(',', plus);
        machine.bMoveX = std::stoull(text.substr(plus+1, comma-plus));
        plus = text.find('+', comma);
        machine.bMoveY = std::stoull(text.substr(plus+1));

        getline(in, text);

        auto equal = text.find('=');
        comma = text.find(',', equal);
        machine.prizeX = std::stoull(text.substr(equal+1, comma-equal-1));

        equal = text.find('=', comma);
        machine.prizeY = std::stoull(text.substr(equal+1));
        getline(in, text);

        return in;
    }

    struct Candidate {
        size_t tokens = 0;
        size_t pressesA;
        size_t pressesB;
        size_t x;
        size_t y;

        friend auto operator<=>(const Candidate& c, const Candidate& c2) = default;
    };

    size_t getCheapestTokensToWin_LS(size_t offset=0) const {
        // use linear systems to solve this

        int64_t aCoefficientFromYEquation = -aMoveY * bMoveX; 
        int64_t aNumerator =(static_cast<int64_t>((offset+prizeX) * bMoveY) - static_cast<int64_t>((offset+prizeY) * bMoveX)); 
        int64_t aDenominator = static_cast<int64_t>(aCoefficientFromYEquation + aMoveX*bMoveY);
        if(aNumerator % aDenominator != 0){
            return 0;
        }
        int64_t aPresses = aNumerator / aDenominator;
        int64_t bNumerator = (offset + prizeY) - (aPresses * aMoveY); 
        if(bNumerator % bMoveY != 0){
            return 0;
        }
        int64_t bPresses = bNumerator/ bMoveY;
        return (aPresses > 0 && bPresses > 0) ? 3*aPresses + bPresses : 0;
    }

    size_t getCheapestTokensToWin() const {
        static size_t i = 0;
        std::println("Hitting machine {}\n", ++i);
        std::priority_queue<Candidate> candidates;
        candidates.emplace(0,0,0,0);
        size_t lowest = 0;
        std::set<std::pair<size_t, size_t>> seen;
        while(!candidates.empty()){
            auto [tokens, pressesA, pressesB, x, y] = candidates.top();
            candidates.pop();
            if(pressesA > 100 || pressesB > 100 || seen.contains({x,y})) { 
                continue;
            }

            if(lowest != 0 && tokens > lowest){
                // we are getting things that have more tokens than our best answer
                break;
            }
            if(x == prizeX && y == prizeY){
                // we found a candidate
                if(lowest == 0){
                    lowest = tokens;
                }
                lowest = std::min(lowest, tokens);
                continue;
            }
            if(x > prizeX || y > prizeY){
                // we are past our point 
                continue;
            }
            seen.emplace(x,y);

            candidates.emplace(tokens+3, pressesA+1, pressesB, x+aMoveX, y+aMoveY);
            candidates.emplace(tokens+1, pressesA, pressesB+1, x+bMoveX, y+bMoveY);
        }
        if(lowest != 0){
            assert(lowest == getCheapestTokensToWin_LS());
        }
        return lowest;
    }
};


int main() {
    std::ifstream in{"input/day13.txt"};
    auto machines = input::readInput<Machine>(in);

    auto tokens = std::views::transform(machines, [](auto m){ return m.getCheapestTokensToWin(); });
    std::println("The fewest tokens sum is {}", std::ranges::fold_left(tokens, 0ull, std::plus<>{}));
    
    auto tokensWithOffset = std::views::transform(machines, [](auto m){ return m.getCheapestTokensToWin_LS(10'000'000'000'000ULL); });
    std::println("The fewest tokens sum w/ offset is {}", std::ranges::fold_left(tokensWithOffset, 0ull, std::plus<>{}));
    return 0;
}