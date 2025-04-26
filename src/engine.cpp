#include "engine.hpp"

Engine::Engine()
{
    this->window = std::make_unique<Window>();
}

void Engine::start()
{
    this->window->init();

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    } 

    Shader shader("../assets/vertex.glsl", "../assets/fragment.glsl");

    float vertices[] = {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
         0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while(!this->window->shouldClose())
    {
        glClearColor(0.2f, 0.57f, 0.92f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();

        glDrawArrays(GL_TRIANGLES, 0, 3);

        this->window->update();
    }

    this->window->terminate();
}