#include "core/window.hpp"

void Window::init()
{
    if(!glfwInit())
    {
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    this->id = glfwCreateWindow(constants::WINDOW_WIDTH, 
                                constants::WINDOW_HEIGHT, 
                                constants::WINDOW_TITLE.c_str(), 
                                NULL, 
                                NULL);
    if(!this->id)
    {
        
        glfwTerminate();
        exit(-1);
    }

    glfwSetInputMode(this->id, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  

    glfwMakeContextCurrent(this->id);
}

void Window::update()
{
    glfwSwapBuffers(this->id);
    glfwPollEvents();
}

bool Window::shouldClose()
{
    return glfwWindowShouldClose(this->id);
}

void Window::terminate()
{
    glfwDestroyWindow(this->id);
    glfwTerminate();
}