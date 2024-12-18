#include <compare>
#include <istream>
#include <map>
#include <optional>
#include <vector>

#include "input.h"
namespace cartesian {

    struct Point;
    size_t manhattanDistance(const Point& p1, const Point& p2);

    struct Point {
        long x = 0;
        long y = 0;
        

        friend auto operator<=>(const Point& lhs, const Point& rhs) = default;
        friend Point operator+(const Point& lhs, const Point& rhs) {
            return Point{lhs.x + rhs.x, lhs.y + rhs.y};
        }
        friend Point operator-(const Point& lhs, const Point& rhs) {
            return Point{lhs.x - rhs.x, lhs.y - rhs.y};
        }

        friend auto operator<=>(const Point& lhs, const Point& rhs) noexcept = default;
    };
    

    size_t manhattanDistance(const Point& p1, const Point& p2);
    
    std::vector<Point> getDirectionOffsets();

    std::vector<Point> getNeighbors(const Point& p);

    const Point left{ -1, 0};
    const Point right{ 1, 0};
    const Point above{ 0, -1};
    const Point below{ 0, 1};

    class Grid {
    public:
        std::optional<char> at(Point p) const;

        std::map<Point, char>::const_iterator begin() const;
        std::map<Point, char>::const_iterator end() const;
        friend std::istream& operator>>(std::istream& in, Grid& grid) {
            auto v = input::readInput<std::string>(in);
            for(auto [y, s] : std::views::enumerate(v)){
                for(auto [x, c] : std::views::enumerate(s)){
                    grid.grid.emplace(Point{x, y}, c);
                }
            }
            return in;
        }

        size_t count(char c) const;
        std::optional<Point> find(char c) const;
        bool contains(Point p) const;

        char& operator[](Point p);

        size_t size() const;

        auto values() const {
            return grid | std::views::transform([](auto p){ return p.second; });
        }

        auto findAll(char c) const {
            return grid | std::views::filter([c](auto p) { return p.second == c;}) | std::views::transform([](auto p){ return p.first; });
        }
    private:
        std::map<Point, char> grid;
    };
}

