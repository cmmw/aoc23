#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include <unordered_set>
#include <vector>
#include <numeric>

std::pair<std::unordered_set<int32_t>, std::unordered_set<int32_t>> parseCard(const std::string &line) {
    std::stringstream ss(line);
    std::string tmp;
    int32_t gameId;
    ss >> tmp;
    ss >> gameId;
    ss >> tmp;
    std::unordered_set<int32_t> w;
    for (int i = 0; i < 10; i++) {
        int32_t n;
        ss >> n;
        w.insert(n);
    }
    ss >> tmp;
    std::unordered_set<int32_t> nrs;
    for (int i = 0; i < 25; i++) {
        int32_t n;
        ss >> n;
        nrs.insert(n);
    }
    return {w, nrs};
}

void part1() {
    std::ifstream ifs("../../4/input.txt");
    std::string line;
    int64_t sum{};
    while (std::getline(ifs, line)) {
        const auto &[w, nrs] = parseCard(line);
        const auto cnt = std::count_if(nrs.begin(), nrs.end(), [&](int32_t n) { return w.count(n) != 0; });
        if (cnt) sum += (1 << (cnt - 1));
    }
    std::cout << sum << std::endl;
}


void part2() {
    std::vector<int> copies(205, 1);
    std::ifstream ifs("../../4/input.txt");
    std::string line;
    int32_t idx{};
    while (std::getline(ifs, line)) {
        const auto &[w, nrs] = parseCard(line);
        const auto cnt = std::count_if(nrs.begin(), nrs.end(), [&](int32_t n) { return w.count(n) != 0; });
        if (cnt) {
            for (int i = 0; i < cnt; i++)
                copies[i + idx + 1] += copies[idx];
        }
        idx++;
    }
    std::cout << std::accumulate(copies.begin(), copies.end(), 0) << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
