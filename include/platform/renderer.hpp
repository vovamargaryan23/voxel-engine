#pragma once
#include <memory>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "utils/shader.hpp"
#include "world/world_generator.hpp"

namespace platform {
    class Renderer {
    public:
        Renderer();

        void init(WorldGenerator& wg);
        void update(WorldGenerator& wg, const glm::mat4& vpMatrix);

        utils::Shader* getCurrentShader() const { return currentShader.get(); }

    private:
        std::unique_ptr<utils::Shader> currentShader;
    };
}
