#include "animation.hpp"

#include <stdexcept>

Animation::Animation(const TextureLayer& sprite_sheet, float duration, int row, std::vector<int> columns) :
    sprite_sheet(sprite_sheet)
{
    if (columns.empty())
        throw std::invalid_argument("Animation needs at least one frame");

    if (duration <= 0.0f)
        throw std::invalid_argument("Animation frame duration must be greater than zero");

    frames.reserve(columns.size());

    for (const auto column : columns) {
        frames.emplace_back(duration, row, column);
    }
}

void Animation::update(float delta_time) {
    current_frame_time += delta_time;

    if (current_frame_time >= frames[current_frame].duration) {
        current_frame_time -= frames[current_frame].duration;
        current_frame = (current_frame + 1) % frames.size();
    }
}

glm::mat4x2 Animation::get_current_uv() const {
    const auto& frame = frames[current_frame];
    return sprite_sheet.get_uv(frame.row, frame.column);
}
