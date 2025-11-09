#include "world/world_generator.hpp"

void WorldGenerator::generate() {
    glm::vec3 startPosition = {0,0,0};
    chunks.emplace_back(startPosition);
}

void WorldGenerator::render() {
    for (Chunk& chunk : chunks) {
        chunk.render();
    }
}
