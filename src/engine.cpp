#include "engine.hpp"


Engine::Engine()
{
    this->window = std::make_unique<platform::Window>();
    this->camera = std::make_unique<platform::Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
    this->inputManager = std::make_unique<utils::InputManager>(*camera);
    this->renderer = std::make_unique<platform::Renderer>();
}

void Engine::Start() const {
    this->renderer->init();

    glfwSetWindowUserPointer(window->get(), inputManager.get());
    glfwSetCursorPosCallback(window->get(), utils::InputManager::cursorCallbackWrapper);
    while(!this->window->shouldClose())
    {

        glm::mat4 projection = glm::perspective(glm::radians(camera->zoom),
            static_cast<float>(constants::DEFAULT_WINDOW_WIDTH) / static_cast<float>(constants::DEFAULT_WINDOW_HEIGHT),
            0.1f,
            100.0f);
        glm::mat4 view = camera->getViewMatrix();

        this->renderer->getCurrentShader()->setMat4("projection", projection);
        this->renderer->getCurrentShader()->setMat4("view", view);
        this->renderer->getCurrentShader()->use();

        inputManager->processInput(utils::DeltaTime::get(static_cast<float>(glfwGetTime())), window->get());
        this->renderer->update();
        this->window->update();
    }

    this->window->terminate();
}
