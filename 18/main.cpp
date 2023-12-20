#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using Vec = std::pair<int32_t, int32_t>;

Vec operator*(const Vec& v, int32_t l) {
    return {v.first * l, v.second * l};
}

Vec operator+(const Vec& v1, const Vec& v2) {
    return {v1.first + v2.first, v1.second + v2.second};
}

Vec& operator+=(Vec& v1, const Vec& v2) {
    v1 = v1 + v2;
    return v1;
}

enum class Dir {
    U,
    D,
    L,
    R
};

Dir toDir(const char c) {
    switch (c) {
    case 'U':
        return Dir::U;
    case 'D':
        return Dir::D;
    case 'L':
        return Dir::L;
    case 'R':
        return Dir::R;
    default:
        assert(false);
    }
}

Vec toVec(const Dir dir) {
    switch (dir) {
    case Dir::U:
        return {0, -1};
    case Dir::D:
        return {0, 1};
    case Dir::L:
        return {-1, 0};
    case Dir::R:
        return {1, 0};
    default:
        assert(false);
    }
}

void part1() {
    std::ifstream ifs("../../18/input.txt");
    std::string line;
    Vec pos{}, prev{};
    int32_t border{};
    int32_t area{};
    while (std::getline(ifs, line)) {
        if (line.empty())
            continue;
        std::stringstream ss(line);
        char dir;
        int32_t l;
        std::string col;
        ss >> dir >> l >> col;
        const auto dirVec = toVec(toDir(dir));
        pos += (dirVec * l);
        border += l;
        area += (prev.first * pos.second - pos.first * prev.second);
        prev = pos;
    }
    area >>= 1;
    const auto result = area + (border / 2) + 1;
    std::cout << result << std::endl;
}

void part2() {
}

int main() {
    part1();
    part2();
    return 0;
}
