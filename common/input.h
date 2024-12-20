#ifndef AOC2024_INPUT_H_
#define AOC2024_INPUT_H_
#include <algorithm>
#include <ranges>
#include <vector>
namespace input {

    template<typename T=std::string>
    std::vector<T> readInput(std::istream& is){
        std::vector<T> data;
        std::ranges::istream_view<T> iv(is);
        std::ranges::copy(iv, std::back_inserter(data));
        return data;
    }
}

#endif