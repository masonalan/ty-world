//
// Created by James Pickering on 2/20/25.
//

#include <fractal/gradient.hpp>

namespace fractal {

GradientSection::GradientSection(const GradientPoint& startPoint, const GradientPoint& endPoint) :
    start{startPoint.threshold},
    end{endPoint.threshold},
    m{endPoint.color - startPoint.color},
    b{startPoint.color},
    distance{end - start} {
}

auto Gradient::addPoint(GradientPoint point) -> void {
    point.color.r /= 255.f;
    point.color.g /= 255.f;
    point.color.b /= 255.f;
    _points.emplace_back(point);

    if (_points.size() > 1) {
        const auto& startPoint = _points.at(_points.size() - 2);
        const auto& endPoint = _points.back();

        _sections.emplace_back(startPoint, endPoint);
    }
}

auto Gradient::getSections() const -> const std::vector<GradientSection>& {
    return _sections;
}

}