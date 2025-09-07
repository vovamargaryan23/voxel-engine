#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "glad/glad.h"

namespace platform {
    constexpr float YAW         = -90.0f;
    constexpr float PITCH       =  0.0f;
    constexpr float SPEED       =  2.5f;
    constexpr float SENSITIVITY =  0.1f;
    constexpr float ZOOM        =  45.0f;

    enum CamerMovement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    class Camera {
    public:
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 worldUp;
        float yaw;
        float pitch;
        float movementSpeed;
        float mouseSensitivity;
        float zoom;

        explicit Camera(glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f),
                        glm::vec3 up_ = glm::vec3(0.0f, 1.0f, 0.0f),
                        float yaw_ = YAW,
                        float pitch_ = PITCH);
        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw_, float pitch_);

        glm::mat4 getViewMatrix() const;
        void processKeyboard(CamerMovement direction, float deltaTime);
        void processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
    private:
        void updateCameraVectors();
    };
}
