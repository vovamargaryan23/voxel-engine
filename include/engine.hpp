#pragma once
#include <memory>
#include <iostream>
#include "glad/glad.h"
#include "platform/window.hpp"
#include "platform/camera.hpp"
#include "utils/shader.hpp"
#include "utils/input_manager.hpp"

class Engine
{
private:
    std::unique_ptr<platform::Window> window;
    std::unique_ptr<platform::Camera> camera;
    std::unique_ptr<utils::InputManager> inputManager;
public:
    Engine();
    void Start() const;
};