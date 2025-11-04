#pragma once
#include <vector>
#include "block_type.hpp"
#include "glm/vec3.hpp"
#include "glad/glad.h"

class Block {
private:
    glm::vec3 position = {0,0,0};
    bool isTransparent_ = false;
    BlockType type = BlockType::EMPTY;

    void adjustTransparency() {
        // TODO: Add general check for other transparent blocks too
        if (type == BlockType::GLASS) {
            isTransparent_ = true;
        }
    }
public:
    Block()  {};
    Block(const glm::vec3 position, const BlockType type_): position(position), type(type_) {this->adjustTransparency();}
    bool isTransparent() const {return this->isTransparent_;}
    glm::vec3 getPosition() const {return this->position;}

};

class BlockRenderer {
    public:
    static std::vector<float> generateBlockVertices(glm::vec3 blockPosition,
                          bool drawFront = true,
                          bool drawBack = true,
                          bool drawLeft = true,
                          bool drawRight = true,
                          bool drawTop = true,
                          bool drawBottom = true) {
    std::vector<float> resultingVertices = {};

    // Front face (z = 0.0f)
    if (drawFront) {
        resultingVertices.insert(resultingVertices.end(),
            {1.0f + blockPosition.x, 1.0f + blockPosition.y, 0.0f + blockPosition.z, 1.0f, 0.0f, 0.0f}); // front-up-right 0
        resultingVertices.insert(resultingVertices.end(),
            {1.0f + blockPosition.x, -1.0f + blockPosition.y, 0.0f + blockPosition.z, 1.0f, 0.0f, 1.0f}); // front-down-right 1
        resultingVertices.insert(resultingVertices.end(),
            {-1.0f + blockPosition.x, 1.0f + blockPosition.y, 0.0f + blockPosition.z, 1.0f, 0.0f, 0.0f}); // front-up-left 2
        resultingVertices.insert(resultingVertices.end(),
            {-1.0f + blockPosition.x, -1.0f + blockPosition.y, 0.0f + blockPosition.z, 1.0f, 1.0f, 0.0f}); // front-down-left 3
    }

    // Back face (z = -1.0f)
    if (drawBack) {
        resultingVertices.insert(resultingVertices.end(),
            {1.0f + blockPosition.x, 1.0f + blockPosition.y, -1.0f + blockPosition.z, 1.0f, 0.0f, 0.0f}); // back-up-right 4
        resultingVertices.insert(resultingVertices.end(),
            {1.0f + blockPosition.x, -1.0f + blockPosition.y, -1.0f + blockPosition.z, 1.0f, 0.0f, 1.0f}); // back-down-right 5
        resultingVertices.insert(resultingVertices.end(),
            {-1.0f + blockPosition.x, 1.0f + blockPosition.y, -1.0f + blockPosition.z, 1.0f, 0.0f, 0.0f}); // back-up-left 6
        resultingVertices.insert(resultingVertices.end(),
            {-1.0f + blockPosition.x, -1.0f + blockPosition.y, -1.0f + blockPosition.z, 1.0f, 1.0f, 0.0f}); // back-down-left 7
    }

    // Left face (x = -1.0f)
    if (drawLeft) {
        resultingVertices.insert(resultingVertices.end(),
            {-1.0f + blockPosition.x, 1.0f + blockPosition.y, 1.0f + blockPosition.z, 0.0f, 0.0f, 1.0f}); // left-up-right 12
        resultingVertices.insert(resultingVertices.end(),
            {-1.0f + blockPosition.x, -1.0f + blockPosition.y, 1.0f + blockPosition.z, 0.0f, 0.0f, 1.0f}); // left-down-right 13
        resultingVertices.insert(resultingVertices.end(),
            {-1.0f + blockPosition.x, 1.0f + blockPosition.y, -1.0f + blockPosition.z, 0.0f, 0.0f, 1.0f}); // left-up-left 6
        resultingVertices.insert(resultingVertices.end(),
            {-1.0f + blockPosition.x, -1.0f + blockPosition.y, -1.0f + blockPosition.z, 0.0f, 0.0f, 1.0f}); // left-down-left 7
    }

    // Right face (x = 1.0f)
    if (drawRight) {
        resultingVertices.insert(resultingVertices.end(),
            {1.0f + blockPosition.x, 1.0f + blockPosition.y, 1.0f + blockPosition.z, 0.0f, 0.0f, 1.0f}); // right-up-right 14
        resultingVertices.insert(resultingVertices.end(),
            {1.0f + blockPosition.x, -1.0f + blockPosition.y, 1.0f + blockPosition.z, 0.0f, 0.0f, 1.0f}); // right-down-right 15
        resultingVertices.insert(resultingVertices.end(),
            {1.0f + blockPosition.x, 1.0f + blockPosition.y, -1.0f + blockPosition.z, 0.0f, 0.0f, 1.0f}); // right-up-left 4
        resultingVertices.insert(resultingVertices.end(),
            {1.0f + blockPosition.x, -1.0f + blockPosition.y, -1.0f + blockPosition.z, 0.0f, 0.0f, 1.0f}); // right-down-left 5
    }

    // Top face (y = 1.0f)
    if (drawTop) {
        resultingVertices.insert(resultingVertices.end(),
            {1.0f + blockPosition.x, 1.0f + blockPosition.y, 1.0f + blockPosition.z, 0.0f, 1.0f, 0.0f}); // top-up-right 8
        resultingVertices.insert(resultingVertices.end(),
            {-1.0f + blockPosition.x, 1.0f + blockPosition.y, 1.0f + blockPosition.z, 0.0f, 1.0f, 0.0f}); // top-up-left 9
        resultingVertices.insert(resultingVertices.end(),
            {1.0f + blockPosition.x, 1.0f + blockPosition.y, -1.0f + blockPosition.z, 0.0f, 1.0f, 0.0f}); // top-down-right 4
        resultingVertices.insert(resultingVertices.end(),
            {-1.0f + blockPosition.x, 1.0f + blockPosition.y, -1.0f + blockPosition.z, 0.0f, 1.0f, 0.0f}); // top-down-left 6
    }

    // Bottom face (y = -1.0f)
    if (drawBottom) {
        resultingVertices.insert(resultingVertices.end(),
            {1.0f + blockPosition.x, -1.0f + blockPosition.y, 1.0f + blockPosition.z, 0.0f, 1.0f, 0.0f}); // bottom-up-right 10
        resultingVertices.insert(resultingVertices.end(),
            {-1.0f + blockPosition.x, -1.0f + blockPosition.y, 1.0f + blockPosition.z, 0.0f, 1.0f, 0.0f}); // bottom-up-left 11
        resultingVertices.insert(resultingVertices.end(),
            {1.0f + blockPosition.x, -1.0f + blockPosition.y, -1.0f + blockPosition.z, 0.0f, 1.0f, 0.0f}); // bottom-down-right 4
        resultingVertices.insert(resultingVertices.end(),
            {-1.0f + blockPosition.x, -1.0f + blockPosition.y, -1.0f + blockPosition.z, 0.0f, 1.0f, 0.0f}); // bottom-down-left 6
    }

    return resultingVertices;
}
};