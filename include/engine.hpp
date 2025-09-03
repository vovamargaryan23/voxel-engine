#pragma once
#include <memory>
#include <iostream>
#include "glad/glad.h"
#include "platform/Window.hpp"
#include "utils/shader.hpp"

class Engine
{
private:
    std::unique_ptr<platform::Window> window;
public:
    Engine();
    void Start() const;
};