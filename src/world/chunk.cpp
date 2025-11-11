#include <cstdlib> // for rand()
#include <ctime>

#include "fast_noise_lite.h"
#include "world/chunk.hpp"

FastNoiseLite noise;

void Chunk::initializeBlocks() {
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    for (int x = 0; x < constants::WORLD_CHUNK_SIZE; x++) {
        for (int y = 0; y < constants::WORLD_CHUNK_SIZE; y++) {
            for (int z = 0; z < constants::WORLD_CHUNK_SIZE; z++) {
                const glm::vec3 blockPosition = position + glm::vec3(x,y,z);

                const auto noiseValue = noise.GetNoise(static_cast<float>(x),static_cast<float>(y),static_cast<float>(z));

                // Assign block types based on the random noise value
                if (noiseValue > 0.3f) {
                    blocks[x][y][z] = Block(blockPosition, BlockType::GRASS);
                } else if (noiseValue > -0.3f) {
                    blocks[x][y][z] = Block(blockPosition, BlockType::EMPTY);
                } else {
                    blocks[x][y][z] = Block(blockPosition, BlockType::STONE);
                }
            }
        }
    }

    this->generateChunkVertices();
}

void Chunk::generateChunkVertices() {
    std::vector<float> chunkVertices;

    for (int x = 0; x < constants::WORLD_CHUNK_SIZE; x++) {
            for (int y = 0; y < constants::WORLD_CHUNK_SIZE; y++) {
                for (int z = 0; z < constants::WORLD_CHUNK_SIZE; z++) {
                    if (blocks[x][y][z].getType() == BlockType::EMPTY) {
                        continue;
                    }
                    bool drawFront = (z == constants::WORLD_CHUNK_SIZE - 1 || blocks[x][y][z + 1].getType() == BlockType::EMPTY);
                    bool drawBack = (z == 0 || blocks[x][y][z - 1].getType() == BlockType::EMPTY);
                    bool drawTop = (y == constants::WORLD_CHUNK_SIZE - 1 || blocks[x][y + 1][z].getType() == BlockType::EMPTY);
                    bool drawBottom = (y == 0 || blocks[x][y - 1][z].getType() == BlockType::EMPTY);
                    bool drawRight = (x == constants::WORLD_CHUNK_SIZE - 1 || blocks[x + 1][y][z].getType() == BlockType::EMPTY);
                    bool drawLeft = (x == 0 || blocks[x - 1][y][z].getType() == BlockType::EMPTY);

                    BlockRenderer::appendBlockVertices(chunkVertices,
                        blocks[x][y][z].getPosition(), drawFront, drawBack, drawLeft, drawRight, drawTop, drawBottom);

                }
            }
        }

    const auto bufferSize = static_cast<GLsizeiptr>(chunkVertices.size() * sizeof(float));

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, chunkVertices.empty() ? nullptr : chunkVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    vertexCount = static_cast<GLsizei>(chunkVertices.size() / 6);
}


void Chunk::render() const {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}
