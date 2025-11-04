#pragma once
#include <platform/camera.hpp>

#include "constants.hpp"
#include "GLFW/glfw3.h"

namespace utils {
    class InputManager {
    public:
        explicit InputManager(platform::Camera &camera) : camera(camera) {
        }

        void processMouse(GLFWwindow *window, double xPosIn, double yPosIn);

        void processInput(float deltaTime, GLFWwindow *window) const;

        static void cursorCallbackWrapper(GLFWwindow *window, const double xPos, const double yPos) {
            auto *inputManager = static_cast<InputManager *>(glfwGetWindowUserPointer(window));
            inputManager->processMouse(window, xPos, yPos);
        }

    private:
        float lastX = constants::DEFAULT_WINDOW_WIDTH / 2.0f;
        float lastY = constants::DEFAULT_WINDOW_HEIGHT / 2.0f;
        bool firstMouse = true;
        platform::Camera &camera;
    };
}
