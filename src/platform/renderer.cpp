#include "platform/renderer.hpp"

#include <memory>

namespace platform {
    Renderer::Renderer() {
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            std::cout << "Failed to initialize GLAD" << std::endl;
            exit(-1);
        }

        this->currentShader = new utils::Shader("../assets/vertex.glsl", "../assets/fragment.glsl");
    }

    void Renderer::init() {
            constexpr float vertices[] = {
        // Front face (z = 0.0f)
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // front-up-right 0
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, // front-down-right 1
        -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // front-up-left 2
        -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 0.0f, // front-down-left 3

        // Back face (z = -1.0f)
        1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, // back-up-right 4
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, // back-down-right 5
        -1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, // back-up-left 6
        -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, // back-down-left 7

        // Top face (y = 1.0f)
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // top-up-right 8
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // top-up-left 9

        // Bottom face (y = -1.0f)
        1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-up-right 10
        -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-up-left 11

        // Left face (x = -1.0f)
        -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // left-up-right 12
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // left-down-right 13

        // Right face (x = 1.0f)
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // right-up-right 14
        1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // right-down-right 15
    };


    constexpr int indices[] = {
        // Front face
        0, 1, 2,
        1, 2, 3,

        // Back face
        4, 5, 6,
        5, 6, 7,

        // Top face
        8, 9, 0,
        9, 0, 1,

        // Bottom face
        10, 11, 2,
        11, 2, 3,

        // Left face
        12, 13, 6,
        13, 6, 7,

        // Right face
        14, 15, 4,
        15, 4, 5
    };


    unsigned int VBO, VAO;
    unsigned int EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    }


    void Renderer::update() {
        glClearColor(0.2f, 0.57f, 0.92f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

    }

    utils::Shader *Renderer::getCurrentShader() const {
        return currentShader;
    }

    Renderer::~Renderer() {
        delete this->currentShader;
    }

}
