#pragma once

namespace utils {
    class DeltaTime {
    public:
        static float get(float currentFrame) {
            static float lastFrame = 0.0f;
            const float dt = currentFrame - lastFrame;
            lastFrame = currentFrame;
            return dt;
        }
    };
}
