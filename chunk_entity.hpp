//
// Created by James Pickering on 2/23/25.
//

#pragma once

#include <fractal/chunk.hpp>

#include <glm/vec2.hpp>

namespace render {
struct Graph;
}

using namespace render;

struct ChunkEntity {
    ChunkEntity(Graph& graph, glm::ivec2 bottomLeft);

    auto load() -> void;

    [[nodiscard]] auto getVao() const -> unsigned int;
    [[nodiscard]] auto getDescription() const -> std::string;

private:
    auto initBuffers() -> void;
    auto writeBuffers() const -> void;

    glm::ivec2 _bottomLeft;
    fractal::Chunk _chunk;

    std::array<glm::mat4, fractal::PointsInChunk> _transforms{};
    std::array<glm::vec4, fractal::PointsInChunk> _colors{};

    unsigned int _vao{};
    unsigned int _colorVbo{0};
    unsigned int _modelVbo{0};

    Graph& _graph;
};