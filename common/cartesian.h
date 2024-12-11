#include <istream>
#include <map>
#include <optional>
#include <vector>

#include "input.h"
namespace cartesian {
    struct Point {
        long x = 0;
        long y = 0;

        friend auto operator<=>(const Point& lhs, const Point& rhs) = default;
        friend Point operator+(const Point& lhs, const Point& rhs) {
            return Point{lhs.x + rhs.x, lhs.y + rhs.y};
        }
    };
    
    std::vector<Point> getDirectionOffsets();

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
    private:
        std::map<Point, char> grid;
    };
}

