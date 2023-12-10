#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using Map = std::vector<char>;
using Vec = std::pair<int32_t, int32_t>;
using Instance = std::tuple<Map, Vec, int32_t, int32_t>;

Instance parseInput() {
    std::ifstream ifs("../../10/input.txt");
    std::string tmp;
    Map map;
    Vec start;
    size_t width{}, height{};
    while (ifs >> tmp) {
        map.insert(map.end(), tmp.begin(), tmp.end());
        const auto p = tmp.find('S');
        if (p != std::string::npos)
            start = {p, height};
        if (width == 0)
            width = tmp.length();
        height++;
    }
    return {map, start, width, height};
}

void move(const Map& map, const size_t width, const Vec& prev, Vec& cur) {
    const auto cIdx = cur.first + cur.second * width;
    auto const c = map[cIdx];
    const auto fromNW = prev.first < cur.first || prev.second < cur.second;
    const auto fromNE = prev.first > cur.first || prev.second < cur.second;
    Vec d;
    switch (c) {
    case '|':
        d = fromNW ? Vec{0, 1} : Vec{0, -1};
        break;
    case '-':
        d = fromNW ? Vec{1, 0} : Vec{-1, 0};
        break;
    case 'L':
        d = fromNW ? Vec{1, 0} : Vec{0, -1};
        break;
    case 'J':
        d = fromNE ? Vec{-1, 0} : Vec{0, -1};
        break;
    case '7':
        d = fromNW ? Vec{0, 1} : Vec{-1, 0};
        break;
    case 'F':
        d = fromNE ? Vec{0, 1} : Vec{1, 0};
        break;
    default:
        assert(false);
    }
    cur.first += d.first;
    cur.second += d.second;
}

const std::map<Vec, std::vector<char>> allowed{
    {{1, 0}, {'-', '7', 'J'}},
    {{-1, 0}, {'-', 'F', 'L'}},
    {{0, 1}, {'|', 'J', 'L'}},
    {{0, -1}, {'|', 'F', '7'}},
};

Vec first(const Map& map, const Vec& s, int32_t width, int32_t height) {
    for (auto y = std::max(0, s.second - 1); y <= std::min(s.second + 1, height - 1); y++) {
        for (auto x = std::max(0, s.first - 1); x <= std::min(s.first + 1, width - 1); x++) {
            if (x == s.first || y == s.second) {
                const auto c = map[x + y * width];
                if (c != '.' && c != 'S') {
                    const Vec d{x - s.first, y - s.second};
                    auto r = allowed.at(d);
                    if (std::ranges::find(r, c) != r.end())
                        return {x, y};
                }
            }
        }
    }
}

void part1() {
    const auto& [map, start, width, height] = parseInput();
    auto prev = start;
    Vec next = first(map, start, width, height);
    int32_t cnt = 1;
    char c{};
    while (c != 'S') {
        auto tmp = next;
        move(map, width, prev, next);
        prev = tmp;
        c = map[next.first + next.second * width];
        cnt++;
    }
    std::cout << (cnt >> 1) << std::endl;
}

void part2() {
    std::ifstream ifs("../../10/input.txt");
}

int main() {
    part1();
    part2();
    return 0;
}
