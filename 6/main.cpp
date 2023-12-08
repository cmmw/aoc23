#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <vector>

using Races = std::vector<std::pair<int64_t, int64_t>>;

Races parseInput() {
    Races races;
    std::ifstream ifs("../../6/input.txt");
    std::string line;
    std::getline(ifs, line);
    std::stringstream ss(line);
    std::string tmp;
    ss >> tmp;
    int64_t time;
    while (ss >> time)
        races.emplace_back(time, 0);
    std::getline(ifs, line);
    ss = std::stringstream(line);
    ss >> tmp;
    int32_t i{};
    int64_t distance;
    while (ss >> distance)
        races[i++].second = distance;
    return races;
}

void part1() {
    Races races = parseInput();
    int64_t sum = 1;
    for (const auto& [time, dist] : races) {
        int32_t cnt{};
        for (int i = 1; i < time; i++)
            cnt += (time - i) * i > dist;
        sum *= cnt;
    }
    std::cout << sum << std::endl;
}

void part2() {
    Races races;
    std::ifstream ifs("../../6/input.txt");
    std::string line;
    std::getline(ifs, line);
    std::stringstream ss(line);
    std::string tmp;
    std::stringstream acc;
    ss >> tmp;
    while (ss >> tmp)
        acc << tmp;
    int64_t time;
    acc >> time;
    std::getline(ifs, line);
    ss = std::stringstream(line);
    ss >> tmp;
    acc = std::stringstream();
    while (ss >> tmp)
        acc << tmp;
    int64_t dist;
    acc >> dist;
    int32_t cnt{};
    for (int i = 1; i < time; i++)
        cnt += (time - i) * i > dist;
    std::cout << cnt << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
