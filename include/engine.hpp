#pragma once
#include <memory>
#include <iostream>

#include "platform/camera.hpp"
#include "platform/window.hpp"
#include "platform/renderer.hpp"

#include "utils/shader.hpp"
#include "utils/input_manager.hpp"
#include "utils/delta_time.hpp"

class Engine
{
private:
    std::unique_ptr<platform::Window> window;
    std::unique_ptr<platform::Camera> camera;
    std::unique_ptr<platform::Renderer> renderer;
    std::unique_ptr<utils::InputManager> inputManager;

public:
    Engine();
    void Start() const;
};