#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <numeric>
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

using FlowValues = std::unordered_map<char, std::pair<uint64_t, uint64_t>>;
using FlowRule = std::tuple<char, char, uint64_t, std::string>;
using FlowRules = std::vector<FlowRule>;
using Flow = std::unordered_map<std::string, FlowRules>;

Flow parseInput2() {
    std::ifstream ifs("../../19/input.txt");
    std::string line;
    Flow flow;
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
                flow[label].emplace_back(op, s, v, n);
            }
            flow[label].emplace_back('.', '.', 0, def);
        }
    }
    return flow;
}

uint64_t traverse(const Flow& flow, const FlowRules& flowRules, const FlowValues& flowValues) {
    uint64_t val{};
    auto nextValues = flowValues;
    for (const auto& [op, s, v, n] : flowRules) {
        const auto [oLb, oUb] = nextValues[s];
        auto& [lb, ub] = nextValues[s];
        if (op == '<')
            ub = std::min(ub, v - 1);
        else if (op == '>')
            lb = std::max(lb, v + 1);
        if (n == "A")
            val += std::accumulate(nextValues.begin(), nextValues.end(), static_cast<uint64_t>(1), [](uint64_t acc, const auto& kv) { const auto& [l, u] = kv.second; return acc * (1+u-l); });
        else if (n != "R")
            val += traverse(flow, flow.at(n), nextValues);
        if (op == '<') {
            lb = ub + 1;
            ub = oUb;
        } else if (op == '>') {
            ub = lb - 1;
            lb = oLb;
        }
    }
    return val;
}

void part2() {
    const auto& flow = parseInput2();
    FlowValues init{{'x', {1, 4000}}, {'m', {1, 4000}}, {'a', {1, 4000}}, {'s', {1, 4000}}};
    std::cout << traverse(flow, flow.at("in"), init) << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
