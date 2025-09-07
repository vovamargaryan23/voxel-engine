#pragma once
#include <memory>
#include "glad/glad.h"
#include "GLFW/glfw3.h"


#include "utils/shader.hpp"

namespace platform {
    class Renderer {
    private:
        utils::Shader *currentShader;
    public:
        Renderer();
        void init();
        void update();
       utils::Shader *getCurrentShader() const;
        ~Renderer();
    };
};