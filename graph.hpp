//
// Created by James Pickering on 2/15/25.
//

#pragma once

#include "direction.hpp"

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

#include <unordered_map>
#include <vector>

struct ChunkEntity;

namespace render {
struct Graph {
    explicit Graph(Graph* parent, float offsetZ, unsigned int level);
    ~Graph();

    auto addChunk(glm::ivec2 pos) -> void;
    auto addColumn(int col) -> void;
    auto addRow(int row) -> void;
    auto removeChunk(glm::ivec2 pos) -> void;
    auto removeColumn(int col) -> void;
    auto removeRow(int row) -> void;
    auto loadNextChunk() -> void;
    auto loadAllChunks() -> void;
    auto setZoom(float zoom) -> void;
    auto shift(util::Direction direction) -> void;

    [[nodiscard]] auto isLoaded() const -> bool;
    [[nodiscard]] auto getZoom() const -> float;
    [[nodiscard]] auto getOffsetZ() const -> float;
    [[nodiscard]] auto getChunk(glm::ivec2 pos) const -> ChunkEntity&;
    [[nodiscard]] auto getChunks() const -> const std::unordered_map<glm::ivec2, std::unique_ptr<ChunkEntity>>&;
    [[nodiscard]] auto getLevel() const -> int;
    [[nodiscard]] auto getParent() const -> Graph*;
    [[nodiscard]] auto getChunkOffset() const -> glm::vec2;
    [[nodiscard]] auto getInitOffset() const -> glm::vec2;

    static glm::ivec2 globalOffset;

private:
    auto idxToChunkKey(int idx) -> glm::ivec2;

    std::unordered_map<glm::ivec2, std::unique_ptr<ChunkEntity>> _chunks;
    std::vector<glm::ivec2> _chunkLoadQueue{};

    glm::vec2 _chunkOffset;
    glm::vec2 _initOffset;

    int _level;
    float _zoom;
    float _offsetZ;

    Graph* _parent;
};
}
