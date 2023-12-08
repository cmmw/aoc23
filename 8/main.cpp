#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

using Map = std::unordered_map<std::string, std::pair<std::string, std::string>>;
using Instance = std::pair<std::string, Map>;

Instance parseInput() {
    Instance instance;
    std::ifstream ifs("../../8/input.txt");
    ifs >> instance.first;
    std::string line;
    while (std::getline(ifs, line)) {
        if (line.empty())
            continue;
        std::stringstream ss(line);
        std::string node, left, right;
        ss >> node >> left >> left >> right;
        left = left.substr(1);
        left.pop_back();
        right.pop_back();
        instance.second[node] = {left, right};
    }
    return instance;
}

void part1() {
    int64_t sum{};
    size_t idx{};
    const auto& [cmd, map] = parseInput();
    std::string node = "AAA";
    while (node != "ZZZ") {
        const auto& [left, right] = map.at(node);
        if (cmd[idx] == 'L')
            node = left;
        else
            node = right;
        sum++;
        idx = (idx + 1) % cmd.size();
    }
    std::cout << sum << std::endl;
}

void part2() {
    std::ifstream ifs("../../8/input.txt");
    int64_t sum{};
    std::cout << sum << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
