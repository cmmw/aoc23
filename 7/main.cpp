#include <algorithm>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <sstream>
#include <unordered_map>
#include <vector>

enum class Type {
    High = 1,
    One,
    Two,
    Three,
    Full,
    Four,
    Five,
};

using Hand = std::tuple<std::string, int64_t, Type>;
using Hands = std::vector<Hand>;

Hands parseInput() {
    Hands hands;
    std::ifstream ifs("../../7/input.txt");
    std::string card;
    int64_t bid;
    while (ifs >> card) {
        ifs >> bid;
        hands.emplace_back(card, bid, Type::High);
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

void types(Hands& hands, bool countJoker = true) {
    for (auto& [hand, bid, type] : hands) {
        std::unordered_map<char, int32_t> m;
        for (auto c : hand) {
            if (!countJoker && c == 'J') {
                m[c] = 1;
                continue;
            }
            m[c]++;
        }
        const auto val = std::ranges::max_element(m, [](const auto& key1, const auto& key2) { return key1.second < key2.second; })->second;
        const auto pairs = std::ranges::count_if(m, [](const auto& key) { return key.second == 2; });
        if (val == 5)
            type = Type::Five;
        else if (val == 4)
            type = Type::Four;
        else if (val == 3 && pairs == 1)
            type = Type::Full;
        else if (val == 3)
            type = Type::Three;
        else if (pairs == 2)
            type = Type::Two;
        else if (pairs == 1)
            type = Type::One;
        else
            type = Type::High;
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

const std::unordered_map<char, int32_t> cardValue2 = []() {
    std::unordered_map<char, int32_t> r;
    int32_t i = 13;
    for (auto c : "AKQT98765432J")
        r[c] = i--;
    return r;
}();

Type promote(Type type) {
    switch (type) {
    case Type::High:
        return Type::One;
    case Type::One:
        return Type::Three;
    case Type::Two:
        return Type::Full;
    case Type::Three:
    case Type::Full:
        return Type::Four;
    case Type::Four:
    case Type::Five:
        return Type::Five;
    }
    assert(false);
}

void part2() {
    Hands hands = parseInput();
    types(hands, false);
    for (auto& h : hands) {
        auto n = std::ranges::count_if(std::get<0>(h), [](char c) { return c == 'J'; });
        auto& r = std::get<2>(h);
        while (n--)
            r = promote(r);
    }
    std::ranges::sort(hands, [](const Hand& h1, const Hand& h2) {
        const auto c1 = std::get<0>(h1);
        const auto c2 = std::get<0>(h2);
        const auto r1 = std::get<2>(h1);
        const auto r2 = std::get<2>(h2);
        if (r1 != r2)
            return r1 < r2;
        for (int i = 0; i < c1.size(); i++) {
            if (c1[i] != c2[i])
                return cardValue2.at(c1[i]) < cardValue2.at(c2[i]);
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

int main() {
    part1();
    part2();
    return 0;
}
