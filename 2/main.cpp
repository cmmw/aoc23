#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <unordered_map>

const std::unordered_map<std::string, int32_t> limits{
    {"red", 12},
    {"green", 13},
    {"blue", 14},
};

void part1() {
    std::ifstream ifs("../../2/input.txt");
    std::string line;
    int64_t sumIds{};

    while (std::getline(ifs, line)) {
        std::stringstream ss(line);
        std::string tmp;
        int32_t gameId;
        ss >> tmp;
        ss >> gameId;
        ss >> tmp;

        std::unordered_map<std::string, int32_t> cnt;
        while (!ss.eof()) {
            int32_t nr;
            std::string col;
            ss >> nr;
            ss >> col;
            const bool done = col.back() == ';';
            if (col.back() == ';' || col.back() == ',')
                col.pop_back();
            cnt[col] += std::max(cnt[col], nr);
            if (cnt[col] > limits.at(col)) {
                gameId = 0;
                break;
            }

            if (done)
                cnt.clear();
        }
        sumIds += gameId;
    }
    std::cout << sumIds << std::endl;
}

void part2() {
    std::ifstream ifs("../../2/input.txt");
    std::string line;
    int64_t sumIds{};

    while (std::getline(ifs, line)) {
        std::stringstream ss(line);
        std::string tmp;
        int32_t gameId;
        ss >> tmp;
        ss >> gameId;
        ss >> tmp;

        std::unordered_map<std::string, int32_t> cnt;
        std::unordered_map<std::string, int32_t> currentCnt;
        while (!ss.eof()) {
            int32_t nr;
            std::string col;
            ss >> nr;
            ss >> col;
            const bool done = col.back() == ';';
            if (col.back() == ';' || col.back() == ',')
                col.pop_back();
            currentCnt[col] += std::max(currentCnt[col], nr);

            for (const auto& c : currentCnt)
                cnt[c.first] = std::max(cnt[c.first], c.second);

            if (done)
                currentCnt.clear();
        }
        int32_t s{1};
        for (const auto& c : cnt)
            s *= c.second;
        sumIds += s;
    }
    std::cout << sumIds << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
