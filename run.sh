set -e
g++ "day$1.cpp" common/*.cpp -std=c++23 -Wall -Werror -pedantic -g -O0 -o aoc
time ./aoc