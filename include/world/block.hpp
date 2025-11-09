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
    BlockType getType() const {return this->type;}

};

class BlockRenderer {
    public:
    static void appendBlockVertices(
                          std::vector<float>& vertices,
                          glm::vec3 blockPosition,
                          bool drawFront = true,
                          bool drawBack = true,
                          bool drawLeft = true,
                          bool drawRight = true,
                          bool drawTop = true,
                          bool drawBottom = true) {
    // TODO: Optimize this to use instancing(or other optimization techniques)
    if (drawFront) {
        vertices.insert(vertices.end(), {-0.5f + blockPosition.x, 0.5f + blockPosition.y, 0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});
        vertices.insert(vertices.end(), {0.5f + blockPosition.x, -0.5f + blockPosition.y, 0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});
        vertices.insert(vertices.end(), {0.5f + blockPosition.x, 0.5f + blockPosition.y, 0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});

        vertices.insert(vertices.end(), {-0.5f + blockPosition.x, 0.5f + blockPosition.y, 0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});
        vertices.insert(vertices.end(), {-0.5f + blockPosition.x, -0.5f + blockPosition.y, 0.5f + blockPosition.z, 0.5f, 0.5f, 0.0f});
        vertices.insert(vertices.end(), {0.5f + blockPosition.x, -0.5f + blockPosition.y, 0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});
    }

    if (drawBack) {
        vertices.insert(vertices.end(), {0.5f + blockPosition.x, 0.5f + blockPosition.y, -0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});
        vertices.insert(vertices.end(), {0.5f + blockPosition.x, -0.5f + blockPosition.y, -0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});
        vertices.insert(vertices.end(), {-0.5f + blockPosition.x, 0.5f + blockPosition.y, -0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});

        vertices.insert(vertices.end(), {-0.5f + blockPosition.x, 0.5f + blockPosition.y, -0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});
        vertices.insert(vertices.end(), {0.5f + blockPosition.x, -0.5f + blockPosition.y, -0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});
        vertices.insert(vertices.end(), {-0.5f + blockPosition.x, -0.5f + blockPosition.y, -0.5f + blockPosition.z, 0.5f, 0.5f, 0.0f});
    }

        if (drawLeft) {
            vertices.insert(vertices.end(), {-0.5f + blockPosition.x, 0.5f + blockPosition.y, -0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});
            vertices.insert(vertices.end(), {-0.5f + blockPosition.x, -0.5f + blockPosition.y, -0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});
            vertices.insert(vertices.end(), {-0.5f + blockPosition.x, 0.5f + blockPosition.y, 0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});

            vertices.insert(vertices.end(), {-0.5f + blockPosition.x, -0.5f + blockPosition.y, -0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});
            vertices.insert(vertices.end(), {-0.5f + blockPosition.x, -0.5f + blockPosition.y, 0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});
            vertices.insert(vertices.end(), {-0.5f + blockPosition.x, 0.5f + blockPosition.y, 0.5f + blockPosition.z, 0.5f, 0.5f, 0.0f});
        }

        if (drawRight) {
            vertices.insert(vertices.end(), {0.5f + blockPosition.x, 0.5f + blockPosition.y, 0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});
            vertices.insert(vertices.end(), {0.5f + blockPosition.x, -0.5f + blockPosition.y, 0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});
            vertices.insert(vertices.end(), {0.5f + blockPosition.x, 0.5f + blockPosition.y, -0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});

            vertices.insert(vertices.end(), {0.5f + blockPosition.x, -0.5f + blockPosition.y, 0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});
            vertices.insert(vertices.end(), {0.5f + blockPosition.x, -0.5f + blockPosition.y, -0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});
            vertices.insert(vertices.end(), {0.5f + blockPosition.x, 0.5f + blockPosition.y, -0.5f + blockPosition.z, 0.5f, 0.5f, 0.0f});
        }

        if (drawTop) {
            vertices.insert(vertices.end(), {-0.5f + blockPosition.x, 0.5f + blockPosition.y, 0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});
            vertices.insert(vertices.end(), {0.5f + blockPosition.x, 0.5f + blockPosition.y, 0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});
            vertices.insert(vertices.end(), {0.5f + blockPosition.x, 0.5f + blockPosition.y, -0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});

            vertices.insert(vertices.end(), {-0.5f + blockPosition.x, 0.5f + blockPosition.y, 0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});
            vertices.insert(vertices.end(), {0.5f + blockPosition.x, 0.5f + blockPosition.y, -0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});
            vertices.insert(vertices.end(), {-0.5f + blockPosition.x, 0.5f + blockPosition.y, -0.5f + blockPosition.z, 0.5f, 0.5f, 0.0f});

        }

        if (drawBottom) {
            vertices.insert(vertices.end(), {-0.5f + blockPosition.x, -0.5f + blockPosition.y, -0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});
            vertices.insert(vertices.end(), {0.5f + blockPosition.x, -0.5f + blockPosition.y, -0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});
            vertices.insert(vertices.end(), {0.5f + blockPosition.x, -0.5f + blockPosition.y, 0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});

            vertices.insert(vertices.end(), {-0.5f + blockPosition.x, -0.5f + blockPosition.y, -0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});
            vertices.insert(vertices.end(), {0.5f + blockPosition.x, -0.5f + blockPosition.y, 0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});
            vertices.insert(vertices.end(), {-0.5f + blockPosition.x, -0.5f + blockPosition.y, 0.5f + blockPosition.z, 0.5f, 0.0f, 0.0f});
        }
    }
};