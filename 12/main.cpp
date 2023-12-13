#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

using Groups = std::vector<int32_t>;
using Instance = std::tuple<std::string, Groups>;

Instance parseLine(const std::string& line) {
    std::stringstream ss(line);
    Instance i;
    ss >> std::get<0>(i);
    char c;
    int32_t n;
    while (ss >> n) {
        std::get<1>(i).push_back(n);
        ss >> c;
    }
    return i;
}

int32_t p(std::string::const_iterator s, std::string::const_iterator e, const Groups& groups, int32_t cg = 0, int32_t j = 0) {
    if (s == e && ((j == groups.size() && cg == 0) || (j == groups.size() - 1 && groups.back() == cg)))
        return 1;
    if (s == e)
        return 0;
    if (j >= groups.size()) {
        if (std::ranges::find_if(s, e, [](const char c) { return c == '#'; }) != e)
            return 0;
        return 1;
    }
    const auto g = groups[j];
    const auto c = *s;
    if (cg > g)
        return 0;
    if (c == '.' && g == cg)
        return p(s + 1, e, groups, 0, j + 1);
    if (c == '.' && cg == 0)
        return p(s + 1, e, groups, 0, j);
    if (c == '?' && cg < g)
        return p(s + 1, e, groups, cg + 1, j) + ((cg == 0) ? p(s + 1, e, groups, 0, j) : 0);
    if (c == '?' && cg == g)
        return p(s + 1, e, groups, 0, j + 1);
    if (c == '#')
        return p(s + 1, e, groups, cg + 1, j);
    return 0;
}

void part1() {
    int32_t sum{};
    std::ifstream ifs("../../12/input.txt");
    std::string tmp;
    while (std::getline(ifs, tmp)) {
        const auto& [line, groups] = parseLine(tmp);
        sum += p(line.begin(), line.end(), groups);
    }
    std::cout << sum << std::endl;
}

void part2() {
    int32_t sum{};
    std::cout << sum << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
