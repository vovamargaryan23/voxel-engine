#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glad/glad.h"

class Shader
{
private:
    int programId;
    GLuint createShader(GLenum shaderType, const char* shaderCode);
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    void use();
};