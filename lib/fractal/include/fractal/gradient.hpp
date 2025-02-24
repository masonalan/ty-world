//
// Created by James Pickering on 2/20/25.
//

#pragma once

#include <glm/glm.hpp>

#include <vector>

namespace fractal {

struct GradientPoint {
    float threshold;

    glm::vec4 color;
};

struct GradientSection {
    GradientSection(const GradientPoint& startPoint, const GradientPoint& endPoint);

    float start;
    float end;
    float distance;

    glm::vec4 m;
    glm::vec4 b;
};

struct Gradient {
    auto addPoint(GradientPoint point) -> void;

    [[nodiscard]] auto getSections() const -> const std::vector<GradientSection>&;

private:
    std::vector<GradientPoint> _points;
    std::vector<GradientSection> _sections;
};

// namespace cexp {
//
// struct GradientPoint {
//     float threshold;
//
//     glm::vec4 color;
// };
//
// struct GradientSection {
//     consteval GradientSection() = default;
//     consteval GradientSection(const GradientPoint& startPoint, const GradientPoint& endPoint) :
//         start{startPoint.threshold},
//         end{endPoint.threshold},
//         m{endPoint.color - startPoint.color},
//         b{startPoint.color},
//         distance{end - start} {};
//
//     float start;
//     float end;
//     float distance;
//
//     glm::vec4 m;
//     glm::vec4 b;
// };
//
// template <int NumPoints>
// struct Gradient {
//     consteval Gradient() = default;
//
//     consteval auto addPoint(GradientPoint point) {
//         _points[_currPoint] = point;
//
//         if (_points.size() > 1) {
//             const auto& startPoint = _points.at(_points.size() - 2);
//             const auto& endPoint = _points.back();
//
//             _sections[_currSection] = {startPoint, endPoint};
//             ++_currSection;
//         }
//
//         ++_currPoint;
//     };
//
//     [[nodiscard]] consteval auto getSections() const -> const std::array<GradientSection, NumPoints - 1>& {
//         return _sections;
//     }
//
// private:
//     unsigned int _currPoint{0};
//     unsigned int _currSection{0};
//     std::array<GradientPoint, NumPoints> _points;
//     std::array<GradientSection, NumPoints - 1> _sections;
// };
//
// }

}