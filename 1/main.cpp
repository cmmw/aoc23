#include <iostream>
#include <fstream>
#include <iterator>
#include <unordered_map>

void part1() {
    int64_t sum{};
    std::ifstream ifs("../../1/input.txt");
    for (std::istream_iterator<std::string> in(ifs); in != std::istream_iterator<std::string>{}; ++in) {
        const auto d0 = std::find_if(in->begin(), in->end(), ::isdigit);
        const auto d1 = std::find_if(in->rbegin(), in->rend(), ::isdigit);
        sum += (*d0 - '0') * 10 + (*d1 - '0');
    }
    std::cout << sum << std::endl;
}


const std::unordered_map<std::string, int> map{
        {"1",     1},
        {"2",     2},
        {"3",     3},
        {"4",     4},
        {"5",     5},
        {"6",     6},
        {"7",     7},
        {"8",     8},
        {"9",     9},
        {"one",   1},
        {"two",   2},
        {"three", 3},
        {"four",  4},
        {"five",  5},
        {"six",   6},
        {"seven", 7},
        {"eight", 8},
        {"nine",  9},
};


void part2() {
    int64_t sum{};
    std::ifstream ifs("../../1/input.txt");
    for (std::istream_iterator<std::string> in(ifs); in != std::istream_iterator<std::string>{}; ++in) {
        int64_t d0, d1;
        size_t idx0{std::numeric_limits<size_t>::max()}, idx1{std::numeric_limits<size_t>::min()};
        for (const auto &s: map) {
            if (auto idx = in->find(s.first); idx != std::string::npos && idx < idx0) {
                d0 = s.second;
                idx0 = idx;
            }
            if (auto idx = in->rfind(s.first); idx != std::string::npos && idx > idx1) {
                d1 = s.second;
                idx1 = idx;
            }
        }
        sum += d0 * 10 + d1;
    }
    std::cout << sum << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
