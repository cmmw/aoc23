#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <vector>

using Map = std::vector<char>;
using Instance = std::tuple<Map, int32_t, int32_t>;
using Instances = std::vector<Instance>;

Instances parseInput() {
    Instances instances;
    std::ifstream ifs("../../13/input.txt");
    std::string line;
    int32_t width{}, height{};
    Map map;
    while (std::getline(ifs, line)) {
        if (line.empty()) {
            instances.emplace_back(map, width, height);
            map.clear();
            width = height = 0;
            continue;
        }
        if (width == 0)
            width = line.size();
        height++;
        map.insert(map.end(), line.begin(), line.end());
    }
    if (!map.empty())
        instances.emplace_back(map, width, height);
    return instances;
}

bool isHorMirrored(const Instance& instance, int32_t y, int32_t m) {
    const auto& [map, width, height] = instance;
    for (int t = 1; t <= std::min(m, width - m); t++) {
        if (map[m - t + y * width] != map[m + (t - 1) + y * width])
            return false;
    }
    return true;
}

bool isVertMirrored(const Instance& instance, int32_t x, int32_t m) {
    const auto& [map, width, height] = instance;
    for (int t = 1; t <= std::min(m, height - m); t++) {
        if (map[(m - t) * width + x] != map[(m + t - 1) * width + x])
            return false;
    }
    return true;
}

int32_t mirror(const Instance& instance) {
    const auto& [map, width, height] = instance;
    std::vector<int> xs;
    std::vector<int> mxs(width - 1);
    std::iota(mxs.begin(), mxs.end(), 1);
    for (int y = 0; y < height && !mxs.empty(); y++) {
        mxs.erase(std::remove_if(mxs.begin(), mxs.end(), [&](int32_t m) { return !isHorMirrored(instance, y, m); }), mxs.end());
    }
    std::vector<int> ys;
    std::vector<int> mys(height - 1);
    std::iota(mys.begin(), mys.end(), 1);
    for (int x = 0; x < width && !mys.empty(); x++) {
        mys.erase(std::remove_if(mys.begin(), mys.end(), [&](int32_t m) { return !isVertMirrored(instance, x, m); }), mys.end());
    }
    assert((mxs.size() == 1) != (mys.size() == 1));
    return mxs.empty() ? (100 * mys[0]) : mxs[0];
}

void part1() {
    int64_t sum{};
    const auto& instances = parseInput();
    for (const auto& instance : instances)
        sum += mirror(instance);
    std::cout << sum << std::endl;
}

void part2() {
    int64_t sum{};
    std::cout << sum << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
