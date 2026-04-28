#pragma once
#include <unordered_map>
#include <glm/glm.hpp>
#include "world/chunk.hpp"

class WorldGenerator {
public:
    WorldGenerator() = default;

    void update(const glm::vec3& cameraPos);
    void render(const glm::mat4& vpMatrix);
    bool isSolid(int wx, int wy, int wz) const;

private:
    struct ChunkKey {
        int x, z;
        bool operator==(const ChunkKey& o) const { return x == o.x && z == o.z; }
    };
    struct ChunkKeyHash {
        size_t operator()(const ChunkKey& k) const {
            return std::hash<int>()(k.x) ^ (std::hash<int>()(k.z) * 2654435761u);
        }
    };
    std::unordered_map<ChunkKey, Chunk, ChunkKeyHash> chunks;

    // Frustum culling
    struct Frustum { glm::vec4 planes[6]; };
    static Frustum extractFrustum(const glm::mat4& vp);
    static bool    aabbInFrustum(const Frustum& f, glm::vec3 mn, glm::vec3 mx);
};
