#pragma once

#include <vector>
#include <memory>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/gtx/string_cast.hpp>

#include "shader.hpp"
#include "../utilities/debug.hpp"
#include "font.hpp"

class Renderer {
    struct SpriteVertex {
        glm::vec2 position;
        glm::vec2 uv;
        f32 layer;
    };
    struct TextVertex {
        glm::vec2 position;
        glm::vec2 uv;
        glm::vec4 color;
        f32 layer;
    };
    struct LineVertex {
        glm::vec2 position;
        glm::vec4 color;
    };
    enum class VertexAttribute {
        FLOAT1 = 1,
        FLOAT2 = 2,
        FLOAT4 = 4,
    };
public:
    Renderer(cref<glm::uvec2> window_size, Texture& texture);
    ~Renderer();

    void render();
    void draw_quad(const glm::vec2& position, const glm::vec2& size, u32 layer_index, const glm::mat4x2& uv);
    void draw_line(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color);
    void draw_quad_lines(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
    void draw_text(const glm::vec2& position, std::string_view text, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

    void set_line_width(f32 width);
    void set_clear_color(const glm::vec4& color);
private:
    static constexpr f32 SCALE = 2.0f; // TODO: maybe fixed-size rendering size?
    static constexpr u32 MAX_OBJECTS = 1000, MAX_VERTICES = MAX_OBJECTS * 4, MAX_INDICES = MAX_OBJECTS * 6;

    Font font;

    u32 index_buffer; // because it has fixed size it can be shared between different vaos

    // sprite renderer
    Shader sprite_shader;
    u32 sprite_vertex_array, sprite_vertex_buffer;
    u32 sprite_index_count;
    std::vector<SpriteVertex> sprite_vertices;

    // text renderer
    Shader text_shader;
    u32 text_vertex_array, text_vertex_buffer;
    u32 text_index_count;
    std::vector<TextVertex> text_vertices;

    // line renderer
    Shader line_shader;
    u32 line_vertex_array, line_vertex_buffer;
    std::vector<LineVertex> line_vertices;

//    void add(cref<glm::vec2> position, cref<glm::vec2> size, cref<SubTexture> texture);
    void add(const glm::vec2& position, const glm::vec2& size, u32 texture_layer, glm::vec2 uv[4], const glm::vec4& color);
    void set_vertex_array_attributes(u32 vertex_array, const std::vector<VertexAttribute>& attributes);
};
