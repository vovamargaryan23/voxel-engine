#pragma once
#include <memory>
#include <iostream>
#include "glad/glad.h"
#include "core/window.hpp"
#include "utils/shader.hpp"

class Engine
{
private:
    std::unique_ptr<Window> window;
public:
    Engine();
    void start();
};