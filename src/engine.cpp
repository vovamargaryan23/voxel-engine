#include "engine.hpp"
#include "utils/config.hpp"
#include "utils/logger.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <algorithm>

Engine::Engine() {
    // Load config first so everything else can query it
    utils::Cfg().load(std::string(ASSETS_DIR) + "/config.ini");

    window         = std::make_unique<platform::Window>();
    camera         = std::make_unique<platform::Camera>(glm::vec3(16.0f, 30.0f, 16.0f));
    camera->movementSpeed    = utils::Cfg().getFloat("camera.speed",       10.0f);
    camera->mouseSensitivity = utils::Cfg().getFloat("camera.sensitivity", 0.1f);

    renderer       = std::make_unique<platform::Renderer>();   // GLAD init
    worldGenerator = std::make_unique<WorldGenerator>();       // needs OpenGL
    inputManager   = std::make_unique<utils::InputManager>(*camera);
}

void Engine::Start() const {
    renderer->init(*worldGenerator);

    glfwSetWindowUserPointer(window->get(), inputManager.get());
    glfwSetCursorPosCallback(window->get(), utils::InputManager::cursorCallbackWrapper);

    auto collides = [&](const glm::vec3& pos) -> bool {
        constexpr float R  = 0.3f;
        constexpr float DY[] = {0.0f, 0.9f, 1.79f};
        for (float dy : DY) {
            for (float dx : {-R, R}) {
                for (float dz : {-R, R}) {
                    glm::vec3 p = pos + glm::vec3(dx, -dy, dz);
                    if (worldGenerator->isSolid(
                            static_cast<int>(std::floor(p.x)),
                            static_cast<int>(std::floor(p.y)),
                            static_cast<int>(std::floor(p.z))))
                        return true;
                }
            }
        }
        return false;
    };

    while (!window->shouldClose()) {
        const float dt = utils::DeltaTime::get(static_cast<float>(glfwGetTime()));

        // Stream chunks around camera
        worldGenerator->update(camera->position);

        // Input + per-axis collision
        const glm::vec3 oldPos = camera->position;
        inputManager->processInput(dt, window->get());
        const glm::vec3 newPos = camera->position;

        camera->position = oldPos;
        if (!collides({newPos.x, oldPos.y, oldPos.z})) camera->position.x = newPos.x;
        if (!collides({camera->position.x, newPos.y, oldPos.z})) camera->position.y = newPos.y;
        if (!collides({camera->position.x, camera->position.y, newPos.z})) camera->position.z = newPos.z;

        // Matrices — use live framebuffer size for correct aspect ratio after resize
        int fbW, fbH;
        glfwGetFramebufferSize(window->get(), &fbW, &fbH);
        const float aspect = fbH > 0 ? static_cast<float>(fbW) / fbH : 1.0f;

        const glm::mat4 model      = glm::mat4(1.0f);
        const glm::mat4 projection = glm::perspective(glm::radians(camera->zoom), aspect, 0.1f, 1000.0f);
        const glm::mat4 view       = camera->getViewMatrix();
        const glm::mat4 vp         = projection * view;

        utils::Shader* shader = renderer->getCurrentShader();
        shader->use();
        shader->setMat4("model",      model);
        shader->setMat4("view",       view);
        shader->setMat4("projection", projection);

        renderer->update(*worldGenerator, vp);
        window->update();
    }

    window->terminate();
}
