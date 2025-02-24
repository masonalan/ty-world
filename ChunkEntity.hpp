// //
// // Created by James Pickering on 2/15/25.
// //
//
// #pragma once
//
// #include <fractal/fractal.hpp>
//
// #include "quad.hpp"
//
// #include <glm/glm.hpp>
//
// #include <array>
//
// namespace render {
// struct Graph;
//
// struct ChunkEntity {
//     ChunkEntity(Graph* graph, glm::ivec2 pos);
//     ~ChunkEntity();
//
//     auto writeBuffers() -> void;
//     auto load() -> void;
//     // auto loadBlock(unsigned int idx) -> void;
//     // auto loadNextBlock() -> void;
//
//     [[nodiscard]] auto isLoaded() const -> bool;
//     [[nodiscard]] auto getQuad() const -> util::Quad<int>;
//     [[nodiscard]] auto getVao() const -> unsigned int;
//
//     glm::ivec2 _topLeftNorm{};
//
// private:
//     auto initBuffers() -> void;
//
//     glm::ivec2 _topLeft{};
//
//     glm::ivec2 _centerBlock{};
//
//     std::array<glm::mat4, fractal::size * fractal::size> _transforms{};
//     std::array<glm::vec4, fractal::size * fractal::size> _colors{};
//
//     unsigned int _vao{};
//     unsigned int _colorVbo{0};
//     unsigned int _modelVbo{0};
//     unsigned int _nextCellToLoad{0};
//
//     Graph* _graph{};
//     util::Quad<int> _quad;
// };
// }
