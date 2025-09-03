#pragma once
#include <string>
#include <stdexcept>
#include "constants.hpp"
#include "GLFW/glfw3.h"


namespace platform {
class Window
{
private:
    std::string title;
    int32_t width{};
    int32_t height{};
    GLFWwindow *id = nullptr;
public:
    Window(const std::string& title, int32_t width, int32_t height);
    Window() : Window(constants::DEFAULT_WINDOW_TITLE, constants::DEFAULT_WINDOW_WIDTH, constants::DEFAULT_WINDOW_HEIGHT) {}
    void update() const;
    bool shouldClose() const;
    void terminate() const;
};
}