#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
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

void part2() {
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

int main() {
    part1();
    part2();
    return 0;
}
