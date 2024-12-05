#include <algorithm>
#include <fstream>
#include <format>
#include <iterator>
#include <numeric>
#include <ranges>
#include <print>
#include <sstream>


#include "common/input.h"
struct Report {
    std::vector<size_t> levels;

    friend std::istream& operator>>(std::istream& is, Report& r){
        std::string line;
        std::getline(is, line);
        std::stringstream ss{line};

        r.levels = input::readInput<size_t>(ss);
        return is;
    }

    bool isSafe() const {
        std::vector<int32_t> differences;
        std::adjacent_difference(levels.begin(), levels.end(), std::back_inserter(differences));
        differences.erase(differences.begin());
        auto result = std::ranges::all_of(differences, [](auto val){return val >= 1 && val <= 3;}) || 
                      std::ranges::all_of(differences, [](auto val){ return val >= -3 && val <= -1;});
        return result;
    }

    bool isSafeAfterDampening() const {
        if(isSafe()){
            return true;
        }
        for(size_t index = 0; index < levels.size(); ++index){
            Report newReport = *this;
            newReport.levels.erase(newReport.levels.begin()+index);
            if(newReport.isSafe()){
                return true;
            }
        }
        return false;
    }
};

int main(){
    std::ifstream in{"input/day2.txt"};
    auto reports = input::readInput<Report>(in);
    auto answer = std::ranges::count_if(reports, [](const auto& report){ return report.isSafe(); });
    std::println("The number of safe reports are {}", answer);
    auto answer2 = std::ranges::count_if(reports, [](const auto& report){ return report.isSafeAfterDampening(); });
    std::println("The number of safe reports after dampening are {}", answer2);
    return 0;
}