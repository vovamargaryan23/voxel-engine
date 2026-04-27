#pragma once
#include <vector>
#include "block_type.hpp"
#include "glm/vec3.hpp"

class Block {
private:
    bool isTransparent_ = false;
    BlockType type = BlockType::EMPTY;

    void adjustTransparency() {
        if (type == BlockType::GLASS)
            isTransparent_ = true;
    }
public:
    Block() = default;
    explicit Block(BlockType type_) : type(type_) { adjustTransparency(); }
    bool isTransparent() const { return isTransparent_; }
    BlockType getType() const { return type; }
};

class BlockRenderer {
public:
    static void appendBlockVertices(
        std::vector<float>& vertices,
        glm::vec3 p,
        bool drawFront = true,
        bool drawBack = true,
        bool drawLeft = true,
        bool drawRight = true,
        bool drawTop = true,
        bool drawBottom = true)
    {
        const float x = p.x, y = p.y, z = p.z;

        if (drawFront) {
            const float f[] = {
                -0.5f+x,  0.5f+y,  0.5f+z,  0.0f,  0.0f,  1.0f,
                 0.5f+x, -0.5f+y,  0.5f+z,  0.0f,  0.0f,  1.0f,
                 0.5f+x,  0.5f+y,  0.5f+z,  0.0f,  0.0f,  1.0f,
                -0.5f+x,  0.5f+y,  0.5f+z,  0.0f,  0.0f,  1.0f,
                -0.5f+x, -0.5f+y,  0.5f+z,  0.0f,  0.0f,  1.0f,
                 0.5f+x, -0.5f+y,  0.5f+z,  0.0f,  0.0f,  1.0f,
            };
            vertices.insert(vertices.end(), f, f + 36);
        }
        if (drawBack) {
            const float f[] = {
                 0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,
                 0.5f+x, -0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,
                -0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,
                -0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,
                 0.5f+x, -0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,
                -0.5f+x, -0.5f+y, -0.5f+z,  0.0f,  0.0f, -1.0f,
            };
            vertices.insert(vertices.end(), f, f + 36);
        }
        if (drawLeft) {
            const float f[] = {
                -0.5f+x,  0.5f+y, -0.5f+z, -1.0f,  0.0f,  0.0f,
                -0.5f+x, -0.5f+y, -0.5f+z, -1.0f,  0.0f,  0.0f,
                -0.5f+x,  0.5f+y,  0.5f+z, -1.0f,  0.0f,  0.0f,
                -0.5f+x, -0.5f+y, -0.5f+z, -1.0f,  0.0f,  0.0f,
                -0.5f+x, -0.5f+y,  0.5f+z, -1.0f,  0.0f,  0.0f,
                -0.5f+x,  0.5f+y,  0.5f+z, -1.0f,  0.0f,  0.0f,
            };
            vertices.insert(vertices.end(), f, f + 36);
        }
        if (drawRight) {
            const float f[] = {
                 0.5f+x,  0.5f+y,  0.5f+z,  1.0f,  0.0f,  0.0f,
                 0.5f+x, -0.5f+y,  0.5f+z,  1.0f,  0.0f,  0.0f,
                 0.5f+x,  0.5f+y, -0.5f+z,  1.0f,  0.0f,  0.0f,
                 0.5f+x, -0.5f+y,  0.5f+z,  1.0f,  0.0f,  0.0f,
                 0.5f+x, -0.5f+y, -0.5f+z,  1.0f,  0.0f,  0.0f,
                 0.5f+x,  0.5f+y, -0.5f+z,  1.0f,  0.0f,  0.0f,
            };
            vertices.insert(vertices.end(), f, f + 36);
        }
        if (drawTop) {
            const float f[] = {
                -0.5f+x,  0.5f+y,  0.5f+z,  0.0f,  1.0f,  0.0f,
                 0.5f+x,  0.5f+y,  0.5f+z,  0.0f,  1.0f,  0.0f,
                 0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  1.0f,  0.0f,
                -0.5f+x,  0.5f+y,  0.5f+z,  0.0f,  1.0f,  0.0f,
                 0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  1.0f,  0.0f,
                -0.5f+x,  0.5f+y, -0.5f+z,  0.0f,  1.0f,  0.0f,
            };
            vertices.insert(vertices.end(), f, f + 36);
        }
        if (drawBottom) {
            const float f[] = {
                -0.5f+x, -0.5f+y, -0.5f+z,  0.0f, -1.0f,  0.0f,
                 0.5f+x, -0.5f+y, -0.5f+z,  0.0f, -1.0f,  0.0f,
                 0.5f+x, -0.5f+y,  0.5f+z,  0.0f, -1.0f,  0.0f,
                -0.5f+x, -0.5f+y, -0.5f+z,  0.0f, -1.0f,  0.0f,
                 0.5f+x, -0.5f+y,  0.5f+z,  0.0f, -1.0f,  0.0f,
                -0.5f+x, -0.5f+y,  0.5f+z,  0.0f, -1.0f,  0.0f,
            };
            vertices.insert(vertices.end(), f, f + 36);
        }
    }
};
