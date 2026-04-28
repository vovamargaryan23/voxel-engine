#include "platform/window.hpp"
#include "glad/glad.h"

namespace platform {
    Window::Window(const std::string& title_, const int32_t width_, const int32_t height_)
        : title(title_), width(width_), height(height_) {
        if (!glfwInit())
            throw std::runtime_error("failed to initialize GLFW");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        id = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (!id)
            throw std::runtime_error("failed to create GLFW window");

        glfwSetWindowUserPointer(id, this);
        glfwSetFramebufferSizeCallback(id, framebufferResizeCallback);
        glfwSetInputMode(id, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwMakeContextCurrent(id);
    }

    void Window::framebufferResizeCallback(GLFWwindow* win, int w, int h) {
        glViewport(0, 0, w, h);
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(win));
        self->width  = w;
        self->height = h;
    }

    void Window::update() const {
        glfwSwapBuffers(id);
        glfwPollEvents();
    }

    bool Window::shouldClose() const { return glfwWindowShouldClose(id); }

    void Window::terminate() const {
        glfwDestroyWindow(id);
        glfwTerminate();
    }
}
