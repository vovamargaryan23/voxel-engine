#pragma once
#include <memory>
#include <vector>
#include "glm/glm.hpp"
#include "world/block.hpp"
#include "constants.hpp"

class Chunk {
public:
    static constexpr int SIZE = constants::WORLD_CHUNK_SIZE;

    explicit Chunk(glm::vec3 position);
    ~Chunk();
    Chunk(const Chunk&)            = delete;
    Chunk& operator=(const Chunk&) = delete;
    Chunk(Chunk&&) noexcept;
    Chunk& operator=(Chunk&&) noexcept;

    void renderOpaque()      const;
    void renderTransparent() const;

    // AABB for frustum culling
    glm::vec3 aabbMin() const { return position - glm::vec3(0.5f); }
    glm::vec3 aabbMax() const { return position + glm::vec3(SIZE - 0.5f, SIZE - 0.5f, SIZE - 0.5f); }

    bool isSolidLocal(int lx, int ly, int lz) const;

private:
    std::unique_ptr<Block[]> blocks;
    glm::vec3 position;

    // Opaque mesh
    unsigned int oVAO = 0, oVBO = 0, oEBO = 0;
    long long    oIndexCount = 0;

    // Transparent mesh
    unsigned int tVAO = 0, tVBO = 0, tEBO = 0;
    long long    tIndexCount = 0;

    Block& blockAt(int x, int y, int z) {
        return blocks[x * SIZE * SIZE + y * SIZE + z];
    }
    const Block& blockAt(int x, int y, int z) const {
        return blocks[x * SIZE * SIZE + y * SIZE + z];
    }
    bool isSolidAt(int x, int y, int z) const;

    void initializeBlocks();
    void buildMesh();
    void uploadMesh(unsigned int& vao, unsigned int& vbo, unsigned int& ebo,
                    const std::vector<float>& verts,
                    const std::vector<unsigned int>& indices);
};
