#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <unordered_map>
#include <vector>

enum class Rank {
    High = 1,
    One,
    Two,
    Three,
    Full,
    Four,
    Five,
};

using Hand = std::tuple<std::string, int64_t, Rank>;
using Hands = std::vector<Hand>;

Hands parseInput() {
    Hands hands;
    std::ifstream ifs("../../7/input.txt");
    std::string card;
    int64_t bid;
    while (ifs >> card) {
        ifs >> bid;
        hands.emplace_back(card, bid, Rank::High);
    }
    return hands;
}

const std::unordered_map<char, int32_t> cardValue = []() {
    std::unordered_map<char, int32_t> r;
    int32_t i = 13;
    for (auto c : "AKQJT98765432")
        r[c] = i--;
    return r;
}();

void types(Hands& hands) {
    for (auto& [hand, bid, rank] : hands) {
        std::unordered_map<char, int32_t> m;
        for (auto c : hand)
            m[c]++;
        const auto val = std::max_element(m.begin(), m.end(), [](const auto& key1, const auto& key2) { return key1.second < key2.second; })->second;
        const auto pairs = std::count_if(m.begin(), m.end(), [](const auto& key) { return key.second == 2; });
        if (val == 5)
            rank = Rank::Five;
        else if (val == 4)
            rank = Rank::Four;
        else if (val == 3 && pairs == 1)
            rank = Rank::Full;
        else if (val == 3)
            rank = Rank::Three;
        else if (pairs == 2)
            rank = Rank::Two;
        else if (pairs == 1)
            rank = Rank::One;
        else
            rank = Rank::High;
    }
}

void part1() {
    Hands hands = parseInput();
    types(hands);
    std::sort(hands.begin(), hands.end(), [](const Hand& h1, const Hand& h2) {
        const auto c1 = std::get<0>(h1);
        const auto c2 = std::get<0>(h2);
        const auto r1 = std::get<2>(h1);
        const auto r2 = std::get<2>(h2);
        if (r1 != r2)
            return r1 < r2;
        for (int i = 0; i < c1.size(); i++) {
            if (c1[i] != c2[i])
                return cardValue.at(c1[i]) < cardValue.at(c2[i]);
        }
        return false;
    });
    int32_t i = 1;
    int64_t sum{};
    for (const auto& h : hands) {
        sum += i * std::get<1>(h);
        i++;
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
