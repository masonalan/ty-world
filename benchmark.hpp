//
// Created by James Pickering on 2/15/25.
//

#pragma once

#include <chrono>
#include <string>
#include <unordered_map>
#include <vector>

struct BenchmarkInfo {
    std::vector<long long> values{};
    long long avg{};
    std::chrono::high_resolution_clock::time_point start{};
    std::chrono::high_resolution_clock::time_point end{};
};

struct Benchmark {
    static auto start(const std::string& name) -> void;
    static auto end(const std::string& name) -> void;
    static auto print(const std::string& name) -> void;
    static auto printAll() -> void;

private:
    std::unordered_map<std::string, BenchmarkInfo> _map{};
};