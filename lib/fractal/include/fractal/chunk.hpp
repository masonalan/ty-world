//
// Created by James Pickering on 2/23/25.
//

#pragma once

#include <fractal/fractal.hpp>
#include <iostream>

namespace fractal {

struct Chunk {
    Chunk(const FastComplexDouble bottomLeft, const double zoom) :
        _bottomLeft{bottomLeft},
        _zoom{zoom} {
    }

    auto load() {
        auto i = 0;
        for (auto x = 0; x < ChunkSize; ++x) {
            for (auto y = 0; y < ChunkSize; ++y) {
                const auto c = FastComplexDouble{(_bottomLeft.real + static_cast<double>(x)) / _zoom,
                                                 (_bottomLeft.img + static_cast<double>(y)) / _zoom};

                _points[i++] = calcPoint(c);
            }
        }
    }

    [[nodiscard]] auto points() const -> const auto& {
        return _points;
    }

private:
    std::array<Point, PointsInChunk> _points;
    FastComplexDouble _bottomLeft;
    double _zoom;
};

}