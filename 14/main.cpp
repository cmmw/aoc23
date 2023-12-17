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

void print(const Instance& instance) {
    const auto& [map, width, height] = instance;
    for (int h = 0; h < height - 1; h++) {
        for (int w = 0; w < width; w++) {
            const auto c = map[w + h * width];
            std::cout << c;
        }
        std::cout << "\n";
    }
    std::cout << "\n---\n";
}

Instance& tiltNorth(Instance& instance) {
    auto& [map, width, height] = instance;
    for (int h = 0; h != height - 1; h++) {
        for (int w = 0; w != width; w++) {
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

Instance& tiltSouth(Instance& instance) {
    auto& [map, width, height] = instance;
    for (int h = height - 1; h != 0; h--) {
        for (int w = 0; w != width; w++) {
            auto& c = map[w + h * width];
            if (c == '.') {
                for (int k = h - 1; k >= 0; k--) {
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

Instance& tiltWest(Instance& instance) {
    auto& [map, width, height] = instance;
    for (int h = 0; h != height; h++) {
        for (int w = 0; w != width - 1; w++) {
            auto& c = map[w + h * width];
            if (c == '.') {
                for (int k = w + 1; k < width; k++) {
                    auto& n = map[k + h * width];
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

Instance& tiltEast(Instance& instance) {
    auto& [map, width, height] = instance;
    for (int h = 0; h != height; h++) {
        for (int w = width - 1; w != 0; w--) {
            auto& c = map[w + h * width];
            if (c == '.') {
                for (int k = w - 1; k >= 0; k--) {
                    auto& n = map[k + h * width];
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
    auto instance = parseInput();
    std::cout << load(tiltNorth(instance)) << std::endl;
}

int64_t cycle(const std::vector<int32_t>& values) {
    const auto it = std::find(values.rbegin() + 1, values.rend(), values.back());
    if (it != values.rend()) {
        auto dist = std::distance(values.rbegin(), it);
        return dist;
    }
    return -1;
}

void part2() {
    int64_t value{};
    auto instance = parseInput();
    std::vector<int32_t> values;
    values.reserve(10000);
    const int64_t maxCycles = 1000000000;
    for (int64_t i = 0; i < maxCycles; i++) {
        tiltEast(tiltSouth(tiltWest(tiltNorth(instance))));
        values.push_back(load(instance));
        if (values.size() > 300) {
            const auto c = cycle(values);
            if (c != -1) {
                const auto d = maxCycles - (i + 1);
                const auto r = d % c;
                value = values[values.size() + r - c - 1];
                break;
            }
        }
    }
    std::cout << value << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
