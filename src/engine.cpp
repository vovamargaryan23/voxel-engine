#include "engine.hpp"

Engine::Engine()
{
    this->window = std::make_unique<platform::Window>();
    this->camera = std::make_unique<platform::Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
    this->inputManager = std::make_unique<InputManager>(*camera);
}

void Engine::Start() const {
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }

    const Shader shader("../assets/vertex.glsl", "../assets/fragment.glsl");
    constexpr float vertices[] = {
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glfwSetWindowUserPointer(window->get(), inputManager.get());
    glfwSetCursorPosCallback(window->get(), InputManager::cursorCallbackWrapper);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while(!this->window->shouldClose())
    {
        const float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.2f, 0.57f, 0.92f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 projection = glm::perspective(glm::radians(camera->zoom),
            (float)constants::DEFAULT_WINDOW_WIDTH / (float)constants::DEFAULT_WINDOW_HEIGHT,
            0.1f,
            100.0f);
        shader.setMat4("projection", projection);

        glm::mat4 view = camera->getViewMatrix();
        shader.setMat4("view", view);
        shader.use();

        inputManager->processInput(deltaTime, window->get());

        glDrawArrays(GL_TRIANGLES, 0, 3);

        this->window->update();
    }

    this->window->terminate();
}
