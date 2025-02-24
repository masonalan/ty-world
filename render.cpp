//
// Created by James Pickering on 2/15/25.
//

#include "render.hpp"
#include "fractal/chunk.hpp"
#include "graph.hpp"

#include <fractal/fractal.hpp>

namespace render {

auto Scene::createGraph() -> Graph& {
    const auto offsetZ = fractal::ReloadGraphThreshold * static_cast<float>(_graphs.size());
    auto parent = _graphs.empty() ? nullptr : _graphs.back().get();
    return *_graphs.emplace_back(std::make_unique<render::Graph>(parent, offsetZ, parent ? parent->getLevel() + 1 : 0));
}

auto Scene::setCurrGraph(size_t idx) -> void {
    _currGraph = idx;
}

auto Scene::zoomInOnce() -> void {
    ++_currGraph;
}

auto Scene::zoomOutOnce() -> void {
    --_currGraph;
}

auto Scene::updateBounds() -> void {
    _bounds.right = static_cast<float>((Graph::globalOffset.x + 1) * fractal::ChunkSize);
    _bounds.left = static_cast<float>((Graph::globalOffset.x - 1) * fractal::ChunkSize);
    _bounds.top = static_cast<float>((Graph::globalOffset.y + 1) * fractal::ChunkSize);
    _bounds.bottom = static_cast<float>((Graph::globalOffset.y - 1) * fractal::ChunkSize);
}

auto Scene::getCurrentGraph() const -> Graph& {
    return *_graphs[_currGraph];
}

auto Scene::getBounds() const -> util::Bounds<float> {
    return _bounds;
}

auto Scene::isPointPastMaxZ(glm::vec3 point) const -> bool {
    const auto absZ = std::abs(getCurrentGraph().getOffsetZ());
    const auto absPos = std::abs(point.z);
    return absZ + fractal::ReloadGraphThreshold < absPos ||
           absZ - fractal::ReloadGraphThreshold > absPos;
}

}