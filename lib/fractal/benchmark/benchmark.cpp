//
// Created by James Pickering on 2/21/25.
//

#include "fractal/chunk.hpp"

#include <fractal/fractal.hpp>
#include <iostream>

#include <benchmark/benchmark.h>

using namespace fractal;

static auto bm_calcNextZStdPow(benchmark::State& state) -> void {
    for (auto _ : state) {
        benchmark::DoNotOptimize(calcNextZStdPow(ComplexDouble{}, ComplexDouble{-1, 0.7}));
    }
}

BENCHMARK(bm_calcNextZStdPow);

static auto bm_calcNextZ(benchmark::State& state) -> void {
    for (auto _ : state) {
        benchmark::DoNotOptimize(calcNextZ(ComplexDouble{}, ComplexDouble{-1, 0.7}));
    }
}

BENCHMARK(bm_calcNextZ);

static auto bm_calcNextZFast(benchmark::State& state) -> void {
    for (auto _ : state) {
        benchmark::DoNotOptimize(calcNextZFast(FastComplexDouble{}, FastComplexDouble{-1, 0.7}));
    }
}

BENCHMARK(bm_calcNextZFast);

static auto bm_calcNextZFaster(benchmark::State& state) -> void {
    constexpr auto c = FasterComplexDouble{};
    for (auto _ : state) {
        benchmark::DoNotOptimize(calcNextZFaster(c, FasterComplexDouble{-1, 0.7}));
    }
}

BENCHMARK(bm_calcNextZFaster);

static auto bm_calcNZsInSet(benchmark::State& state) -> void {
    for (auto _ : state) {
        benchmark::DoNotOptimize(calcNZs(ComplexDouble{0, 0}, ComplexDouble{0, 0}, Iterations));
    }
}

BENCHMARK(bm_calcNZsInSet);

static auto bm_calcNZsInSetFaster(benchmark::State& state) -> void {
    for (auto _ : state) {
        benchmark::DoNotOptimize(calcNZsFaster(FastComplexDouble{0, 0}, FastComplexDouble{0, 0}));
    }
}

BENCHMARK(bm_calcNZsInSetFaster);

static auto bm_calcNZsOutOfSet(benchmark::State& state) -> void {
    for (auto _ : state) {
        auto c = calcNZs(ComplexDouble{0, 0}, ComplexDouble{-1, -0.3}, Iterations);
        benchmark::DoNotOptimize(c);
    }
}

BENCHMARK(bm_calcNZsOutOfSet);

static auto bm_calcNZsOutOfSetFaster(benchmark::State& state) -> void {
    for (auto _ : state) {
        benchmark::DoNotOptimize(calcNZsFaster(FastComplexDouble{0, 0}, FastComplexDouble{-1, -0.3}));
    }
}

BENCHMARK(bm_calcNZsOutOfSetFaster);

static auto bm_calcPointOutOfSetFaster(benchmark::State& state) -> void {
    for (auto _ : state) {
        benchmark::DoNotOptimize(calcPoint({-1, -0.3}));
    }
}

BENCHMARK(bm_calcPointOutOfSetFaster);

static auto bm_calcPointInSetFaster(benchmark::State& state) -> void {
    for (auto _ : state) {
        benchmark::DoNotOptimize(calcPoint({0, 0}));
    }
}

BENCHMARK(bm_calcPointInSetFaster);

static auto bm_constructPointOutOfSet(benchmark::State& state) -> void {
    for (auto _ : state) {
        benchmark::DoNotOptimize(Point{fractal::Iterations / 3});
    }
}

BENCHMARK(bm_constructPointOutOfSet);

static auto bm_constructPointInSet(benchmark::State& state) -> void {
    for (auto _ : state) {
        benchmark::DoNotOptimize(Point{fractal::Iterations});
    }
}

BENCHMARK(bm_constructPointInSet);

static auto bm_loadChunk(benchmark::State& state) -> void {
    auto chunk = Chunk{{-1, 0}, 1};
    for (auto _ : state) {
        chunk.load();
    }
}

BENCHMARK(bm_loadChunk);

BENCHMARK_MAIN();