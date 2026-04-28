#include "fast_noise_lite.h"
#include "glad/glad.h"
#include "world/chunk.hpp"
#include "constants.hpp"
#include <vector>
#include <cmath>
#include <algorithm>

// ---------------------------------------------------------------------------
// Vertex layout: x y z  nx ny nz  blockTypeF  ao   (8 floats, stride 32 B)
// ---------------------------------------------------------------------------

static float vertAO(bool s1, bool s2, bool corner) {
    if (s1 && s2) return 0.0f;
    return (3.0f - (float(s1) + float(s2) + float(corner))) / 3.0f;
}

static void addFace(
    std::vector<float>&        verts,
    std::vector<unsigned int>& idx,
    glm::vec3 v0, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3,
    glm::vec3 normal, float blockType,
    float ao0, float ao1, float ao2, float ao3)
{
    unsigned int b = static_cast<unsigned int>(verts.size() / 8);

    auto push = [&](glm::vec3 p, float ao) {
        verts.push_back(p.x);      verts.push_back(p.y);      verts.push_back(p.z);
        verts.push_back(normal.x); verts.push_back(normal.y); verts.push_back(normal.z);
        verts.push_back(blockType);
        verts.push_back(ao);
    };
    push(v0, ao0); push(v1, ao1); push(v2, ao2); push(v3, ao3);

    // AO flip: choose the diagonal that avoids a high-contrast seam
    if (ao0 + ao2 > ao1 + ao3) {
        idx.insert(idx.end(), {b, b+3, b+1,  b+1, b+3, b+2});
    } else {
        idx.insert(idx.end(), {b, b+2, b+1,  b,   b+3, b+2});
    }
}

// ---------------------------------------------------------------------------
// Chunk impl
// ---------------------------------------------------------------------------

Chunk::Chunk(glm::vec3 pos)
    : blocks(std::make_unique<Block[]>(SIZE * SIZE * SIZE)), position(pos) {
    initializeBlocks();
    buildMesh();
}

Chunk::~Chunk() {
    if (oVAO) { glDeleteVertexArrays(1, &oVAO); glDeleteBuffers(1, &oVBO); glDeleteBuffers(1, &oEBO); }
    if (tVAO) { glDeleteVertexArrays(1, &tVAO); glDeleteBuffers(1, &tVBO); glDeleteBuffers(1, &tEBO); }
}

Chunk::Chunk(Chunk&& o) noexcept
    : blocks(std::move(o.blocks)), position(o.position),
      oVAO(o.oVAO), oVBO(o.oVBO), oEBO(o.oEBO), oIndexCount(o.oIndexCount),
      tVAO(o.tVAO), tVBO(o.tVBO), tEBO(o.tEBO), tIndexCount(o.tIndexCount) {
    o.oVAO = o.oVBO = o.oEBO = 0; o.oIndexCount = 0;
    o.tVAO = o.tVBO = o.tEBO = 0; o.tIndexCount = 0;
}

Chunk& Chunk::operator=(Chunk&& o) noexcept {
    if (this != &o) {
        if (oVAO) { glDeleteVertexArrays(1, &oVAO); glDeleteBuffers(1, &oVBO); glDeleteBuffers(1, &oEBO); }
        if (tVAO) { glDeleteVertexArrays(1, &tVAO); glDeleteBuffers(1, &tVBO); glDeleteBuffers(1, &tEBO); }
        blocks = std::move(o.blocks); position = o.position;
        oVAO = o.oVAO; oVBO = o.oVBO; oEBO = o.oEBO; oIndexCount = o.oIndexCount;
        tVAO = o.tVAO; tVBO = o.tVBO; tEBO = o.tEBO; tIndexCount = o.tIndexCount;
        o.oVAO = o.oVBO = o.oEBO = 0; o.oIndexCount = 0;
        o.tVAO = o.tVBO = o.tEBO = 0; o.tIndexCount = 0;
    }
    return *this;
}

// ---------------------------------------------------------------------------
// Block init — 2D heightmap terrain
// ---------------------------------------------------------------------------
void Chunk::initializeBlocks() {
    FastNoiseLite noise;
    noise.SetSeed(constants::NOISE_SEED);
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetFrequency(0.025f);

    const float baseH  = SIZE * 0.4f;
    const float rangeH = SIZE * 0.35f;
    const float seaY   = SIZE * 0.25f;

    for (int x = 0; x < SIZE; x++) {
        for (int z = 0; z < SIZE; z++) {
            const float wx = position.x + x;
            const float wz = position.z + z;
            const float h  = noise.GetNoise(wx, wz);
            const int   surface = static_cast<int>(baseH + h * rangeH);

            for (int y = 0; y < SIZE; y++) {
                Block& b = blockAt(x, y, z);
                if (y > surface) {
                    b = Block(y <= static_cast<int>(seaY) ? BlockType::WATER : BlockType::EMPTY);
                } else if (y == surface) {
                    b = Block(y <= static_cast<int>(seaY) ? BlockType::DIRT : BlockType::GRASS);
                } else if (y >= surface - 3) {
                    b = Block(BlockType::DIRT);
                } else {
                    b = Block(BlockType::STONE);
                }
            }
        }
    }
}

// ---------------------------------------------------------------------------
// Solid / occluding helpers
// ---------------------------------------------------------------------------
bool Chunk::isSolidAt(int x, int y, int z) const {
    if (x < 0 || x >= SIZE || y < 0 || y >= SIZE || z < 0 || z >= SIZE) return false;
    const Block& b = blockAt(x, y, z);
    return !b.isEmpty() && !b.isTransparent();
}

bool Chunk::isSolidLocal(int lx, int ly, int lz) const {
    if (lx < 0 || lx >= SIZE || ly < 0 || ly >= SIZE || lz < 0 || lz >= SIZE) return false;
    return !blockAt(lx, ly, lz).isEmpty();
}

// ---------------------------------------------------------------------------
// Mesh build — EBO, per-block colours, AO, separate transparent pass
// ---------------------------------------------------------------------------
void Chunk::buildMesh() {
    std::vector<float>        oV, tV;
    std::vector<unsigned int> oI, tI;

    for (int x = 0; x < SIZE; x++) {
    for (int y = 0; y < SIZE; y++) {
    for (int z = 0; z < SIZE; z++) {
        const Block& b = blockAt(x, y, z);
        if (b.isEmpty()) continue;

        const bool transp = b.isTransparent();
        auto& verts = transp ? tV : oV;
        auto& inds  = transp ? tI : oI;

        const float bt  = static_cast<float>(static_cast<int>(b.getType()));
        const glm::vec3 p(position.x + x, position.y + y, position.z + z);

        // Neighbour occlusion: face drawn when neighbour is empty or transparent
        auto occluding = [&](int nx, int ny, int nz) -> bool {
            if (nx < 0||nx>=SIZE||ny < 0||ny>=SIZE||nz < 0||nz>=SIZE) return false;
            const Block& nb = blockAt(nx, ny, nz);
            return !nb.isEmpty() && !nb.isTransparent();
        };

        // ---------- FRONT (+Z) ----------
        if (z == SIZE-1 || !occluding(x,y,z+1)) {
            float ao0 = vertAO(isSolidAt(x-1,y,z+1),  isSolidAt(x,y+1,z+1), isSolidAt(x-1,y+1,z+1));
            float ao1 = vertAO(isSolidAt(x+1,y,z+1),  isSolidAt(x,y+1,z+1), isSolidAt(x+1,y+1,z+1));
            float ao2 = vertAO(isSolidAt(x+1,y,z+1),  isSolidAt(x,y-1,z+1), isSolidAt(x+1,y-1,z+1));
            float ao3 = vertAO(isSolidAt(x-1,y,z+1),  isSolidAt(x,y-1,z+1), isSolidAt(x-1,y-1,z+1));
            addFace(verts, inds,
                p+glm::vec3(-0.5f,+0.5f,+0.5f), p+glm::vec3(+0.5f,+0.5f,+0.5f),
                p+glm::vec3(+0.5f,-0.5f,+0.5f), p+glm::vec3(-0.5f,-0.5f,+0.5f),
                {0,0,1}, bt, ao0, ao1, ao2, ao3);
        }
        // ---------- BACK (-Z) ----------
        if (z == 0 || !occluding(x,y,z-1)) {
            float ao0 = vertAO(isSolidAt(x+1,y,z-1),  isSolidAt(x,y+1,z-1), isSolidAt(x+1,y+1,z-1));
            float ao1 = vertAO(isSolidAt(x-1,y,z-1),  isSolidAt(x,y+1,z-1), isSolidAt(x-1,y+1,z-1));
            float ao2 = vertAO(isSolidAt(x-1,y,z-1),  isSolidAt(x,y-1,z-1), isSolidAt(x-1,y-1,z-1));
            float ao3 = vertAO(isSolidAt(x+1,y,z-1),  isSolidAt(x,y-1,z-1), isSolidAt(x+1,y-1,z-1));
            addFace(verts, inds,
                p+glm::vec3(+0.5f,+0.5f,-0.5f), p+glm::vec3(-0.5f,+0.5f,-0.5f),
                p+glm::vec3(-0.5f,-0.5f,-0.5f), p+glm::vec3(+0.5f,-0.5f,-0.5f),
                {0,0,-1}, bt, ao0, ao1, ao2, ao3);
        }
        // ---------- LEFT (-X) ----------
        if (x == 0 || !occluding(x-1,y,z)) {
            float ao0 = vertAO(isSolidAt(x-1,y,z-1),  isSolidAt(x-1,y+1,z), isSolidAt(x-1,y+1,z-1));
            float ao1 = vertAO(isSolidAt(x-1,y,z+1),  isSolidAt(x-1,y+1,z), isSolidAt(x-1,y+1,z+1));
            float ao2 = vertAO(isSolidAt(x-1,y,z+1),  isSolidAt(x-1,y-1,z), isSolidAt(x-1,y-1,z+1));
            float ao3 = vertAO(isSolidAt(x-1,y,z-1),  isSolidAt(x-1,y-1,z), isSolidAt(x-1,y-1,z-1));
            addFace(verts, inds,
                p+glm::vec3(-0.5f,+0.5f,-0.5f), p+glm::vec3(-0.5f,+0.5f,+0.5f),
                p+glm::vec3(-0.5f,-0.5f,+0.5f), p+glm::vec3(-0.5f,-0.5f,-0.5f),
                {-1,0,0}, bt, ao0, ao1, ao2, ao3);
        }
        // ---------- RIGHT (+X) ----------
        if (x == SIZE-1 || !occluding(x+1,y,z)) {
            float ao0 = vertAO(isSolidAt(x+1,y,z+1),  isSolidAt(x+1,y+1,z), isSolidAt(x+1,y+1,z+1));
            float ao1 = vertAO(isSolidAt(x+1,y,z-1),  isSolidAt(x+1,y+1,z), isSolidAt(x+1,y+1,z-1));
            float ao2 = vertAO(isSolidAt(x+1,y,z-1),  isSolidAt(x+1,y-1,z), isSolidAt(x+1,y-1,z-1));
            float ao3 = vertAO(isSolidAt(x+1,y,z+1),  isSolidAt(x+1,y-1,z), isSolidAt(x+1,y-1,z+1));
            addFace(verts, inds,
                p+glm::vec3(+0.5f,+0.5f,+0.5f), p+glm::vec3(+0.5f,+0.5f,-0.5f),
                p+glm::vec3(+0.5f,-0.5f,-0.5f), p+glm::vec3(+0.5f,-0.5f,+0.5f),
                {1,0,0}, bt, ao0, ao1, ao2, ao3);
        }
        // ---------- TOP (+Y) ----------
        if (y == SIZE-1 || !occluding(x,y+1,z)) {
            float ao0 = vertAO(isSolidAt(x-1,y+1,z),  isSolidAt(x,y+1,z-1), isSolidAt(x-1,y+1,z-1));
            float ao1 = vertAO(isSolidAt(x+1,y+1,z),  isSolidAt(x,y+1,z-1), isSolidAt(x+1,y+1,z-1));
            float ao2 = vertAO(isSolidAt(x+1,y+1,z),  isSolidAt(x,y+1,z+1), isSolidAt(x+1,y+1,z+1));
            float ao3 = vertAO(isSolidAt(x-1,y+1,z),  isSolidAt(x,y+1,z+1), isSolidAt(x-1,y+1,z+1));
            addFace(verts, inds,
                p+glm::vec3(-0.5f,+0.5f,-0.5f), p+glm::vec3(+0.5f,+0.5f,-0.5f),
                p+glm::vec3(+0.5f,+0.5f,+0.5f), p+glm::vec3(-0.5f,+0.5f,+0.5f),
                {0,1,0}, bt, ao0, ao1, ao2, ao3);
        }
        // ---------- BOTTOM (-Y) ----------
        if (y == 0 || !occluding(x,y-1,z)) {
            float ao0 = vertAO(isSolidAt(x-1,y-1,z),  isSolidAt(x,y-1,z+1), isSolidAt(x-1,y-1,z+1));
            float ao1 = vertAO(isSolidAt(x+1,y-1,z),  isSolidAt(x,y-1,z+1), isSolidAt(x+1,y-1,z+1));
            float ao2 = vertAO(isSolidAt(x+1,y-1,z),  isSolidAt(x,y-1,z-1), isSolidAt(x+1,y-1,z-1));
            float ao3 = vertAO(isSolidAt(x-1,y-1,z),  isSolidAt(x,y-1,z-1), isSolidAt(x-1,y-1,z-1));
            addFace(verts, inds,
                p+glm::vec3(-0.5f,-0.5f,+0.5f), p+glm::vec3(+0.5f,-0.5f,+0.5f),
                p+glm::vec3(+0.5f,-0.5f,-0.5f), p+glm::vec3(-0.5f,-0.5f,-0.5f),
                {0,-1,0}, bt, ao0, ao1, ao2, ao3);
        }
    }}}

    uploadMesh(oVAO, oVBO, oEBO, oV, oI);
    oIndexCount = static_cast<long long>(oI.size());
    uploadMesh(tVAO, tVBO, tEBO, tV, tI);
    tIndexCount = static_cast<long long>(tI.size());
}

void Chunk::uploadMesh(unsigned int& vao, unsigned int& vbo, unsigned int& ebo,
                       const std::vector<float>& verts,
                       const std::vector<unsigned int>& indices) {
    if (indices.empty()) return;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(verts.size() * sizeof(float)),
        verts.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)),
        indices.data(), GL_STATIC_DRAW);

    constexpr int stride = 8 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, stride, (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
}

void Chunk::renderOpaque() const {
    if (!oVAO || oIndexCount == 0) return;
    glBindVertexArray(oVAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(oIndexCount), GL_UNSIGNED_INT, nullptr);
}

void Chunk::renderTransparent() const {
    if (!tVAO || tIndexCount == 0) return;
    glBindVertexArray(tVAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(tIndexCount), GL_UNSIGNED_INT, nullptr);
}
