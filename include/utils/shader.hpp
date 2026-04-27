#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <glm/glm.hpp>
#include "glad/glad.h"

namespace utils {
    class Shader {
    private:
        GLuint programId;
        mutable std::unordered_map<std::string, GLint> locationCache;

        GLuint createShader(GLenum shaderType, const char *shaderCode);
        GLint getUniformLocation(const std::string &name) const;

    public:
        Shader(const std::string &vertexPath, const std::string &fragmentPath);
        void use() const;
        void setMat4(const std::string &name, const glm::mat4 &mat) const;
        ~Shader();
    };
}
