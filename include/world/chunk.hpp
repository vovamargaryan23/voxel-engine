#pragma once
#include <memory>
#include "glm/glm.hpp"

#include "world/block.hpp"
#include "constants.hpp"

class Chunk {
private:
    static constexpr int SIZE = constants::WORLD_CHUNK_SIZE;
    std::unique_ptr<Block[]> blocks;
    glm::vec3 position;
    unsigned int VAO = 0, VBO = 0;
    long long vertexCount = 0;

    void initializeBlocks();
    void generateChunkVertices();

    Block& blockAt(int x, int y, int z) { return blocks[x * SIZE * SIZE + y * SIZE + z]; }
    const Block& blockAt(int x, int y, int z) const { return blocks[x * SIZE * SIZE + y * SIZE + z]; }

public:
    explicit Chunk(glm::vec3 position);
    Chunk(float x, float y, float z);
    ~Chunk();
    Chunk(const Chunk&) = delete;
    Chunk& operator=(const Chunk&) = delete;
    Chunk(Chunk&&) noexcept;
    Chunk& operator=(Chunk&&) noexcept;

    void render() const;
};
