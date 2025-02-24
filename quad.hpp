//
// Created by James Pickering on 2/15/25.
//

#pragma once

#include "direction.hpp"

#include <glm/glm.hpp>

#include <vector>

namespace util {
template <typename T>
struct Quad {
    glm::vec<2, T> center;
    glm::vec<2, T> size;

    [[nodiscard]] auto left() const -> float {
        return center.x - size.x / static_cast<T>(2);
    }

    [[nodiscard]] auto top() const -> float {
        return center.y + size.y / static_cast<T>(2);
    }

    [[nodiscard]] auto right() const -> float {
        return center.x + size.x / static_cast<T>(2);
    }

    [[nodiscard]] auto bottom() const -> float {
        return center.y - size.y / static_cast<T>(2);
    }
};

template <typename T>
struct Bounds {
    T left, top, right, bottom;

    auto isPointOutside(glm::vec2 point) const -> std::vector<Direction> {
        auto dirs = std::vector<Direction>{};
        if (point.x > right) {
            dirs.emplace_back(Direction::Right);
        }
        if (point.x < left) {
            dirs.emplace_back(Direction::Left);
        }
        if (point.y > top) {
            dirs.emplace_back(Direction::Top);
        }
        if (point.y < bottom) {
            dirs.emplace_back(Direction::Bottom);
        }
        return dirs;
    }
};
}
