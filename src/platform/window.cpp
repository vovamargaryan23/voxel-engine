#include "platform/window.hpp"

namespace platform {
    Window::Window(const std::string& title, const int32_t width, const int32_t height) {
        if(!glfwInit())
        {
            throw std::runtime_error("failed to initialize GLFW");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        this->id = glfwCreateWindow(width,
                                    height,
                                title.c_str(),
                                    nullptr,
                                    nullptr);
        if(!this->id)
        {
            throw std::runtime_error("failed to create GLFW window");
        }

        glfwSetInputMode(this->id, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glfwMakeContextCurrent(this->id);
    }

    void Window::update() const {
        glfwSwapBuffers(this->id);
        glfwPollEvents();
    }

    bool Window::shouldClose() const {
        return glfwWindowShouldClose(this->id);
    }

    void Window::terminate() const {
        glfwDestroyWindow(this->id);
        glfwTerminate();
    }

    GLFWwindow* Window::get() const {
        return this->id;
    }
}
