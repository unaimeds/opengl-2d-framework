#pragma once

#include "textures.hpp"

#include <vector>

struct AnimationFrame {
    float duration;
    int row, column;
};

class Animation {
public:
    Animation(const TextureLayer& sprite_sheet, float duration, int row, std::vector<int> columns);

    void update(float delta_time);

    glm::mat4x2 get_current_uv() const;
private:
    const TextureLayer& sprite_sheet;

    std::vector<AnimationFrame> frames;

    float current_frame_time = 0.0f;
    std::size_t current_frame = 0;
};
