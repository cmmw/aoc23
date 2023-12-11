#include <cstdint>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <vector>

using Vec = std::pair<int32_t, int32_t>;
using Galaxies = std::vector<Vec>;
using Expand = std::unordered_set<int32_t>;
using Instance = std::tuple<Galaxies, Expand, Expand>;

Instance parseInput() {
    std::ifstream ifs("../../11/input.txt");
    std::string tmp;
    Galaxies galaxies;
    int32_t y{};
    int32_t x{};
    while (ifs >> tmp) {
        x = 0;
        for (auto c : tmp) {
            if (c == '#')
                galaxies.emplace_back(x, y);
            x++;
        }
        y++;
    }
    Expand ex, ey;
    for (int i = 0; i < x; i++)
        ex.insert(i);
    for (int i = 0; i < y; i++)
        ey.insert(i);
    for (const auto& g : galaxies) {
        ex.erase(g.first);
        ey.erase(g.second);
    }
    return {galaxies, ex, ey};
}

int32_t dist(const Galaxies& galaxies, const Expand& ex, const Expand& ey, int32_t i, int32_t j) {
    int32_t d{};
    const auto& g1 = galaxies[i];
    const auto& g2 = galaxies[j];
    for (const auto e : ex) {
        if (e > std::min(g1.first, g2.first) && e < std::max(g1.first, g2.first))
            d++;
    }
    for (const auto e : ey) {
        if (e > std::min(g1.second, g2.second) && e < std::max(g1.second, g2.second))
            d++;
    }
    d += std::abs(g1.first - g2.first) + std::abs(g1.second - g2.second);
    return d;
}

void part1() {
    int32_t sum{};
    const auto& [galaxies, ex, ey] = parseInput();
    for (int i = 0; i < galaxies.size() - 1; i++) {
        for (int j = i + 1; j < galaxies.size(); j++) {
            sum += dist(galaxies, ex, ey, i, j);
        }
    }
    std::cout << sum << std::endl;
}

void part2() {
}

int main() {
    part1();
    part2();
    return 0;
}
