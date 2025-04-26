#pragma once
#include <string>
#include "constants.hpp"
#include "GLFW/glfw3.h"


class Window
{
private:
    GLFWwindow* id;
public:
    void init();
    void update();
    bool shouldClose();
    void terminate();
};