#include <cassert>
#include <cstdio>
#include <fstream>
#include <print>
#include <string_view>
#include <vector>

#include "common/input.h"


class StringParser {
public:
    StringParser(std::string_view sv) : sv(sv), it(sv.begin()) {
    }

    void readUntilLiteral(std::string literal){
        assert(it <= sv.end());
        if(static_cast<size_t>(std::distance(it, sv.end())) <= literal.size()){
            it = sv.end();
            return;
        }
        while(it!=sv.end()){
            if(isLiteralNextInText(literal)){
                break;
            }
            ++it;
        }
        if(it == sv.end()){
            return;
        }
        it+=literal.size();   
    }

    std::optional<size_t> readNumber(){
        size_t number = 0;
        auto next = peek();
        if(isdigit(next)){
            number += next - '0';
            ++it;
        }
        else {
            return {};
        }
        next = peek();
        if(isdigit(next)){
            number *= 10;
            number += next - '0';
            ++it;
        }
        else {
            return number;
        }
        next = peek();
        if(isdigit(next)){
            number *= 10;
            number += next - '0';
            ++it;
        }
        return number;
    }

    bool hasRemainingCharacters() const {
        return it != sv.end();
    }

    char peek() const{
        if(it == sv.end()){
            return '\0';
        }
        return *it;
    }

    char readNextCharacter() {
        assert(it < sv.end());
        return *it++;
    }

    bool isLiteralNextInText(std::string_view text){
        if(it < sv.end() && static_cast<size_t>(std::distance(it, sv.end())) < text.length()){
            return false;
        }
        auto s = std::string(it, it+text.length());
        if(s == text){
            it += text.length();
            return true;
        }

        return false;
    }

private: 
    std::string_view sv;
    std::string_view::iterator it;
};




size_t getAnswer(std::string_view sv, bool shouldUseDosAndDonts = false){
    StringParser parser(sv);
    bool isEnabled = true;
    size_t answer = 0;
    while(parser.hasRemainingCharacters()){
        if(parser.isLiteralNextInText("don't()")){
            isEnabled = false;
        }
        else if(parser.isLiteralNextInText("do()")){
            isEnabled = true;
        }
        else if(parser.isLiteralNextInText("mul(")){

            auto num1 = parser.readNumber();
            if(!num1 || !parser.hasRemainingCharacters() || parser.peek() != ','){
                continue;
            }
            parser.readNextCharacter();

            auto num2 = parser.readNumber();
            if(!num2 || !parser.hasRemainingCharacters() || parser.peek() != ')'){
                continue;
            }
            parser.readNextCharacter();
            if(!shouldUseDosAndDonts || isEnabled){
                answer += num1.value() * num2.value();
            }
        }
        else {
            parser.readNextCharacter();
        }
    }
    return answer;
}

int main() {
    std::ifstream in{"input/day3.txt"};
    std::string input;
    getline(in, input);

    auto answer = getAnswer(input); 
    std::println("The sum of all multiplications are {}", answer);
    auto answer2 = getAnswer(input, true); 
    std::println("The sum of all multiplications w/ do's and don'ts are {}", answer2);
}