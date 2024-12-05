set -e
g++ "day$1.cpp" -std=c++23 -Wall -Werror -pedantic -g -O3 -o aoc
time ./aoc