// //
// // Created by James Pickering on 2/15/25.
// //
//
// #include "ChunkEntity.hpp"
// #include "graph.hpp"
// #include "render.hpp"
//
// #include <hotplate/shapes.hpp>
//
// #include <glad/glad.h>
// #include <glm/ext/matrix_transform.hpp>
// #include <glm/glm.hpp>
//
// #include <chrono>
//
// namespace render {
// ChunkEntity::ChunkEntity(Graph* graph, glm::ivec2 relativePos) :
//     _graph{graph},
//     _topLeft{relativePos},
//     _topLeftNorm{relativePos - Graph::globalOffset},
//     _quad{util::Quad<int>{}},
//     _centerBlock{_topLeft * fractal::size - fractal::size / 2} {
//     _quad.size = glm::ivec2{fractal::size, fractal::size};
//     initBuffers();
// }
//
// ChunkEntity::~ChunkEntity() = default;
//
// auto ChunkEntity::writeBuffers() -> void {
//     glBindBuffer(GL_ARRAY_BUFFER, _modelVbo);
//     glBufferData(GL_ARRAY_BUFFER, fractal::size * fractal::size * sizeof(glm::mat4), &_transforms[0], GL_DYNAMIC_DRAW);
//     glBindBuffer(GL_ARRAY_BUFFER, _colorVbo);
//     glBufferData(GL_ARRAY_BUFFER, fractal::size * fractal::size * sizeof(glm::vec4), &_colors[0], GL_DYNAMIC_DRAW);
// }
//
// auto ChunkEntity::initBuffers() -> void {
//     glGenBuffers(1, &_modelVbo);
//     glGenBuffers(1, &_colorVbo);
//
//     glGenVertexArrays(1, &_vao);
//     glBindVertexArray(_vao);
//     glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer::cube());
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
//     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//     glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//     glEnableVertexAttribArray(0);
//     glEnableVertexAttribArray(1);
//     glEnableVertexAttribArray(2);
//
//     glBindBuffer(GL_ARRAY_BUFFER, _modelVbo);
//     glEnableVertexAttribArray(3);
//     glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0);
//     glEnableVertexAttribArray(4);
//     glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(1 * sizeof(glm::vec4)));
//     glEnableVertexAttribArray(5);
//     glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
//     glEnableVertexAttribArray(6);
//     glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));
//     glVertexAttribDivisor(3, 1);
//     glVertexAttribDivisor(4, 1);
//     glVertexAttribDivisor(5, 1);
//     glVertexAttribDivisor(6, 1);
//
//     glBindBuffer(GL_ARRAY_BUFFER, _colorVbo);
//     glEnableVertexAttribArray(7);
//     glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
//     glVertexAttribDivisor(7, 1);
//
//     glBindVertexArray(0);
// }
//
// auto ChunkEntity::isLoaded() const -> bool {
//     return _nextCellToLoad == fractal::numCells;
// }
//
// auto ChunkEntity::getQuad() const -> util::Quad<int> {
//     return _quad;
// }
//
// auto ChunkEntity::loadBlock(unsigned int idx) -> void {
//     // // if (x == 0 || x == fractal::size - 1 || y == 0 || y == fractal::size - 1) {
//     // //     intensity = 3;
//     // //     color = glm::vec4{1.f, 0.f, 0.f, 1.f};
//     // // }
//
//     auto relativeBlockPos = fractal::blockIndexToPos(idx);
//     relativeBlockPos.x = -relativeBlockPos.x;
//
//     const auto topLeftBlockPos = _topLeft * fractal::size;
//     const auto z = -fractal::distance - _graph->getOffsetZ();
//     const auto blockPos = glm::vec3{topLeftBlockPos - relativeBlockPos, z};
//
//     const auto topLeftBlockPosNoShift = _topLeftNorm * fractal::size;
//     const auto fracPos = glm::vec3{(glm::vec2)topLeftBlockPosNoShift - (glm::vec2)relativeBlockPos + _graph->getInitOffset() * (glm::vec2)fractal::size * _graph->getZoom() + (glm::vec2)_graph->getChunkOffset() * (glm::vec2)fractal::size, z};
//
//     const auto point = fractal::calcPoint(fracPos.x / _graph->getZoom(), fracPos.y / _graph->getZoom(), 1.f);
//
//     auto model = glm::mat4{1.f};
//     _transforms[idx] = glm::translate(model, blockPos);
//     _colors[idx] = point.color;
// }
//
// auto ChunkEntity::loadNextBlock() -> void {
//     const auto start = std::chrono::high_resolution_clock::now();
//
//     if (isLoaded()) {
//         throw std::runtime_error("Chunk is already loaded");
//     }
//     loadBlock(_nextCellToLoad++);
//
//     const auto end = std::chrono::high_resolution_clock::now();
//     chunkCalcNextCellRunTime = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
// }
//
// auto ChunkEntity::getVao() const -> unsigned int {
//     return _vao;
// }
//
// }
