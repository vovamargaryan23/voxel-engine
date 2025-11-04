#include "utils/input_manager.hpp"

namespace utils {
    void InputManager::processInput(const float deltaTime, GLFWwindow* window) const {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.processKeyboard(platform::CameraMovement::FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.processKeyboard(platform::CameraMovement::BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.processKeyboard(platform::CameraMovement::LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.processKeyboard(platform::CameraMovement::RIGHT, deltaTime);
    }

    void InputManager::processMouse(GLFWwindow *window, const double xPosIn, const double yPosIn) {
        const auto xPos = static_cast<float>(xPosIn);
        const auto yPos = static_cast<float>(yPosIn);

        if (firstMouse)
        {
            lastX = xPos;
            lastY = yPos;
            firstMouse = false;
        }

        const float xOffset = xPos - lastX;
        const float yOffset = lastY - yPos;

        lastX = xPos;
        lastY = yPos;

        camera.processMouseMovement(xOffset, yOffset);
    }
};