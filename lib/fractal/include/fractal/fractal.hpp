//
// Created by James Pickering on 2/13/25.
//

#pragma once

#include <fractal/gradient.hpp>

#include <glm/glm.hpp>

#include <complex>
#include <map>

namespace fractal {

constexpr auto Iterations = 500;
constexpr auto ChunkSize = 70;
constexpr auto PointsInChunk = ChunkSize * ChunkSize;
constexpr auto chunkGridSize = glm::ivec2{20, 20};
constexpr auto GraphSize = ChunkSize * chunkGridSize.x;
constexpr auto ZoomFactor = GraphSize / 4;
constexpr auto chunkGridSizeHalf = chunkGridSize / 2;
constexpr auto numChunks = chunkGridSize.x * chunkGridSize.y;
constexpr auto GraphDistance = ChunkSize * chunkGridSize.x;
constexpr auto ReloadGraphThreshold = GraphDistance / 2;

struct FastComplexDouble {
    double real;
    double img;

    auto operator*(const auto& other) const {
        return FastComplexDouble{real * other.real + -img * other.img, real * other.img + img * other.real};
    }

    auto operator+(const auto& other) const {
        return FastComplexDouble{real + other.real, img + other.img};
    }
};

struct FasterComplexDouble {
    double real;
    double img;

    auto operator*(const auto& other) -> auto& {
        real = real * other.real - img * other.img;
        img = real * other.img + img * other.real;
        return *this;
    }

    auto operator+(const auto& other) -> auto& {
        real += other.real;
        img += other.img;
        return *this;
    }
};

using ComplexDouble = std::complex<double>;

static auto defaultGradient() {
    auto g = Gradient{};

    g.addPoint({0.f, {1.f, 1.f, 1.f, 1.f}});// White

    g.addPoint({0.16f, {0, 7, 100, 1}});        // Blue red
    g.addPoint({0.42f, {32, 107, 203, 1.f}});   // Red
    g.addPoint({0.6425f, {237, 255, 255, 1.f}});// Yellow
    g.addPoint({0.8575f, {0, 2, 0, 1.f}});

    return g;
};

static const auto gradient = defaultGradient();

struct Point {
    Point() :
        color{},
        intensity{} {
    }

    explicit Point(const int itrUsed) :
        color{},
        intensity{} {
        // if (itrUsed == Iterations) {
        //     color = {0.0, 0.007843, 0.0, 1.f};
        //     intensity = 0.2;
        //     return;
        // }

        const auto f = static_cast<float>(itrUsed) / static_cast<float>(Iterations);

        for (const auto& s : gradient.getSections()) {
            if (f >= s.start && f < s.end) {
                const auto distFromStart = f - s.start;
                const auto x = distFromStart / s.distance;

                color = s.m * x * x * x + s.b;
                break;
            }
        }

        intensity = ((double)itrUsed / static_cast<double>(Iterations)) * 300.;
    }

    glm::vec4 color;
    double intensity;
};

inline auto calcNextZFast(const FastComplexDouble z, const FastComplexDouble c) {
    return z * z + c;
}

inline auto calcNextZFaster(FasterComplexDouble z, const FasterComplexDouble c) {
    z* z + c;
    return z;
}

inline auto calcNextZ(const ComplexDouble z, const ComplexDouble c) {
    return z * z + c;
}

inline auto calcNextZStdPow(const ComplexDouble z, const ComplexDouble c) {
    return std::pow(z, 2) + c;
}

inline auto calcNZs(ComplexDouble z, ComplexDouble c, unsigned int n) {
    auto i = 0;
    for (; i < n && z.real() < 2 && z.imag() < 2; i++) {
        z = calcNextZ(z, c);
    }
    return std::pair(z, i);
}

inline auto calcNZsFaster(FastComplexDouble z, const FastComplexDouble c) {
    auto i = 0;
    for (; i < Iterations && std::abs(z.real) < 2 && std::abs(z.img) < 2; i++) {
        z = calcNextZFast(z, c);
    }
    return i;
}

inline auto calcPoint(FastComplexDouble point) {
    constexpr auto z = FastComplexDouble{0, 0};
    const auto i = calcNZsFaster(z, point);
    return Point{i};
}

}
