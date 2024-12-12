#include <algorithm>
#include <cassert>
#include <fstream>
#include <print>
#include <set>

#include "common/input.h"


using Update = std::vector<std::string>;
using OrderingRules = std::set<std::pair<std::string, std::string>>;
bool isPrecedenceSatisfied(const OrderingRules& rules, const std::string& s1, const std::string& s2){
    // if a path exists from s2 to s1, s2 precedes s1 and that's bad
    return !rules.contains({s2, s1});
}
bool isCorrectlyOrdered(const Update& update, const OrderingRules& rules) {
    for(auto iter = update.begin(); iter != update.end(); ++iter){
        for(auto iter2 = iter+1; iter2 != update.end(); ++iter2){
            if(!isPrecedenceSatisfied(rules, *iter, *iter2)){
                return false;
            }
        }
    }
    return true;
}
Update correctUpdate(const Update& update, const OrderingRules& rules) {
    Update out;
    for(auto text: update) {
        auto iter = out.begin();
        for(; iter < out.end(); ++iter){
            if(!isPrecedenceSatisfied(rules, *iter, text)){
                break; 
            }
        }
        out.insert(iter, text);
    }
    assert(isCorrectlyOrdered(out, rules) && out.size() == update.size());
    return out;
}
struct PuzzleInput {
    std::set<std::pair<std::string, std::string>> orderingRules;
    std::vector<Update> updates;

    friend std::istream& operator>>(std::istream& in, PuzzleInput& p){
        auto strings = input::readInput(in);
        for(auto s: strings){
            if(s.contains('|')){
                auto pipe = std::ranges::find(s, '|');
                p.orderingRules.insert(std::pair(std::string(s.begin(), pipe), std::string(pipe+1, s.end())));
            }
            else {
                p.updates.emplace_back(std::views::split(s, ',') | std::ranges::to<Update>());
            }
        }
        return in;
    }

    std::vector<Update> getCorrectlyOrderedUpdates() const {
        return std::views::filter(updates, [this](auto u){ return isCorrectlyOrdered(u, this->orderingRules); }) |
            std::ranges::to<std::vector<Update>>();
    }
    
    std::vector<Update> reorderUpdatesToBeCorrect() const {
        return std::views::filter(updates, [this](auto u) { return !isCorrectlyOrdered(u, this->orderingRules); }) |
               std::views::transform([this](auto u){ return correctUpdate(u, this->orderingRules); }) |
               std::ranges::to<std::vector<Update>>();
    }
};

size_t getMiddleUpdate(const Update& update) {
    auto s = update[update.size() / 2];
    return std::stoi(s);
}

int main() {
    std::ifstream in{"input/day5.txt"};
    PuzzleInput puzzleInput;
    in >> puzzleInput;

    auto correctlyOrderedUpdates = puzzleInput.getCorrectlyOrderedUpdates();
    auto midpoints = std::views::transform(correctlyOrderedUpdates, getMiddleUpdate);
    auto answer = std::ranges::fold_left(midpoints, 0u, std::plus<>{});
    std::println("Midpoints of answer all added up are {} ", answer);

    auto orderedUpdates = puzzleInput.reorderUpdatesToBeCorrect();
    midpoints = std::views::transform(orderedUpdates, getMiddleUpdate);
    answer = std::ranges::fold_left(midpoints, 0u, std::plus<>{});
    std::println("Midpoints of answer all added up are {} ", answer);

}