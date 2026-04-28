#include "platform/renderer.hpp"
#include "utils/logger.hpp"
#include <iostream>

namespace platform {
    Renderer::Renderer() {
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            utils::Log().error("Failed to initialize GLAD");
            exit(-1);
        }
        currentShader = std::make_unique<utils::Shader>(
            std::string(ASSETS_DIR) + "/vertex.glsl",
            std::string(ASSETS_DIR) + "/fragment.glsl"
        );
    }

    void Renderer::init(WorldGenerator& wg) {
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        wg.update(glm::vec3(0.0f));
    }

    void Renderer::update(WorldGenerator& wg, const glm::mat4& vpMatrix) {
        glClearColor(0.2f, 0.57f, 0.92f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        wg.render(vpMatrix);
    }
}
