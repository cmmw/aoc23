#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>

using Map = std::tuple<std::vector<char>, int32_t, int32_t>;

Map parseInput() {
    Map m;
    auto& [map, width, height] = m;
    std::ifstream ifs("../../16/input.txt");
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

using Vec = std::pair<int32_t, int32_t>;

char at(const Map& map, const Vec& pos) {
    const auto& [m, width, height] = map;
    return m[pos.first + pos.second * width];
}

Vec operator+(const Vec& v1, const Vec& v2) {
    return {v1.first + v2.first, v1.second + v2.second};
}

using Beam = std::pair<Vec, Vec>;
using Beams = std::set<Beam>;

Beams updateBeam(const Map& map, const Beam& beam) {
    const auto& [pos, dir] = beam;
    const auto c = at(map, pos);
    if (c == '.')
        return {{pos, dir}};
    if (c == '/') {
        if (dir.first > 0)
            return {{pos, {0, -1}}};
        else if (dir.first < 0)
            return {{pos, {0, 1}}};
        else if (dir.second > 0)
            return {{pos, {-1, 0}}};
        else if (dir.second < 0)
            return {{pos, {1, 0}}};
    }
    if (c == '\\') {
        if (dir.first > 0)
            return {{pos, {0, 1}}};
        else if (dir.first < 0)
            return {{pos, {0, -1}}};
        else if (dir.second > 0)
            return {{pos, {1, 0}}};
        else if (dir.second < 0)
            return {{pos, {-1, 0}}};
    }
    if (c == '|' && dir.first != 0)
        return {{{pos}, {0, 1}}, {{pos}, {0, -1}}};
    if (c == '-' && dir.second != 0)
        return {{{pos}, {-1, 0}}, {{pos}, {1, 0}}};
    return {{pos, dir}};
}

Beams advance(const Map& map, const Beams& beams, Beams& e) {
    Beams next;
    const auto& [m, width, height] = map;
    for (auto& [pos, dir] : beams)
        next.insert({pos + dir, dir});
    erase_if(next, [&](const Beam& b) {
        const auto& [pos, dir] = b;
        return pos.first < 0 || pos.first >= width || pos.second < 0 || pos.second >= height || e.contains(b);
    });
    e.insert(next.begin(), next.end());
    return next;
}

size_t fireBeam(const Map& map, const Vec& pos, const Vec& dir) {
    Beams history;
    Beams beams;
    beams.insert({pos, dir});
    history.insert(beams.begin(), beams.end());
    while (!beams.empty()) {
        Beams tmp;
        for (const auto& beam : beams) {
            const auto& next = updateBeam(map, beam);
            tmp.insert(next.begin(), next.end());
        }
        beams = advance(map, tmp, history);
    }
    std::set<Vec> c;
    std::ranges::transform(history, std::inserter(c, c.begin()), [](const Beam& b) { return b.first; });
    return c.size();
}

void part1() {
    const auto& map = parseInput();
    std::cout << fireBeam(map, {0, 0}, {1, 0}) << std::endl;
}

void part2() {
    const auto& map = parseInput();
    const auto& [m, width, height] = map;
    size_t e{};
    for (int i = 0; i < width; i++) {
        e = std::max(e, fireBeam(map, {i, 0}, {0, 1}));
        e = std::max(e, fireBeam(map, {i, height - 1}, {0, -1}));
    }
    for (int i = 0; i < height; i++) {
        e = std::max(e, fireBeam(map, {0, i}, {1, 0}));
        e = std::max(e, fireBeam(map, {width - 1, i}, {-1, 0}));
    }
    std::cout << e << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
