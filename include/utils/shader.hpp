#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include "glad/glad.h"

namespace utils {
    class Shader {
    private:
        int programId;

        GLuint createShader(GLenum shaderType, const char *shaderCode);

    public:
        Shader(const std::string &vertexPath, const std::string &fragmentPath);
        void use() const;
        void setMat4(const std::string &name, const glm::mat4 &mat) const;
        ~Shader();
    };
};
