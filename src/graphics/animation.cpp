#include "animation.hpp"
#include "../utilities/debug.hpp"

Animation::Animation(cref<TextureLayer> sprite_sheet, f32 duration, i32 row, std::vector<i32> columns, u8 frame_count) : sprite_sheet(sprite_sheet), frame_count(frame_count), current_frame(0), current_frame_time(0.0f) {
    if (frame_count > MAX_FRAMES) {
        debug.error("Tried creating animation but frame_count ({}) is greater than MAX_FRAMES ({})", frame_count, MAX_FRAMES);
        return;
    }

    for (u8 i = 0; i < frame_count; i++) {
        frames[i].duration = duration;
        frames[i].row = row;
        frames[i].column = columns[i];
    }
}

glm::mat4x2 Animation::get_current_uv() const {
    auto& frame = frames[current_frame];
    return sprite_sheet.get_uv(frame.row, frame.column);
}

void Animation::update(f32 delta_time) {
    current_frame_time += delta_time;
    if (current_frame_time >= frames[current_frame].duration) {
        current_frame_time = 0.0f;
        current_frame = (current_frame + 1) % frame_count;
    }
}
