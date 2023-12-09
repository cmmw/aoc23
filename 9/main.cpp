#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using Layer = std::vector<int32_t>;
using Layers = std::vector<Layer>;

Layer parseLine(const std::string& line) {
    std::vector<int32_t> values;
    std::stringstream ss(line);
    int32_t value;
    while (ss >> value)
        values.push_back(value);
    return values;
}

void part1() {
    std::ifstream ifs("../../9/input.txt");
    std::string line;
    int64_t sum{};
    while (std::getline(ifs, line)) {
        Layers layers{parseLine(line)};
        while (std::ranges::any_of(layers.back(), [](int32_t v) { return v != 0; })) {
            const auto& prev = layers.back();
            Layer next;
            assert(prev.size() > 1);
            next.reserve(prev.size() - 1);
            for (int i = 0; i < prev.size() - 1; i++)
                next.push_back(prev[i + 1] - prev[i]);
            layers.push_back(next);
        }
        int64_t x{};
        for (auto i = static_cast<int32_t>(layers.size() - 1); i >= 0; i--)
            x += layers[i].back();
        sum += x;
    }
    std::cout << sum << std::endl;
}

void part2() {
    std::ifstream ifs("../../9/input.txt");
    std::string line;
    int64_t sum{};
    while (std::getline(ifs, line)) {
        Layers layers{parseLine(line)};
        while (std::ranges::any_of(layers.back(), [](int32_t v) { return v != 0; })) {
            const auto& prev = layers.back();
            Layer next;
            assert(prev.size() > 1);
            next.reserve(prev.size() - 1);
            for (int i = 0; i < prev.size() - 1; i++)
                next.push_back(prev[i + 1] - prev[i]);
            layers.push_back(next);
        }
        int64_t x{};
        for (auto i = static_cast<int32_t>(layers.size() - 1); i >= 0; i--)
            x = layers[i].front() - x;
        sum += x;
    }
    std::cout << sum << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
