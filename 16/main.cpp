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
            width = line.size();
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
using Beams = std::vector<Beam>;

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

using Tiles = std::set<Beam>;

void advance(const Map& map, Beams& beams, Tiles& e) {
    const auto& [m, width, height] = map;
    for (auto& [pos, dir] : beams)
        pos = pos + dir;
    beams.erase(std::remove_if(beams.begin(), beams.end(), [&](const Beam& b) {
                    const auto& [pos, dir] = b;
                    return pos.first < 0 || pos.first >= width || pos.second < 0 || pos.second >= height || e.contains(b);
                }),
                beams.end());
    e.insert(beams.begin(), beams.end());
}

void part1() {
    Tiles e;
    Beams beams;
    beams.push_back({{}, {1, 0}});
    e.insert(beams.begin(), beams.end());
    const auto& map = parseInput();
    while (!beams.empty()) {
        Beams tmp;
        for (const auto& beam : beams) {
            const auto& next = updateBeam(map, beam);
            tmp.insert(tmp.end(), next.begin(), next.end());
        }
        advance(map, tmp, e);
        std::swap(tmp, beams);
    }
    std::set<Vec> c;
    std::ranges::transform(e, std::inserter(c, c.begin()), [](const Beam& b) { return b.first; });
    std::cout << c.size() << std::endl;
}

void part2() {
}

int main() {
    part1();
    part2();
    return 0;
}
