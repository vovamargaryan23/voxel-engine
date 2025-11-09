#include "platform/renderer.hpp"

#include <memory>

namespace platform {
    Renderer::Renderer() {
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            exit(-1);
        }

        this->currentShader = new utils::Shader("../assets/vertex.glsl", "../assets/fragment.glsl");
        this->worldGenerator = std::make_unique<WorldGenerator>();
    }

    void Renderer::init() {
        // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ); // Toggle Wireframe

        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);

        this->worldGenerator->generate();

    }

    void Renderer::update() {
        glClearColor(0.2f, 0.57f, 0.92f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->worldGenerator->render();
    }

    utils::Shader *Renderer::getCurrentShader() const {
        return currentShader;
    }

    Renderer::~Renderer() {
        delete this->currentShader;
    }

}
