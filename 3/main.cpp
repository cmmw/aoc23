#include <iostream>
#include <fstream>
#include <iterator>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <set>

constexpr int32_t Width = 140;
constexpr int32_t Height = 140;

using Map = std::vector<char>;

int32_t idx(int32_t x, int32_t y) {
    return x + y * Width;
}

bool isSymbol(char c) {
    return !isdigit(c) && c != '.';
}

bool isPartNumber(int32_t x, int32_t y, const Map &map) {
    for (int i = std::max(0, x - 1); i <= std::min(x + 1, Width - 1); i++) {
        for (int j = std::max(0, y - 1); j <= std::min(y + 1, Height - 1); j++) {
            if (isSymbol(map[idx(i, j)]))
                return true;
        }
    }
    return false;
}

void part1() {
    std::ifstream ifs("../../3/input.txt");
    std::istream_iterator<char> in(ifs);
    Map map(in, {});
    int64_t sum{};
    for (int y = 0; y < Height; y++) {
        std::stringstream ss;
        bool include = false;
        for (int x = 0; x < Width; x++) {
            const bool isDig = isdigit(map[idx(x, y)]);
            if (!include && isDig && isPartNumber(x, y, map))
                include = true;
            const char c = map[idx(x, y)];
            if (isDig)
                ss << c;
            if (!isDig || x == Width - 1) {
                if (!ss.str().empty()) {
                    if (include)
                        sum += std::strtol(ss.str().c_str(), nullptr, 10);
                    ss.str("");
                }
                include = false;
            }
        }
    }
    std::cout << sum << std::endl;
}


std::string_view parseNr(int32_t x, int32_t y, const Map &map) {
    if (!isdigit(map[idx(x, y)])) return {};
    auto s = x;
    for (auto c = map[idx(s, y)]; isdigit(c) && s >= 0; c = map[idx(--s, y)]) {}
    auto e = x;
    for (auto c = map[idx(e, y)]; isdigit(c) && e < Width; c = map[idx(++e, y)]) {}
    return {&map[idx(s + 1, y)], &map[idx(e, y)]};
}

void part2() {
    std::ifstream ifs("../../3/input.txt");
    std::istream_iterator<char> in(ifs);
    Map map(in, {});
    int64_t sum{};
    for (int y = 0; y < Height; y++) {
        std::stringstream ss;
        for (int x = 0; x < Width; x++) {
            const auto c = map[idx(x, y)];
            std::vector<std::string_view> vs;
            if (c == '*') {
                std::set<std::string_view, decltype([](const std::string_view &v1, const std::string_view &v2) {
                    return v1.begin() < v2.begin() || v1.end() < v2.end();
                })> set;
                for (int i = std::max(0, x - 1); i <= std::min(x + 1, Width - 1); i++) {
                    for (int j = std::max(0, y - 1); j <= std::min(y + 1, Height - 1); j++) {
                        const auto sv = parseNr(i, j, map);
                        if (!sv.empty())
                            set.insert(sv);
                    }
                }
                if (set.size() == 2) {
                    int64_t prod = 1;
                    for (const auto sv: set)
                        prod *= std::strtol(std::string(sv).c_str(), nullptr, 10);
                    sum += prod;
                }
            }
        }
    }
    std::cout << sum << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
