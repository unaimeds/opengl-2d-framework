#pragma once

#include "textures.hpp"

struct AnimationFrame {
    f32 duration;
    i32 row;
    i32 column;
};

class Animation {
public:
    Animation(cref<TextureLayer> sprite_sheet, f32 duration, i32 row, std::vector<i32> columns, u8 frame_count);

    glm::mat4x2 get_current_uv() const;
    void update(f32 delta_time);
private:
    static constexpr auto MAX_FRAMES = 8;

    cref<TextureLayer> sprite_sheet;
    AnimationFrame frames[MAX_FRAMES];
    u8 frame_count;
    f32 current_frame_time;
    u8 current_frame;
};
