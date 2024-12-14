#include <algorithm>
#include <fstream>
#include <print>
#include <ranges>
#include <sstream>
#include <span>

#include "common/input.h"

struct Equation {
    size_t target;
    std::vector<size_t> numbers;

    friend std::istream& operator>>(std::istream& in, Equation& eq){
        eq.numbers = {};
        std::string line;
        getline(in, line);
        if(line == ""){
            return in;
        }
        std::stringstream ss{line};
        std::string text;
        ss >> text;

        text.pop_back(); // remove the colon 
        eq.target = std::stoull(text);
        std::ranges::copy(std::views::istream<size_t>(ss), std::back_inserter(eq.numbers));
        return in;
    }
};

size_t concat(size_t a, size_t num){
    return std::stoull(std::to_string(a) + std::to_string(num));
}

bool isEquationTrue(const Equation& equation, bool shouldConcat = false){
    std::vector<size_t> answer = {*equation.numbers.begin()};
    for(auto num : std::views::drop(equation.numbers, 1)){
        std::vector<size_t> candidates;
        for(auto a: answer){
            if(a+num <= equation.target){
                candidates.push_back(a+num);
            }
            if(a*num <= equation.target){
                candidates.emplace_back(a*num);    
            }
            if(shouldConcat && concat(a, num) <= equation.target){
                candidates.emplace_back(concat(a, num));
            }
        }

        std::swap(candidates, answer);
    }
    return std::ranges::any_of(answer, [target=equation.target](auto a){ return a == target; });
}


size_t getTotalTestValue(std::span<Equation> equations, bool withConcat=false){
    auto eqs = std::views::filter(equations, [withConcat](auto e){ return isEquationTrue(e, withConcat);}) | std::views::transform([](auto e){return e.target; });
    return std::ranges::fold_left(eqs, 0u, std::plus<>{});
}

int main(){
    std::ifstream in{"input/day7.txt"};
    auto equations = input::readInput<Equation>(in);
    std::println("The total test value is {}", getTotalTestValue(equations));
    std::println("The total test value with concat is {}", getTotalTestValue(equations, true));

    return 0; 
}