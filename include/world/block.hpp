#pragma once
#include "block_type.hpp"
#include "glm/vec3.hpp"

class Block {
public:
    bool isTransparent = false;
    BlockType type;
    glm::vec3 coordinate;
};
