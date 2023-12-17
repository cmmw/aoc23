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

std::pair<std::vector<int>, std::vector<int>> getMirror(const Instance& instance, const std::vector<int>& exx = {}, const std::vector<int>& exy = {}) {
    const auto& [map, width, height] = instance;
    std::vector<int> mxs(width - 1);
    std::iota(mxs.begin(), mxs.end(), 1);
    for (auto tmp : exx) {
        auto [first, last] = std::ranges::remove(mxs, tmp);
        mxs.erase(first, last);
    }
    for (int y = 0; y < height && !mxs.empty(); y++)
        mxs.erase(std::remove_if(mxs.begin(), mxs.end(), [&](int32_t m) { return !isHorMirrored(instance, y, m); }), mxs.end());
    std::vector<int> mys(height - 1);
    std::iota(mys.begin(), mys.end(), 1);
    for (auto tmp : exy) {
        auto [first, last] = std::ranges::remove(mys, tmp);
        mys.erase(first, last);
    }
    for (int x = 0; x < width && !mys.empty(); x++)
        mys.erase(std::remove_if(mys.begin(), mys.end(), [&](int32_t m) { return !isVertMirrored(instance, x, m); }), mys.end());
    if (mxs.empty() && mys.empty())
        return {};
    assert((mxs.size() == 1) != (mys.size() == 1));
    return {mxs, mys};
}

void part1() {
    int64_t sum{};
    const auto& instances = parseInput();
    for (const auto& instance : instances) {
        const auto& [mxs, mys] = getMirror(instance);
        assert((mxs.size() == 1) != (mys.size() == 1));
        sum += (mxs.empty() ? (100 * mys[0]) : mxs[0]);
    }
    std::cout << sum << std::endl;
}

void part2() {
    int64_t sum{};
    auto instances = parseInput();
    for (auto& instance : instances) {
        const auto& [exx, exy] = getMirror(instance);
        auto& [map, width, height] = instance;
        for (int h = 0; h < height; h++) {
            for (int w = 0; w < width; w++) {
                const auto o = map[h * width + w];
                if (o == '?')
                    continue;
                map[h * width + w] = (o == '.') ? '#' : '-';
                const auto& [mxs, mys] = getMirror(instance, exx, exy);
                map[h * width + w] = o;
                if ((mxs.size() == 1) != (mys.size() == 1)) {
                    sum += (mxs.empty() ? (100 * mys[0]) : mxs[0]);
                    h = height;
                    break;
                }
            }
        }
    }
    std::cout << sum << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
