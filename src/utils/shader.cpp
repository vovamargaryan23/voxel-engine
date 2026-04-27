#include "utils/shader.hpp"

namespace utils {
    Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
        std::string vertexCode;
        std::string fragmentCode;

        std::ifstream vertexFile(vertexPath);
        std::ifstream fragmentFile(fragmentPath);

        vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            std::stringstream vertexStream, fragmentStream;
            vertexStream << vertexFile.rdbuf();
            fragmentStream << fragmentFile.rdbuf();
            vertexCode = vertexStream.str();
            fragmentCode = fragmentStream.str();
        } catch (std::ifstream::failure const &e) {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }

        GLuint vertexShader = createShader(GL_VERTEX_SHADER, vertexCode.c_str());
        GLuint fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentCode.c_str());

        this->programId = glCreateProgram();
        glAttachShader(this->programId, vertexShader);
        glAttachShader(this->programId, fragmentShader);
        glLinkProgram(this->programId);

        int success;
        char infoLog[512];
        glGetProgramiv(this->programId, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(this->programId, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER PROGRAM LINK FAILED: " << infoLog << std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    GLuint Shader::createShader(GLenum shaderType, const char *shaderCode) {
        int success;
        char infoLog[512];

        GLuint shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &shaderCode, nullptr);
        glCompileShader(shader);
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(shader, 512, nullptr, infoLog);
            std::cerr << "ERROR::SHADER COMPILATION FAILED: " << infoLog << std::endl;
        }

        return shader;
    }

    GLint Shader::getUniformLocation(const std::string &name) const {
        auto it = locationCache.find(name);
        if (it != locationCache.end())
            return it->second;
        const GLint location = glGetUniformLocation(this->programId, name.c_str());
        locationCache[name] = location;
        return location;
    }

    void Shader::use() const {
        glUseProgram(this->programId);
    }

    void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
    }

    Shader::~Shader() {
        glDeleteProgram(this->programId);
    }
}
