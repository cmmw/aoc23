#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <vector>

using Map = std::vector<char>;
using Instance = std::tuple<Map, int32_t, int32_t>;

Instance parseInput() {
    Instance instance;
    auto& [map, width, height] = instance;
    std::ifstream ifs("../../14/input.txt");
    std::string line;
    while (std::getline(ifs, line)) {
        if (line.empty())
            continue;
        if (width == 0)
            width = line.size();
        height++;
        map.insert(map.end(), line.begin(), line.end());
    }
    return instance;
}

Instance tilt(Instance instance) {
    auto& [map, width, height] = instance;
    for (int h = 0; h < height - 1; h++) {
        for (int w = 0; w < width; w++) {
            auto& c = map[w + h * width];
            if (c == '.') {
                for (int k = h + 1; k < height; k++) {
                    auto& n = map[w + k * width];
                    if (n == '#')
                        break;
                    if (n == 'O') {
                        c = n;
                        n = '.';
                        break;
                    }
                }
            }
        }
    }
    return instance;
}

int32_t load(const Instance& instance) {
    auto& [map, width, height] = instance;
    int32_t load{};
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            const auto c = map[w + h * width];
            if (c == 'O')
                load += (height - h);
        }
    }
    return load;
}

void part1() {
    std::cout << load(tilt(parseInput())) << std::endl;
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
