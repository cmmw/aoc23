#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <optional>
#include <regex>
#include <sstream>
#include <unordered_map>

const std::regex ruleRegex(R"((\w+)\{((?:\w+(?:<|>)\d+:\w+,?)+)(\w+)\})");

using Item = std::unordered_map<char, int32_t>;
using Items = std::vector<Item>;
using Rule = std::function<std::optional<std::string>(Item)>;
using Workflow = std::vector<Rule>;
using Workflows = std::unordered_map<std::string, Workflow>;
using Instance = std::pair<Items, Workflows>;

template <typename OP>
auto buildRule(const char sel, const int32_t v, const std::string& next) {
    return [=](const Item& item) -> std::optional<std::string> {
        if (OP{}(item.at(sel), v))
            return next;
        return std::nullopt;
    };
}

auto buildDefaultRule(const std::string& next) {
    return [=](const Item& item) -> std::optional<std::string> {
        return next;
    };
}

Instance parseInput() {
    std::ifstream ifs("../../19/input.txt");
    std::string line;
    Workflows workflows;
    Items items;
    while (std::getline(ifs, line)) {
        if (line.empty())
            continue;
        std::smatch res;
        if (std::regex_match(line, res, ruleRegex)) {
            const std::string label = res[1];
            const std::string def = res[res.size() - 1];
            const std::string rules = res[2];
            std::string str;
            std::stringstream ss(rules);
            while (std::getline(ss, str, ',')) {
                const auto s = str.front();
                const auto p0 = str.find_first_of("<>");
                const char op = str[p0];
                const auto p1 = str.find(':');
                const auto v = std::strtol(str.substr(p0 + 1, p1 - p0 - 1).c_str(), nullptr, 10);
                const auto n = str.substr(p1 + 1);
                switch (op) {
                case '<':
                    workflows[label].emplace_back(buildRule<std::less<>>(s, v, n));
                    break;
                case '>':
                    workflows[label].emplace_back(buildRule<std::greater<>>(s, v, n));
                    break;
                default:
                    assert(false);
                }
            }
            workflows[label].emplace_back(buildDefaultRule(def));
        } else {
            char tmp;
            int32_t x, m, a, s;
            std::stringstream ss(line);
            for (auto r : {&x, &m, &a, &s})
                ss >> tmp >> tmp >> tmp >> *r;
            items.push_back({{'x', x}, {'m', m}, {'a', a}, {'s', s}});
        }
    }
    return {items, workflows};
}

enum class Result {
    A,
    R
};

Result processItem(const Item& item, const Workflows& workflows) {
    std::string next = "in";
    while (next != "A" && next != "R") {
        const auto& workflow = workflows.at(next);
        for (const auto& rule : workflow) {
            if (auto opt = rule(item)) {
                next = *opt;
                break;
            }
        }
    }
    if (next == "A")
        return Result::A;
    return Result::R;
}

void part1() {
    int64_t v{};
    const auto& [items, workflows] = parseInput();
    for (const auto& item : items) {
        if (processItem(item, workflows) == Result::A)
            v += item.at('x') + item.at('m') + item.at('a') + item.at('s');
    }
    std::cout << v << std::endl;
}

void part2() {
}

int main() {
    part1();
    part2();
    return 0;
}
