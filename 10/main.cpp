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
    return {};
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

void replaceS(Map& map, const Vec& s, int32_t width, int32_t height) {
    char c1{}, c2{};
    for (auto y = std::max(0, s.second - 1); y <= std::min(s.second + 1, height - 1); y++) {
        for (auto x = std::max(0, s.first - 1); x <= std::min(s.first + 1, width - 1); x++) {
            if (x == s.first || y == s.second) {
                auto c = map[x + y * width];
                if (c != '.' && c != 'S') {
                    const Vec d{x - s.first, y - s.second};
                    auto r = allowed.at(d);
                    if (std::ranges::find(r, c) != r.end()) {
                        if (d.first < 0)
                            c = 'W';
                        else if (d.first > 0)
                            c = 'E';
                        else if (d.second < 0)
                            c = 'N';
                        else if (d.second > 0)
                            c = 'S';
                        c1 == 0 ? c1 = c : c2 = c;
                    }
                }
            }
        }
    }
    if (c1 > c2)
        std::swap(c1, c2);
    char sym;
    if (c1 == 'E') {
        switch (c2) {
        case 'N':
            sym = 'L';
            break;
        case 'S':
            sym = 'F';
            break;
        case 'W':
            sym = '-';
            break;
        default:
            assert(false);
        }
    } else if (c1 == 'N') {
        switch (c2) {
        case 'S':
            sym = '|';
            break;
        case 'W':
            sym = 'J';
            break;
        default:
            assert(false);
        }
    } else if (c1 == 'S') {
        assert(c2 == 'W');
        sym = '7';
    }
    map[s.first + s.second * width] = sym;
}

void part2() {
    const auto& [omap, start, width, height] = parseInput();
    Map map;
    map.resize(omap.size(), '.');
    map[start.first + start.second * width] = 'S';
    auto prev = start;
    Vec next = first(omap, start, width, height);
    char c = omap[next.first + next.second * width];
    while (c != 'S') {
        map[next.first + next.second * width] = c;
        auto tmp = next;
        move(omap, width, prev, next);
        prev = tmp;
        c = omap[next.first + next.second * width];
    }
    int32_t cnt{};
    replaceS(map, start, width, height);
    for (int y = 0; y < height; y++) {
        bool inside = false;
        char edgeStart{};

        for (int x = 0; x < width; x++) {
            c = map[x + y * width];
            if (c == 'L' || c == 'F') {
                edgeStart = c;
            } else if (edgeStart != 0 && c == 'J' || c == '7') {
                switch (edgeStart) {
                case 'L':
                    inside = c == 'J' == inside;
                    break;
                case 'F':
                    inside = c == '7' == inside;
                    break;
                default:
                    assert(false);
                }
                edgeStart = 0;
            } else {
                if (edgeStart == 0) {
                    if (c != '.')
                        inside = !inside;
                    if (c == '.' && inside)
                        cnt++;
                }
            }
        }
    }
    std::cout << cnt << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
