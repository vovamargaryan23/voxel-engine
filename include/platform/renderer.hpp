#pragma once
#include <memory>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "utils/shader.hpp"
#include "world/world_generator.hpp"

namespace platform {
    class Renderer {
    private:
        std::unique_ptr<utils::Shader> currentShader;
        std::unique_ptr<WorldGenerator> worldGenerator;
    public:
        Renderer();
        void init();
        void update();
        utils::Shader* getCurrentShader() const;
    };
}
