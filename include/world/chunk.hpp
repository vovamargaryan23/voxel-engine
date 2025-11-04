#pragma once
#include "glm/glm.hpp"

#include "world/block.hpp"
#include "constants.hpp"

class Chunk {
private:
    Block blocks[constants::WORLD_CHUNK_SIZE][constants::WORLD_CHUNK_SIZE][constants::WORLD_CHUNK_SIZE] = {};
    glm::vec3 position;
    void initializeBlocks();
    unsigned int VBO, VAO;

    void generateChunkVertices();
public:
    explicit Chunk(const glm::vec3 position): position(position) { this->initializeBlocks();
                                                                   this->generateChunkVertices(); }
    Chunk(const float x, const float y, const float z) : position(x, y, z) { this->initializeBlocks();
                                                                             this->generateChunkVertices();}

    void render() const;
};