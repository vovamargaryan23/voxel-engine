#pragma once
#include <string>
#include <stdexcept>
#include "constants.hpp"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace platform {
    class Window {
    public:
        Window(const std::string& title, int32_t width, int32_t height);
        Window() : Window(constants::DEFAULT_WINDOW_TITLE,
                          constants::DEFAULT_WINDOW_WIDTH,
                          constants::DEFAULT_WINDOW_HEIGHT) {}

        void update()       const;
        bool shouldClose()  const;
        void terminate()    const;
        GLFWwindow* get()   const { return id; }
        int getWidth()      const { return width; }
        int getHeight()     const { return height; }

    private:
        std::string  title;
        int32_t      width{};
        int32_t      height{};
        GLFWwindow*  id = nullptr;

        static void framebufferResizeCallback(GLFWwindow* win, int w, int h);
    };
}
