#pragma once
#include "block_type.hpp"

class Block {
public:
    Block() = default;
    explicit Block(BlockType t) : type(t) {}

    BlockType getType()      const { return type; }
    bool      isTransparent() const {
        return type == BlockType::GLASS || type == BlockType::WATER;
    }
    bool      isEmpty()       const { return type == BlockType::EMPTY; }

private:
    BlockType type = BlockType::EMPTY;
};
