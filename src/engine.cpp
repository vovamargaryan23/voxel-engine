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

    while (!this->window->shouldClose())
    {
        const glm::mat4 model      = glm::mat4(1.0f);
        const glm::mat4 projection = glm::perspective(
            glm::radians(camera->zoom),
            static_cast<float>(constants::DEFAULT_WINDOW_WIDTH) / static_cast<float>(constants::DEFAULT_WINDOW_HEIGHT),
            0.1f, 100.0f);
        const glm::mat4 view = camera->getViewMatrix();

        utils::Shader* shader = this->renderer->getCurrentShader();
        shader->use();
        shader->setMat4("model", model);
        shader->setMat4("projection", projection);
        shader->setMat4("view", view);

        inputManager->processInput(utils::DeltaTime::get(static_cast<float>(glfwGetTime())), window->get());
        this->renderer->update();
        this->window->update();
    }

    this->window->terminate();
}
