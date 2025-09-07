#pragma once

namespace utils {
    static float deltaTime = 0.0f;
    static float lastFrame = 0.0f;

    class DeltaTime {
    public:
        static float get(float currentFrame) {
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            return deltaTime;
        }
    };
};