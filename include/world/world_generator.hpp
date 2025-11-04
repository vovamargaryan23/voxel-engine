#pragma once
#include <vector>
#include "world/chunk.hpp"

class WorldGenerator {
private:
    std::vector<Chunk> chunks;
public:
    WorldGenerator() = default;
    void generate();
    void render();
};