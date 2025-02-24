//
// Created by James Pickering on 2/15/25.
//

#include "graph.hpp"
#include "chunk_entity.hpp"
#include "render.hpp"

#include <iostream>

namespace render {
glm::ivec2 Graph::globalOffset = {0, 0};

Graph::Graph(Graph* parent, const float offsetZ, unsigned int level) :
    _parent(parent),
    _level{(int)level},
    _zoom{static_cast<float>(std::pow(2, level))},
    _offsetZ{offsetZ},
    _initOffset{(parent ? parent->_initOffset + (glm::vec2)parent->_chunkOffset / parent->getZoom() : glm::vec2{})} {
    for (auto i = 0; i < fractal::numChunks; ++i) {
        const auto pos = idxToChunkKey(i);
        addChunk(pos);
    }

    std::cout << "Created graph:" << std::endl;
    std::cout << "\tLevel: " << _level << std::endl;
    std::cout << "\tZoom: " << getZoom() << std::endl;
    std::cout << "\tStart offset: " << _initOffset.x << ", " << _initOffset.y << std::endl;
}

Graph::~Graph() = default;

auto Graph::isLoaded() const -> bool {
    return _chunkLoadQueue.empty();
}

auto Graph::getZoom() const -> float {
    return _zoom;
}

auto Graph::getOffsetZ() const -> float {
    return _offsetZ;
}

auto Graph::getChunk(glm::ivec2 pos) const -> ChunkEntity& {
    return *(_chunks.at(pos));
}

auto Graph::getChunks() const -> const std::unordered_map<glm::ivec2, std::unique_ptr<ChunkEntity>>& {
    return _chunks;
}

auto Graph::getLevel() const -> int {
    return _level;
}

auto Graph::getParent() const -> Graph* {
    return _parent;
}

auto Graph::getChunkOffset() const -> glm::vec2 {
    return _chunkOffset;
}

auto Graph::getInitOffset() const -> glm::vec2 {
    return _initOffset;
}

auto Graph::addChunk(glm::ivec2 pos) -> void {
    _chunks[pos] = std::make_unique<ChunkEntity>(*this, pos);
    _chunkLoadQueue.push_back(pos);
}

auto Graph::removeChunk(glm::ivec2 pos) -> void {
    if (const auto itr = std::find(_chunkLoadQueue.begin(), _chunkLoadQueue.end(), pos);
        itr != _chunkLoadQueue.end()) {
        _chunkLoadQueue.erase(itr);
    }
    if (!_chunks.erase(pos)) {
        throw std::runtime_error("Chunk does not exist");
    }
}

auto Graph::removeRow(int row) -> void {
    for (auto col = -fractal::chunkGridSizeHalf.x + globalOffset.x; col < fractal::chunkGridSizeHalf.x + globalOffset.x; ++col) {
        removeChunk(glm::ivec2{col, row});
    }
}

auto Graph::removeColumn(int col) -> void {
    for (auto row = -fractal::chunkGridSizeHalf.y + globalOffset.y; row < fractal::chunkGridSizeHalf.y + globalOffset.y; ++row) {
        removeChunk(glm::ivec2{col, row});
    }
}

auto Graph::addRow(int row) -> void {
    for (auto col = -fractal::chunkGridSizeHalf.x + globalOffset.x; col < fractal::chunkGridSizeHalf.x + globalOffset.x; ++col) {
        addChunk(glm::ivec2{col, row});
    }
}

auto Graph::addColumn(int col) -> void {
    for (auto row = -fractal::chunkGridSizeHalf.y + globalOffset.y; row < fractal::chunkGridSizeHalf.y + globalOffset.y; ++row) {
        addChunk(glm::ivec2{col, row});
    }
}

auto Graph::setZoom(float zoom) -> void {
    _zoom = zoom;
}

auto Graph::shift(const util::Direction direction) -> void {
    switch (direction) {
    case util::Direction::Left:
        --globalOffset.x;
        --_chunkOffset.x;
        removeColumn(globalOffset.x + fractal::chunkGridSizeHalf.x);
        addColumn(globalOffset.x + -fractal::chunkGridSizeHalf.x);

        break;
    case util::Direction::Right:
        ++globalOffset.x;
        ++_chunkOffset.x;
        removeColumn(globalOffset.x + -fractal::chunkGridSizeHalf.x - 1);
        addColumn(globalOffset.x + fractal::chunkGridSizeHalf.x - 1);

        break;
    case util::Direction::Top:
        ++globalOffset.y;
        ++_chunkOffset.y;
        removeRow(globalOffset.y + -fractal::chunkGridSizeHalf.y - 1);
        addRow(globalOffset.y + fractal::chunkGridSizeHalf.y - 1);

        break;
    case util::Direction::Bottom:
        --globalOffset.y;
        --_chunkOffset.y;
        removeRow(globalOffset.y + fractal::chunkGridSizeHalf.y);
        addRow(globalOffset.y + -fractal::chunkGridSizeHalf.y);

        break;
    default:
        break;
    }
}

auto Graph::loadNextChunk() -> void {
    auto& chunk = getChunk(_chunkLoadQueue.back());
    chunk.load();
    _chunkLoadQueue.pop_back();
}

auto Graph::loadAllChunks() -> void {
    while (!isLoaded()) {
        loadNextChunk();
    }
}

auto Graph::idxToChunkKey(int idx) -> glm::ivec2 {
    const auto offset = Graph::globalOffset;
    const auto x = idx / fractal::chunkGridSize.x - fractal::chunkGridSizeHalf.x + offset.x;
    const auto y = idx - (x - offset.x + fractal::chunkGridSizeHalf.x) * fractal::chunkGridSize.x - fractal::chunkGridSizeHalf.y +
                   offset.y;

    return glm::ivec2{x, y};
}

}
