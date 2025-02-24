//
// Created by James Pickering on 2/15/25.
//

#include "benchmark.hpp"

#include <iostream>

static auto b = Benchmark{};

auto Benchmark::start(const std::string& name) -> void {
    if (!b._map.contains(name)) {
        b._map.emplace(name, BenchmarkInfo{});
    }
    b._map[name].start = std::chrono::high_resolution_clock::now();
}

auto Benchmark::end(const std::string& name) -> void {
    b._map[name].end = std::chrono::high_resolution_clock::now();
    b._map[name].values.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(b._map[name].end - b._map[name].start).count());

    if (b._map[name].values.size() > 100) {
        b._map[name].values.erase(b._map[name].values.begin());
    }

    auto sum = (long long){};
    for (auto v : b._map[name].values) {
        sum += v;
    }
    b._map[name].avg = sum / b._map[name].values.size();
}

auto Benchmark::print(const std::string& name) -> void {
    std::cout << "====== Benchmark: " << name << " ======" << std::endl;
    const auto& info = b._map[name];
    std::cout << "Average Time (ns): " << info.avg << std::endl;
    std::cout << std::endl;
}

auto Benchmark::printAll() -> void {
    for (const auto& [name, _] : b._map) {
        print(name);
    }
}
