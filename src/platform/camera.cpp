#include "platform/camera.hpp"

namespace platform {
    Camera::Camera(glm::vec3 position_, glm::vec3 up_, float yaw_, float pitch_)
        :front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
        {
            position = position_;
            worldUp = up_;
            yaw = yaw_;
            pitch = pitch_;
            updateCameraVectors();
        }

    Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw_, float pitch_)
        : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM)
        {
            position = glm::vec3(posX, posY, posZ);
            worldUp = glm::vec3(upX, upY, upZ);
            yaw = yaw_;
            pitch = pitch_;
            updateCameraVectors();
        }

    glm::mat4 Camera::getViewMatrix() const {
        return glm::lookAt(position, position + front, up);
    }

    void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
        float velocity = movementSpeed * deltaTime;
        if (direction == FORWARD)
            position += front * velocity;
        if (direction == BACKWARD)
            position -= front * velocity;
        if (direction == LEFT)
            position -= right * velocity;
        if (direction == RIGHT)
            position += right * velocity;
    }

    void Camera::processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch) {
        xOffset *= mouseSensitivity;
        yOffset *= mouseSensitivity;

        yaw += xOffset;
        pitch += yOffset;

        if (constrainPitch) {
            if (pitch > 89.0) {
                pitch = 89.0;
            }
            if (pitch < -89.0) {
                pitch = -89.0;
            }
        }

        updateCameraVectors();
    }

    void Camera::updateCameraVectors() {
        glm::vec3 front_;
        front_.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front_.y = sin(glm::radians(pitch));
        front_.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(front_);
        right = glm::normalize(glm::cross(front, worldUp));
        up = glm::normalize(glm::cross(right, front));
    }
};