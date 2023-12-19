#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <ranges>
#include <set>
#include <vector>

using Map = std::tuple<std::vector<char>, int32_t, int32_t>;
using Vec = std::pair<int32_t, int32_t>;
using Head = std::tuple<Vec, Vec, int32_t, int32_t>;
using HeadQueue = std::priority_queue<Head, std::vector<Head>, decltype([](const Head& h1, const Head& h2) {
                                          const auto& [p1, d1, c1, s1] = h1;
                                          const auto& [p2, d2, c2, s2] = h2;
                                          return c1 > c2;
                                      })>;
using HeadVec = std::vector<Head>;
using HeadId = std::tuple<Vec, Vec, int32_t>;
using Cache = std::set<HeadId>;

Map parseInput() {
    Map m;
    auto& [map, width, height] = m;
    std::ifstream ifs("../../17/input.txt");
    std::string line;
    while (std::getline(ifs, line)) {
        if (line.empty())
            continue;
        if (width == 0)
            width = static_cast<int32_t>(line.size());
        height++;
        map.insert(map.end(), line.begin(), line.end());
    }
    return m;
}

int32_t at(const Map& map, const Vec& pos) {
    const auto& [m, width, height] = map;
    return m[pos.first + pos.second * width] - '0';
}

Vec operator+(const Vec& v1, const Vec& v2) {
    return {v1.first + v2.first, v1.second + v2.second};
}

Vec operator-(const Vec& v) {
    return {-v.first, -v.second};
}

HeadId id(const Head& head) {
    const auto& [pos, dir, c, s] = head;
    return {pos, dir, s};
}

HeadVec next(const Map& map, const Head& current) {
    HeadVec heads;
    const auto& [m, width, height] = map;
    const auto& [pos, dir, c, s] = current;
    for (const Vec& d : {Vec{1, 0}, {0, 1}, {-1, 0}, {0, -1}}) {
        if (dir == -d)
            continue;
        if (dir == d && s == 3)
            continue;
        const auto& n = pos + d;
        if (n.first < 0 || n.first >= width || n.second < 0 || n.second >= height)
            continue;
        heads.emplace_back(n, d, c + at(map, n), dir == d ? (s + 1) : 1);
    }
    return heads;
}

void part1() {
    int32_t v{};
    const auto& map = parseInput();
    const auto& [_, width, height] = map;
    HeadQueue newHeads;
    newHeads.emplace();
    Cache cache;
    while (!newHeads.empty()) {
        auto current = newHeads.top();
        const auto& [pos, _0, c, _1] = current;
        if (pos.first == width - 1 && pos.second == height - 1) {
            v = c;
            break;
        }
        newHeads.pop();
        for (const auto& n : next(map, current)) {
            if (const auto& [it, ok] = cache.insert(id(n)); ok)
                newHeads.push(n);
        }
    }
    std::cout << v << std::endl;
}

HeadVec next2(const Map& map, const Head& current) {
    HeadVec heads;
    const auto& [m, width, height] = map;
    const auto& [pos, dir, c, s] = current;
    for (const Vec& d : {Vec{1, 0}, {0, 1}, {-1, 0}, {0, -1}}) {
        if (dir == -d)
            continue;
        if (dir == d && s == 10)
            continue;
        if (dir != Vec{0, 0} && dir != d && s < 4)
            continue;
        const auto& n = pos + d;
        if (n.first < 0 || n.first >= width || n.second < 0 || n.second >= height)
            continue;
        heads.emplace_back(n, d, c + at(map, n), dir == d ? (s + 1) : 1);
    }
    return heads;
}

void part2() {
    int32_t v{};
    const auto& map = parseInput();
    const auto& [_, width, height] = map;
    HeadQueue newHeads;
    newHeads.emplace();
    Cache cache;
    while (!newHeads.empty()) {
        auto current = newHeads.top();
        const auto& [pos, _0, c, _1] = current;
        if (pos.first == width - 1 && pos.second == height - 1) {
            v = c;
            break;
        }
        newHeads.pop();
        for (const auto& n : next2(map, current)) {
            if (const auto& [it, ok] = cache.insert(id(n)); ok)
                newHeads.push(n);
        }
    }
    std::cout << v << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
