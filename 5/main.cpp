#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <sstream>
#include <unordered_map>
#include <vector>

enum class Cat {
    Soil,
    Fertilizer,
    Water,
    Light,
    Temperature,
    Humidity,
    Location
};

using Map = std::unordered_map<Cat, std::vector<std::tuple<int64_t, int64_t, int64_t>>>;
using Seeds = std::vector<int64_t>;
using Pair = std::pair<Seeds, Map>;

const std::unordered_map<std::string, Cat> stringMap{
    {"seed-to-soil", Cat::Soil},
    {"soil-to-fertilizer", Cat::Fertilizer},
    {"fertilizer-to-water", Cat::Water},
    {"water-to-light", Cat::Light},
    {"light-to-temperature", Cat::Temperature},
    {"temperature-to-humidity", Cat::Humidity},
    {"humidity-to-location", Cat::Location},
};

Pair parseInput() {
    std::ifstream ifs("../../5/input.txt");
    std::string line;
    std::getline(ifs, line);
    std::stringstream ss(line.substr(7));
    std::istream_iterator<int64_t> in(ss);
    Seeds seeds(in, {});
    Map map;
    Cat curCat;
    while (std::getline(ifs, line)) {
        if (line.empty())
            continue;
        ss = std::stringstream(line);
        std::string tmp;
        ss >> tmp;
        const auto it = stringMap.find(tmp);
        if (it != stringMap.end()) {
            curCat = it->second;
            continue;
        }
        ss = std::stringstream(line);
        int64_t dst, src, len;
        ss >> dst >> src >> len;
        map[curCat].emplace_back(dst, src, len);
    }
    return {seeds, map};
}

void part1() {
    int64_t min = std::numeric_limits<int64_t>::max();
    const auto& [seeds, map] = parseInput();
    for (const auto seed : seeds) {
        int64_t next = seed;
        for (const auto cat : {Cat::Soil, Cat::Fertilizer, Cat::Water, Cat::Light, Cat::Temperature, Cat::Humidity, Cat::Location}) {
            for (const auto& [dst, src, len] : map.at(cat)) {
                if (src <= next && next < src + len) {
                    next = dst + (next - src);
                    break;
                }
            }
        }
        min = std::min(min, next);
    }
    std::cout << min << std::endl;
}

using Range = std::pair<int64_t, int64_t>;
using Ranges = std::list<Range>;

bool nextIntervals(int64_t dst, int64_t src, int64_t len, const Range& range, Ranges& current, Ranges& next) {
    const auto rs = range.first;
    const auto rl = range.second;

    if (rs + rl <= src || rs >= src + len)
        return false;

    if (rs < src) {
        if (rs + rl > src + len) {
            // 4
            next.emplace_back(dst, len);
            current.emplace_back(rs, src - rs);
            current.emplace_back(src + len, rs + rl - (src + len));
        } else {
            // 2
            next.emplace_back(dst, rs + rl - src);
            current.emplace_back(rs, src - rs);
        }
    } else if (rs + rl > src + len) {
        // 3
        next.emplace_back(dst + rs - src, len - (rs - src));
        current.emplace_back(src + len, rs + rl - (src + len));
    } else {
        // 1
        next.emplace_back(dst + (rs - src), rl);
    }
    return true;
}

void part2() {
    int64_t min = std::numeric_limits<int64_t>::max();
    const auto& [seeds, map] = parseInput();
    for (int i = 0; i < seeds.size() - 1; i += 2) {
        Ranges ranges{{seeds[i], seeds[i + 1]}};
        for (const auto cat : {Cat::Soil, Cat::Fertilizer, Cat::Water, Cat::Light, Cat::Temperature, Cat::Humidity, Cat::Location}) {
            Ranges next;
            while (!ranges.empty()) {
                auto range = ranges.front();
                ranges.pop_front();
                bool processed = false;
                for (const auto& [dst, src, len] : map.at(cat)) {
                    processed |= nextIntervals(dst, src, len, range, ranges, next);
                    if (processed)
                        break;
                }
                if (!processed)
                    next.emplace_back(range);
            }
            next.swap(ranges);
        }
        const auto tmp = std::min_element(ranges.begin(), ranges.end(), [](const auto& range1, const auto& range2) { return range1.first < range2.first; });
        min = std::min(min, tmp->first);
    }
    std::cout << min << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
