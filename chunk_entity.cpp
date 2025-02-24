//
// Created by James Pickering on 2/23/25.
//

#include "chunk_entity.hpp"

#include "graph.hpp"

#include <fractal/chunk.hpp>
#include <hotplate/shapes.hpp>

#include <glad/glad.h>
#include <glm/ext/matrix_transform.hpp>
#include <glm/vec2.hpp>

ChunkEntity::ChunkEntity(Graph& graph, const glm::ivec2 bottomLeft) :
    _graph{graph},
    _bottomLeft{bottomLeft},
    _chunk{[&bottomLeft, &graph]() {
        const auto bottomLeftBlockPosNoShift = (bottomLeft - Graph::globalOffset) * fractal::ChunkSize;
        const auto fracPos = glm::vec2{(glm::vec2)bottomLeftBlockPosNoShift + graph.getInitOffset() * (glm::vec2)fractal::ChunkSize * graph.getZoom() + (glm::vec2)graph.getChunkOffset() * (glm::vec2)fractal::ChunkSize};

        return fractal::Chunk{{(double)fracPos.x, (double)fracPos.y},
                              static_cast<double>(fractal::ZoomFactor * graph.getZoom())};
    }()} {
    initBuffers();
}

auto ChunkEntity::load() -> void {
    _chunk.load();

    static constexpr auto model = glm::mat4{1.f};

    auto i = 0;
    for (auto x = 0; x < fractal::ChunkSize; ++x) {
        for (auto y = 0; y < fractal::ChunkSize; ++y) {
            const auto pos = glm::vec2{static_cast<double>(_bottomLeft.x) * fractal::ChunkSize + x,
                                       static_cast<double>(_bottomLeft.y) * fractal::ChunkSize + y};

            const auto m = glm::translate(model, {pos, -fractal::GraphDistance - fractal::ReloadGraphThreshold * _graph.getLevel()});
            _transforms[i] = glm::scale(m, {1.f, 1.f, _chunk.points().at(i).intensity});
            _colors[i] = _chunk.points().at(i).color;
            ++i;
        }
    }

    writeBuffers();
}

auto ChunkEntity::getVao() const -> unsigned int {
    return _vao;
}

auto ChunkEntity::getDescription() const -> std::string {
    return "Chunk Entity:\n"
           "\t_bottomLeft: " +
           std::to_string(_bottomLeft.x) + ", " + std::to_string(_bottomLeft.y);
}

auto ChunkEntity::initBuffers() -> void {
    glGenBuffers(1, &_modelVbo);
    glGenBuffers(1, &_colorVbo);

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer::cube());
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, _modelVbo);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(1 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);

    glBindBuffer(GL_ARRAY_BUFFER, _colorVbo);
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
    glVertexAttribDivisor(7, 1);

    glBindVertexArray(0);
}

auto ChunkEntity::writeBuffers() const -> void {
    glBindBuffer(GL_ARRAY_BUFFER, _modelVbo);
    glBufferData(GL_ARRAY_BUFFER, fractal::PointsInChunk * sizeof(glm::mat4), &_transforms[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, _colorVbo);
    glBufferData(GL_ARRAY_BUFFER, fractal::PointsInChunk * sizeof(glm::vec4), &_colors[0], GL_DYNAMIC_DRAW);
}