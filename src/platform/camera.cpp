#include "platform/camera.hpp"

namespace platform {
    Camera::Camera(glm::vec3 position_, glm::vec3 up_, float yaw_, float pitch_)
        : position(position_), yaw(yaw_), pitch(pitch_),
          movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM),
          front(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f),
          right(1.0f, 0.0f, 0.0f), worldUp(up_) {
        updateCameraVectors();
    }

    Camera::Camera(float posX, float posY, float posZ,
                   float upX,  float upY,  float upZ,
                   float yaw_, float pitch_)
        : position(posX, posY, posZ), yaw(yaw_), pitch(pitch_),
          movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM),
          front(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f),
          right(1.0f, 0.0f, 0.0f), worldUp(upX, upY, upZ) {
        updateCameraVectors();
    }

    glm::mat4 Camera::getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }

    void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
        float velocity = movementSpeed * deltaTime;
        switch (direction) {
            case FORWARD:  position += front   * velocity; break;
            case BACKWARD: position -= front   * velocity; break;
            case LEFT:     position -= right   * velocity; break;
            case RIGHT:    position += right   * velocity; break;
            case UP:       position += worldUp * velocity; break;
            case DOWN:     position -= worldUp * velocity; break;
        }
    }

    void Camera::processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch) {
        xOffset *= mouseSensitivity;
        yOffset *= mouseSensitivity;
        yaw   += xOffset;
        pitch += yOffset;

        if (constrainPitch) {
            if (pitch >  89.0f) pitch =  89.0f;
            if (pitch < -89.0f) pitch = -89.0f;
        }
        updateCameraVectors();
    }

    void Camera::updateCameraVectors() {
        glm::vec3 f;
        f.x   = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        f.y   = sin(glm::radians(pitch));
        f.z   = sin(glm::radians(yaw))  * cos(glm::radians(pitch));
        front = glm::normalize(f);
        right = glm::normalize(glm::cross(front, worldUp));
        up    = glm::normalize(glm::cross(right, front));
    }
}
