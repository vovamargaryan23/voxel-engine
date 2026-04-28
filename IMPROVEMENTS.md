# Voxel Engine — Planned Improvements & Features

> Date: 2026-04-27

---

## Rendering

### 1. Index Buffer Objects (EBO)
A cube face has 4 unique vertices but currently emits 6 (two triangles with 2 shared vertices duplicated). Switching to indexed rendering with an EBO reduces GPU memory by ~33% and improves vertex cache reuse via the post-transform cache.

### 2. Frustum Culling
Before issuing a draw call for each chunk, test its AABB against the camera frustum. Chunks entirely outside all six planes are skipped with zero GPU cost. Essential once multi-chunk worlds are in play.

### 3. Greedy Meshing
Instead of emitting one quad per visible block face, merge co-planar adjacent faces of the same block type into a single larger quad. For flat terrain this can reduce draw vertex count by 80–90%.

### 4. Ambient Occlusion (AO)
Per-vertex AO baked at mesh generation time. For each vertex, count how many of the three adjacent corner voxels are solid. Darken the vertex colour proportionally. Produces the characteristic soft shadows in voxel engines at zero runtime cost.

### 5. Per-Block-Type Colours / Textures
Currently all blocks share one hardcoded base colour. Two approaches:
- **Colour palette**: encode block type as a float in the vertex data; the fragment shader does a palette lookup from a 1D texture or a small uniform array.
- **Texture atlas**: pack all block face sprites into one atlas texture; encode UV offsets in the vertex data.

### 6. View-Distance & Level of Detail (LOD)
Generate simplified meshes for chunks far from the camera (e.g., surface-only, lower noise resolution). Swap to the full mesh when the chunk enters a closer ring. Reduces both geometry and generation time at distance.

### 7. Transparent Block Rendering
Glass and water blocks need a separate rendering pass after all opaque geometry, with depth writes disabled and blending enabled. The `isTransparent()` flag on `Block` is already wired; it just needs the second pass in the renderer.

---

## World Generation

### 8. Multi-Chunk World with Streaming
`WorldGenerator` currently produces one chunk. Extend it to a grid of chunks around the camera. Load new chunks as the player moves and unload chunks beyond the view distance. A worker thread pool for background generation prevents frame stalls.

### 9. 2D Heightmap Terrain
Switch from full 3D noise to a 2D heightmap approach: `height = f(x, z)`. Fill below the surface with stone, a thin layer of dirt, and a single grass cap. Produces recognisable terrain far more cheaply than 3D noise and avoids floating islands unless intentional.

### 10. Biomes
Divide the world into biome regions using a low-frequency noise map. Each biome controls surface block type (sand, snow, grass), tree density, and height range. Biome blending at borders uses a weighted average of two heightmaps.

### 11. Structures (Trees, Caves, Ores)
- **Trees**: place trunks and leaf canopies at surface points above a noise threshold.
- **Caves**: carve hollow regions using a second 3D noise pass with a lower threshold.
- **Ores**: scatter ore blocks at specific depth bands using small-scale noise.

---

## Camera & Input

### 12. Vertical Camera Movement
Add `UP` / `DOWN` `CameraMovement` values and handle `Space` / `Left Shift` in `InputManager`. Required for flight-mode exploration.

### 13. Collision Detection
Basic AABB player–world collision. Test the camera bounding box against nearby solid blocks each frame and project the movement vector to prevent clipping through geometry.

### 14. Window Resize Handling
Register a GLFW framebuffer size callback. Recalculate the projection matrix and call `glViewport` in the callback so the aspect ratio stays correct when the window is resized.

---

## Architecture & Code Quality

### 15. Configuration File
Load chunk size, view distance, window dimensions, and noise seed from a simple INI or JSON config file at startup instead of hardcoding them in `constants.hpp`. Enables runtime tuning without recompiling.

### 16. Logging System
Replace raw `std::cerr` / `std::cout` calls with a lightweight log system that supports severity levels (DEBUG, INFO, WARN, ERROR) and can write to a file. Useful for release builds where a terminal is not visible.

### 17. Dirty-Flag Mesh Rebuilds
When the player modifies a block (place / break), mark the owning chunk and its face-sharing neighbours as dirty. Rebuild only those meshes asynchronously on the next frame rather than all chunks up-front.

### 18. Event System
Replace direct method calls between subsystems with a lightweight publish/subscribe event bus. Decouples `Engine`, `InputManager`, `Renderer`, and `WorldGenerator`, making each easier to test and extend independently.

### 19. Unit & Integration Tests
Add a CMake test target using a lightweight C++ test framework (Catch2 or doctest). Priority areas: noise value ranges, face-culling correctness, chunk serialisation round-trip, and block type lookups.

---

## Summary Priority Order

| Priority | Feature | Impact |
|----------|---------|--------|
| High | Multi-chunk streaming (#8) | Playable world |
| High | 2D heightmap terrain (#9) | Better terrain |
| High | Frustum culling (#2) | Frame-rate |
| Medium | EBO / indexed rendering (#1) | GPU memory |
| Medium | Per-block colours / textures (#5) | Visual quality |
| Medium | Ambient occlusion (#4) | Visual quality |
| Medium | Window resize (#14) | Polish |
| Medium | Vertical movement + collision (#12, #13) | Playability |
| Low | Greedy meshing (#3) | Geometry count |
| Low | LOD (#6) | Scale |
| Low | Biomes / structures (#10, #11) | Content |
| Low | Config file + logging (#15, #16) | Maintainability |
