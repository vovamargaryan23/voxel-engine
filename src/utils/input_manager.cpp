#include "utils/input_manager.hpp"

namespace utils {
    void InputManager::processInput(const float deltaTime, GLFWwindow* window) const {
        if (glfwGetKey(window, GLFW_KEY_W)            == GLFW_PRESS)
            camera.processKeyboard(platform::FORWARD,  deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S)            == GLFW_PRESS)
            camera.processKeyboard(platform::BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A)            == GLFW_PRESS)
            camera.processKeyboard(platform::LEFT,     deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D)            == GLFW_PRESS)
            camera.processKeyboard(platform::RIGHT,    deltaTime);
        if (glfwGetKey(window, GLFW_KEY_SPACE)        == GLFW_PRESS)
            camera.processKeyboard(platform::UP,       deltaTime);
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)   == GLFW_PRESS)
            camera.processKeyboard(platform::DOWN,     deltaTime);
        if (glfwGetKey(window, GLFW_KEY_ESCAPE)       == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    void InputManager::processMouse(GLFWwindow*, const double xPosIn, const double yPosIn) {
        const auto xPos = static_cast<float>(xPosIn);
        const auto yPos = static_cast<float>(yPosIn);

        if (firstMouse) {
            lastX      = xPos;
            lastY      = yPos;
            firstMouse = false;
        }

        camera.processMouseMovement(xPos - lastX, lastY - yPos);
        lastX = xPos;
        lastY = yPos;
    }
}
