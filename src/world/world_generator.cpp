#include "world/world_generator.hpp"
#include "constants.hpp"
#include "glad/glad.h"
#include <cmath>
#include <algorithm>

// ---------------------------------------------------------------------------
// Streaming: load chunks within VIEW_DISTANCE, unload those beyond
// ---------------------------------------------------------------------------
void WorldGenerator::update(const glm::vec3& camPos) {
    const int S  = constants::WORLD_CHUNK_SIZE;
    const int VD = constants::VIEW_DISTANCE;

    const int cx = static_cast<int>(std::floor(camPos.x / S));
    const int cz = static_cast<int>(std::floor(camPos.z / S));

    // Evict far chunks
    for (auto it = chunks.begin(); it != chunks.end(); ) {
        if (std::max(std::abs(it->first.x - cx), std::abs(it->first.z - cz)) > VD + 1)
            it = chunks.erase(it);
        else
            ++it;
    }

    // Generate missing chunks (nearest first)
    for (int r = 0; r <= VD; r++) {
        for (int dx = -r; dx <= r; dx++) {
        for (int dz = -r; dz <= r; dz++) {
            if (std::abs(dx) != r && std::abs(dz) != r) continue; // shell only
            ChunkKey key{ cx + dx, cz + dz };
            if (chunks.count(key) == 0)
                chunks.emplace(key, Chunk(glm::vec3((cx+dx)*S, 0, (cz+dz)*S)));
        }}
    }
}

// ---------------------------------------------------------------------------
// Render with frustum culling — opaque pass then transparent pass
// ---------------------------------------------------------------------------
void WorldGenerator::render(const glm::mat4& vpMatrix) {
    const Frustum f = extractFrustum(vpMatrix);

    // Opaque pass
    for (auto& [key, chunk] : chunks) {
        if (aabbInFrustum(f, chunk.aabbMin(), chunk.aabbMax()))
            chunk.renderOpaque();
    }

    // Transparent pass
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    for (auto& [key, chunk] : chunks) {
        if (aabbInFrustum(f, chunk.aabbMin(), chunk.aabbMax()))
            chunk.renderTransparent();
    }
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

// ---------------------------------------------------------------------------
// Collision query
// ---------------------------------------------------------------------------
bool WorldGenerator::isSolid(int wx, int wy, int wz) const {
    const int S = constants::WORLD_CHUNK_SIZE;
    const int cx = static_cast<int>(std::floor(static_cast<float>(wx) / S));
    const int cz = static_cast<int>(std::floor(static_cast<float>(wz) / S));

    auto it = chunks.find({cx, cz});
    if (it == chunks.end()) return false;

    const int lx = wx - cx * S;
    const int lz = wz - cz * S;
    return it->second.isSolidLocal(lx, wy, lz);
}

// ---------------------------------------------------------------------------
// Frustum extraction (Gribb/Hartmann method)
// ---------------------------------------------------------------------------
WorldGenerator::Frustum WorldGenerator::extractFrustum(const glm::mat4& vp) {
    glm::mat4 m = glm::transpose(vp); // m[i] = i-th row of vp
    Frustum f;
    f.planes[0] = m[3] + m[0]; // left
    f.planes[1] = m[3] - m[0]; // right
    f.planes[2] = m[3] + m[1]; // bottom
    f.planes[3] = m[3] - m[1]; // top
    f.planes[4] = m[3] + m[2]; // near
    f.planes[5] = m[3] - m[2]; // far
    for (auto& p : f.planes) {
        float len = glm::length(glm::vec3(p));
        if (len > 0.0f) p /= len;
    }
    return f;
}

bool WorldGenerator::aabbInFrustum(const Frustum& f, glm::vec3 mn, glm::vec3 mx) {
    for (const auto& plane : f.planes) {
        glm::vec3 n(plane);
        glm::vec3 pv(n.x >= 0 ? mx.x : mn.x,
                     n.y >= 0 ? mx.y : mn.y,
                     n.z >= 0 ? mx.z : mn.z);
        if (glm::dot(n, pv) + plane.w < 0.0f)
            return false;
    }
    return true;
}
