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

void part1() {
    const auto sequence = parseInput();
    std::cout << std::accumulate(sequence.begin(), sequence.end(), 0, [](int64_t s, const std::string& seq) {
        int64_t t{};
        for (const auto c : seq) {
            t += c;
            t *= 17;
            t %= 256;
        }
        return s + t;
    }) << std::endl;
}

void part2() {
}

int main() {
    part1();
    part2();
    return 0;
}
