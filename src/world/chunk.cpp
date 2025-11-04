#include "world/chunk.hpp"

void Chunk::generateChunkVertices() {
    std::vector<float> chunkVertices;

    for (int x = 0; x < constants::WORLD_CHUNK_SIZE; x++) {
        for (int y = 0; y < constants::WORLD_CHUNK_SIZE; y++) {
            for (int z = 0; z < constants::WORLD_CHUNK_SIZE; z++) {
                const glm::vec3 blockPosition = position + glm::vec3(x, y, z);
                std::vector<float> blockVertices = BlockRenderer::generateBlockVertices(blockPosition);
                chunkVertices.insert(chunkVertices.end(), blockVertices.begin(), blockVertices.end());
            }
        }
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, chunkVertices.size() * sizeof(float), chunkVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Chunk::initializeBlocks() {
    for (int x = 0; x < constants::WORLD_CHUNK_SIZE; x++) {
        for (int y = 0; y < constants::WORLD_CHUNK_SIZE; y++) {
            for (int z = 0; z < constants::WORLD_CHUNK_SIZE; z++) {
                const glm::vec3 blockPosition = position + glm::vec3(x,y,z);
                blocks[x][y][z] = Block(blockPosition, BlockType::GRASS);
            }
        }
    }
}

void Chunk::render() const {
    constexpr int count = constants::WORLD_CHUNK_SIZE * constants::WORLD_CHUNK_SIZE * constants::WORLD_CHUNK_SIZE * 36;
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, count);
}
