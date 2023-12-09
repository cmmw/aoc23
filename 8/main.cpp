#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

using Map = std::unordered_map<std::string, std::pair<std::string, std::string>>;
using Instance = std::tuple<std::string, Map, std::vector<std::string>>;

Instance parseInput() {
    Instance instance;
    std::ifstream ifs("../../8/input.txt");
    ifs >> std::get<0>(instance);
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
        std::get<1>(instance)[node] = {left, right};
        if (node.back() == 'A')
            std::get<2>(instance).push_back(node);
    }
    return instance;
}

void part1() {
    int64_t sum{};
    size_t idx{};
    const auto& [cmd, map, _] = parseInput();
    std::string node = "AAA";
    while (node != "ZZZ") {
        const auto& [left, right] = map.at(node);
        node = cmd[idx] == 'L' ? left : right;
        sum++;
        idx = (idx + 1) % cmd.size();
    }
    std::cout << sum << std::endl;
}

int64_t gcd(int64_t x, int64_t y) {
    if (x == y)
        return x;
    if (x > y)
        return gcd(x - y, y);
    return gcd(x, y - x);
}

void part2() {
    size_t idx{};
    auto [cmd, map, nodes] = parseInput();
    std::vector<int64_t> rounds(nodes.size(), 0);
    int64_t r{};
    while (std::ranges::any_of(rounds, [](int n) { return n == 0; })) {
        const bool cmdLeft = cmd[idx] == 'L';
        int32_t i{};
        r++;
        for (auto& node : nodes) {
            const auto& [left, right] = map.at(node);
            node = cmdLeft ? left : right;
            if (node.back() == 'Z' && rounds[i] == 0)
                rounds[i] = r;
            i++;
        }
        idx = (idx + 1) % cmd.size();
    }
    while (rounds.size() > 1) {
        auto x = rounds.back();
        rounds.pop_back();
        auto y = rounds.back();
        rounds.pop_back();
        auto g = gcd(x, y);
        rounds.insert(rounds.begin(), (x * y) / g);
    }
    std::cout << rounds.front() << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
