#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

using Sequence = std::vector<std::string>;

Sequence parseInput() {
    Sequence sequence;
    std::ifstream ifs("../../15/input.txt");
    std::string line;
    while (std::getline(ifs, line, ','))
        sequence.push_back(line);
    while (std::isspace(sequence.back().back()))
        sequence.back().pop_back();
    return sequence;
}

int32_t hash(const std::string& s) {
    return std::accumulate(s.begin(), s.end(), 0, [](int32_t s, char c) { return ((s + c) * 17 % 256); });
}

void part1() {
    const auto sequence = parseInput();
    std::cout << std::accumulate(sequence.begin(), sequence.end(), 0, [](int64_t s, const std::string& seq) { return s + hash(seq); }) << std::endl;
}

struct Box {
    std::string label;
    int32_t value;
    bool operator==(const Box& b) const {
        return this->label == b.label;
    }
};
using Boxes = std::vector<Box>;

void part2() {
    std::array<Boxes, 256> hmap;
    std::ifstream ifs("../../15/input.txt");
    std::string line;
    while (std::getline(ifs, line, ',')) {
        while (std::isspace(line.back()))
            line.pop_back();
        if (auto pos = line.find('='); pos != std::string::npos) {
            const Box b{line.substr(0, pos), std::strtol(line.substr(pos + 1).c_str(), nullptr, 10)};
            auto& boxes = hmap[hash(b.label)];
            if (auto it = std::ranges::find(boxes, b); it != boxes.end())
                *it = b;
            else
                boxes.push_back(b);
        } else if (pos = line.find('-'); pos != std::string::npos) {
            const auto label = line.substr(0, pos);
            auto& boxes = hmap[hash(label)];
            if (auto it = std::ranges::find(boxes, Box{label, 0}); it != boxes.end())
                boxes.erase(it);
        }
    }
    int64_t sum{};
    for (int i = 0; i < hmap.size(); i++) {
        for (int j = 0; j < hmap[i].size(); j++)
            sum += (i + 1) * (j + 1) * hmap[i][j].value;
    }
    std::cout << sum << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
