#include "fast_noise_lite.h"
#include "glad/glad.h"
#include "world/chunk.hpp"

Chunk::Chunk(glm::vec3 pos)
    : blocks(std::make_unique<Block[]>(SIZE * SIZE * SIZE)), position(pos) {
    initializeBlocks();
}

Chunk::Chunk(float x, float y, float z)
    : blocks(std::make_unique<Block[]>(SIZE * SIZE * SIZE)), position(x, y, z) {
    initializeBlocks();
}

Chunk::~Chunk() {
    if (VAO) glDeleteVertexArrays(1, &VAO);
    if (VBO) glDeleteBuffers(1, &VBO);
}

Chunk::Chunk(Chunk&& other) noexcept
    : blocks(std::move(other.blocks)), position(other.position),
      VAO(other.VAO), VBO(other.VBO), vertexCount(other.vertexCount) {
    other.VAO = 0;
    other.VBO = 0;
    other.vertexCount = 0;
}

Chunk& Chunk::operator=(Chunk&& other) noexcept {
    if (this != &other) {
        if (VAO) glDeleteVertexArrays(1, &VAO);
        if (VBO) glDeleteBuffers(1, &VBO);
        blocks = std::move(other.blocks);
        position = other.position;
        VAO = other.VAO; VBO = other.VBO; vertexCount = other.vertexCount;
        other.VAO = 0; other.VBO = 0; other.vertexCount = 0;
    }
    return *this;
}

void Chunk::initializeBlocks() {
    FastNoiseLite noise;
    noise.SetSeed(2541225);
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

    for (int x = 0; x < SIZE; x++) {
        for (int y = 0; y < SIZE; y++) {
            for (int z = 0; z < SIZE; z++) {
                const glm::vec3 worldPos = position + glm::vec3(x, y, z);
                const float noiseValue = noise.GetNoise(worldPos.x, worldPos.y, worldPos.z);

                if (noiseValue > 0.3f)
                    blockAt(x, y, z) = Block(BlockType::GRASS);
                else if (noiseValue > -0.3f)
                    blockAt(x, y, z) = Block(BlockType::EMPTY);
                else
                    blockAt(x, y, z) = Block(BlockType::STONE);
            }
        }
    }

    generateChunkVertices();
}

void Chunk::generateChunkVertices() {
    std::vector<float> chunkVertices;
    chunkVertices.reserve(static_cast<size_t>(SIZE) * SIZE * 36 * 4);

    for (int x = 0; x < SIZE; x++) {
        for (int y = 0; y < SIZE; y++) {
            for (int z = 0; z < SIZE; z++) {
                const Block& b = blockAt(x, y, z);
                if (b.getType() == BlockType::EMPTY)
                    continue;

                auto isOccluding = [&](int nx, int ny, int nz) -> bool {
                    const Block& nb = blockAt(nx, ny, nz);
                    return nb.getType() != BlockType::EMPTY && !nb.isTransparent();
                };

                const bool drawFront  = (z == SIZE - 1 || !isOccluding(x, y, z + 1));
                const bool drawBack   = (z == 0        || !isOccluding(x, y, z - 1));
                const bool drawTop    = (y == SIZE - 1 || !isOccluding(x, y + 1, z));
                const bool drawBottom = (y == 0        || !isOccluding(x, y - 1, z));
                const bool drawRight  = (x == SIZE - 1 || !isOccluding(x + 1, y, z));
                const bool drawLeft   = (x == 0        || !isOccluding(x - 1, y, z));

                BlockRenderer::appendBlockVertices(chunkVertices,
                    position + glm::vec3(x, y, z),
                    drawFront, drawBack, drawLeft, drawRight, drawTop, drawBottom);
            }
        }
    }

    const auto bufferSize = static_cast<GLsizeiptr>(chunkVertices.size() * sizeof(float));

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, chunkVertices.empty() ? nullptr : chunkVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void*>(nullptr));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    vertexCount = static_cast<long long>(chunkVertices.size() / 6);
}

void Chunk::render() const {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexCount));
}
