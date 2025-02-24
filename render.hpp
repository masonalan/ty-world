//
// Created by James Pickering on 2/14/25.
//

#pragma once

#include "quad.hpp"

#include <fractal/fractal.hpp>

#include <glm/vec2.hpp>

#include <memory>
#include <vector>

namespace render {

static long long chunkCalcNextCellRunTime = 0;

struct Graph;

struct Scene {
    auto createGraph() -> Graph&;
    auto setCurrGraph(size_t idx) -> void;
    auto zoomInOnce() -> void;
    auto zoomOutOnce() -> void;
    auto updateBounds() -> void;

    [[nodiscard]] auto getCurrentGraph() const -> Graph&;
    [[nodiscard]] auto getBounds() const -> util::Bounds<float>;
    [[nodiscard]] auto isPointPastMaxZ(glm::vec3 point) const -> bool;

private:
    util::Bounds<float> _bounds{};
    std::vector<std::unique_ptr<render::Graph>> _graphs{};
    int _currGraph = 0;
};

}
