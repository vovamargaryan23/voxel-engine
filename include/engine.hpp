#pragma once
#include <memory>
#include "platform/camera.hpp"
#include "platform/window.hpp"
#include "platform/renderer.hpp"
#include "utils/input_manager.hpp"
#include "utils/delta_time.hpp"
#include "world/world_generator.hpp"

class Engine {
public:
    Engine();
    void Start() const;

private:
    std::unique_ptr<platform::Window>       window;
    std::unique_ptr<platform::Camera>       camera;
    std::unique_ptr<platform::Renderer>     renderer;
    std::unique_ptr<WorldGenerator>         worldGenerator;
    std::unique_ptr<utils::InputManager>    inputManager;
};
