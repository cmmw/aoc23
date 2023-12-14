#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
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

using Key = std::tuple<std::string_view, int32_t, int32_t>;
using Cache = std::map<Key, int64_t>;

Cache cache;
int64_t p(std::string::const_iterator s, std::string::const_iterator e, const Groups& groups, int32_t cg = 0, int32_t j = 0) {

    if (auto it = cache.find({{s, e}, cg, j}); it != cache.end())
        return it->second;

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
        cache[{{s, e}, cg, j}] = p(s + 1, e, groups, 0, j + 1);
    else if (c == '.' && cg == 0)
        cache[{{s, e}, cg, j}] = p(s + 1, e, groups, 0, j);
    else if (c == '?' && cg < g)
        cache[{{s, e}, cg, j}] = p(s + 1, e, groups, cg + 1, j) + ((cg == 0) ? p(s + 1, e, groups, 0, j) : 0);
    else if (c == '?' && cg == g)
        cache[{{s, e}, cg, j}] = p(s + 1, e, groups, 0, j + 1);
    else if (c == '#')
        cache[{{s, e}, cg, j}] = p(s + 1, e, groups, cg + 1, j);
    return cache[{{s, e}, cg, j}];
}

void part1() {
    int64_t sum{};
    std::ifstream ifs("../../12/input.txt");
    std::string tmp;
    while (std::getline(ifs, tmp)) {
        const auto& [line, groups] = parseLine(tmp);
        cache.clear();
        sum += p(line.begin(), line.end(), groups);
    }
    std::cout << sum << std::endl;
}

void part2() {
    int64_t sum{};
    std::ifstream ifs("../../12/input.txt");
    std::string tmp;
    while (std::getline(ifs, tmp)) {
        const auto& [line, groups] = parseLine(tmp);
        const std::string newLine = line + "?" + line + "?" + line + "?" + line + "?" + line;
        auto newGroups = groups;
        for (int i = 0; i < 4; i++)
            newGroups.insert(newGroups.end(), groups.begin(), groups.end());
        cache.clear();
        sum += p(newLine.begin(), newLine.end(), newGroups);
    }
    std::cout << sum << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
