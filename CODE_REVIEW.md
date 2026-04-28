# Voxel Engine — Code Review

> Scanned: `include/`, `src/`, `assets/`, `CMakeLists.txt`  
> Date: 2026-04-27

---

## Pros

| # | Area | Observation |
|---|------|-------------|
| 1 | **Structure** | Clean modular layout (`platform/`, `utils/`, `world/`) that mirrors the conceptual separation of concerns. |
| 2 | **Ownership** | `std::unique_ptr` used for major subsystems in `Engine` — no manual memory management at the top level. |
| 3 | **Face culling** | `generateChunkVertices` checks adjacent blocks before emitting a face, avoiding interior geometry entirely. |
| 4 | **Static mesh** | Chunk vertices are built once at load time and uploaded to the GPU as a static VBO — no CPU rebuild every frame. |
| 5 | **GL state** | Depth testing and back-face culling are correctly enabled (`GL_DEPTH_TEST`, `GL_CULL_FACE`). |
| 6 | **Noise generation** | FastNoiseLite is integrated for 3D terrain with per-chunk world-space coordinates, producing continuous terrain across chunk boundaries. |
| 7 | **Header guards** | `#pragma once` used consistently across all headers. |
| 8 | **Const correctness** | `const` applied to getters and read-only methods throughout. |
| 9 | **Explicit constructors** | Single-argument constructors are marked `explicit` where appropriate (e.g., `Camera`, `Chunk`). |
| 10 | **Constants namespace** | Magic numbers centralised in `constants.hpp` with `constexpr`. |

---

## Cons & Bugs

### Critical Bugs

**1. Uniforms set before shader is bound (`src/engine.cpp:26-28`)**

```cpp
// current (BROKEN — glUniformMatrix4fv acts on the currently active program)
this->renderer->getCurrentShader()->setMat4("projection", projection);
this->renderer->getCurrentShader()->setMat4("view", view);
this->renderer->getCurrentShader()->use();   // too late
```

`glUniformMatrix4fv` only modifies the *currently active* shader program. Calling `setMat4` before `use()` silently writes into whatever program was previously bound (or into nothing). The fix is to call `use()` first.

```cpp
// fixed
this->renderer->getCurrentShader()->use();
this->renderer->getCurrentShader()->setMat4("projection", projection);
this->renderer->getCurrentShader()->setMat4("view", view);
```

---

**2. Missing semicolon in `src/main.cpp:6`**

```cpp
engine.Start()   // syntax error — missing ;
```

This prevents the project from compiling.

---

**3. Stack overflow — 128³ `Block` array allocated on the stack (`include/world/chunk.hpp:9`)**

```cpp
Block blocks[128][128][128] = {};
```

`Block` contains a `glm::vec3` (12 B), a `bool` (1 B), and a `BlockType` enum (4 B). With padding, each `Block` is ~20 bytes.

```
128³ × 20 B = 33,554,432 B ≈ 32 MB per Chunk on the stack
```

The default stack size on Linux is 8 MB. Any `Chunk` construction will overflow the stack and crash. This array must be heap-allocated.

```cpp
// fix: use a flat heap-allocated array
std::unique_ptr<Block[]> blocks;
// in constructor:
blocks = std::make_unique<Block[]>(SIZE * SIZE * SIZE);
// access:
blocks[x * SIZE * SIZE + y * SIZE + z]
```

---

### Performance Issues

**4. `glGetUniformLocation` called every frame (`src/utils/shader.cpp:70`)**

```cpp
glUniformMatrix4fv(glGetUniformLocation(this->programId, name.c_str()), ...);
```

`glGetUniformLocation` is a driver round-trip. With two uniform calls per frame it is 2 driver queries at 60+ FPS. Cache the locations at construction time in an `std::unordered_map<std::string, GLint>`.

---

**5. `appendBlockVertices` — no `reserve`, 12 `vector::insert` calls per face (`include/world/block.hpp:30-100`)**

Each face makes 6 `insert` calls (one per vertex), each inserting 6 floats. Up to 72 calls per solid block. With potentially millions of blocks, this causes repeated reallocations and poor cache locality.

```cpp
// Before the chunk mesh loop:
chunkVertices.reserve(estimatedFaceCount * 6 * 6);
// Inside appendBlockVertices, replace multiple inserts with one batch:
const float faceData[] = { ... };
vertices.insert(vertices.end(), faceData, faceData + 36); // all 6 vertices at once
```

---

**6. Each `Block` stores its world position (redundant 12 B × 2 M blocks)**

`Block::position` is set to the world-space coordinate and stored permanently, but the position is only used during `generateChunkVertices`. It can be derived on the fly from array indices + chunk offset, saving ~25 MB per chunk.

---

**7. `glGetUniformLocation` / `setMat4` — no cached location**

Already noted above. Also `setMat4` takes `const std::string&` by value and calls `.c_str()` every frame. Pre-cache the `GLint` location per uniform name.

---

### Design & Code Quality Issues

**8. `DeltaTime` — `static` globals in a header (`include/utils/delta_time.hpp:4-5`)**

```cpp
static float deltaTime = 0.0f;
static float lastFrame = 0.0f;
```

`static` at namespace scope in a header gives *each translation unit its own copy* of these variables. The intended singleton state is silently broken if more than one TU includes this header. These should be `static` member variables of the class, or the class should be a proper instance.

---

**9. `Renderer::currentShader` — raw pointer mixed with `unique_ptr` (`include/platform/renderer.hpp:13`)**

```cpp
utils::Shader *currentShader;          // raw
std::unique_ptr<WorldGenerator> worldGenerator;  // smart
```

The same class mixes ownership styles. `currentShader` is allocated with `new` in the constructor and freed in the destructor. If an exception is thrown between construction and destruction, the shader leaks. Use `std::unique_ptr<utils::Shader>`.

---

**10. GPU resource leak — `Chunk` has no destructor**

`Chunk` calls `glGenVertexArrays` and `glGenBuffers` but never calls `glDeleteVertexArrays` / `glDeleteBuffers`. This leaks GPU objects whenever a `Chunk` is destroyed (e.g., on unload).

```cpp
Chunk::~Chunk() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}
```

---

**11. Global `FastNoiseLite noise` in `src/world/chunk.cpp:7`**

```cpp
FastNoiseLite noise;
```

This is mutable file-scope state. It is re-configured on every `initializeBlocks()` call and is not thread-safe. It should be a local variable in `initializeBlocks`, or a member/static of `WorldGenerator`.

---

**12. `BlockType` — unscoped enum pollutes the global namespace (`include/world/block_type.hpp`)**

```cpp
enum BlockType { EMPTY, GRASS, STONE, GLASS };
```

`EMPTY`, `GRASS`, `STONE`, and `GLASS` are injected into the global namespace. Use `enum class BlockType` and qualify usages as `BlockType::GRASS`, etc.

---

**13. All `Camera` members are public (`include/platform/camera.hpp:23-29`)**

`position`, `front`, `up`, `right`, `worldUp`, `yaw`, `pitch` etc. are all public. Any code can mutate them and leave the camera in an inconsistent state (e.g., setting `yaw` without calling `updateCameraVectors`). At minimum `front`, `right`, `up` should be read-only (private with getters), since they are derived from `yaw`/`pitch`.

---

**14. `int programId` should be `GLuint` (`include/utils/shader.hpp:12`)**

OpenGL object IDs are `GLuint` (`unsigned int`). Using signed `int` causes implicit conversion warnings and can produce incorrect behavior if the ID value is ever used in bitwise operations or compared against `GL_` constants.

---

**15. `isTransparent_` is computed but never used**

`Block::adjustTransparency()` sets `isTransparent_` for `GLASS` blocks, but `generateChunkVertices` only checks `BlockType::EMPTY` when deciding whether to draw a face — it never consults `isTransparent_`. Transparent neighbours should not occlude faces of adjacent blocks.

---

**16. No model matrix in the vertex shader (`assets/vertex.glsl`)**

```glsl
gl_Position = projection * view * vec4(aPos, 1.0);
```

There is no `model` uniform. This means chunks cannot be transformed independently — they always render at the world origin. Every chunk's vertices must be pre-baked in world space, making future chunk streaming or instancing impossible without a shader change.

---

**17. `const std::string` constant in a header (`include/constants.hpp:8`)**

```cpp
const std::string DEFAULT_WINDOW_TITLE = "Voxel Engine";
```

`const std::string` in a header constructs a `std::string` object in every TU that includes the file. Use `constexpr const char*` or, in C++17+, `constexpr std::string_view`.

```cpp
constexpr const char* DEFAULT_WINDOW_TITLE = "Voxel Engine";
```

---

**18. Vertex color data is wasteful and incorrect**

Each vertex carries 3 floats of "color" (`aColor`), but every face uses `{0.5f, 0.0f, 0.0f}` or `{0.5f, 0.5f, 0.0f}` — constant values with no meaningful per-vertex differentiation. The color attribute adds 12 bytes per vertex (half the vertex size) for data that should instead be a per-face or per-block-type uniform or flat attribute.

---

### Build System Issues

**19. `file(GLOB_RECURSE)` in CMakeLists.txt**

```cmake
file(GLOB_RECURSE SOURCES src/*.cpp ...)
```

CMake's `GLOB_RECURSE` does not re-run when new `.cpp` files are added — incremental builds will silently miss new sources until the developer manually re-runs CMake. Prefer explicitly listing sources or use `target_sources`.

---

**20. No optimization flags for release builds**

```cmake
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
```

Only `-Wall` is set, globally (affecting all build types). There are no `-O2`/`-O3` flags for release, no `-g` for debug. Use `CMAKE_CXX_FLAGS_RELEASE` and `CMAKE_CXX_FLAGS_DEBUG` or `target_compile_options` with generator expressions.

```cmake
target_compile_options(VoxelEngine PRIVATE
    -Wall -Wextra
    $<$<CONFIG:Release>:-O3>
    $<$<CONFIG:Debug>:-g -O0>
)
```

---

## Summary Table

| Severity | Issue | File |
|----------|-------|------|
| **Critical bug** | Uniforms set before `use()` | `src/engine.cpp` |
| **Critical bug** | Missing semicolon | `src/main.cpp` |
| **Critical crash** | 32 MB `Block` array on stack | `include/world/chunk.hpp` |
| **Performance** | `glGetUniformLocation` every frame | `src/utils/shader.cpp` |
| **Performance** | No `reserve` + 12× `insert` per face | `include/world/block.hpp` |
| **Performance** | `Block` stores redundant world position | `include/world/block.hpp` |
| **Memory leak** | `Chunk` VAO/VBO never deleted | `src/world/chunk.cpp` |
| **Design** | `static` globals in header (ODR) | `include/utils/delta_time.hpp` |
| **Design** | Raw pointer mixed with `unique_ptr` | `include/platform/renderer.hpp` |
| **Design** | Global mutable `FastNoiseLite` | `src/world/chunk.cpp` |
| **Design** | Unscoped enum in global namespace | `include/world/block_type.hpp` |
| **Design** | All `Camera` members public | `include/platform/camera.hpp` |
| **Design** | `isTransparent_` unused in culling | `include/world/block.hpp` |
| **Design** | No model matrix in vertex shader | `assets/vertex.glsl` |
| **Minor** | `int programId` should be `GLuint` | `include/utils/shader.hpp` |
| **Minor** | `const std::string` in header | `include/constants.hpp` |
| **Minor** | Wasteful per-vertex color data | `include/world/block.hpp` |
| **Build** | `GLOB_RECURSE` in CMake | `CMakeLists.txt` |
| **Build** | No release/debug compile flags | `CMakeLists.txt` |
