#include <glad/glad.h>
#include <glm/mat4x2.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "renderer.hpp"

Renderer::Renderer(cref<glm::uvec2> window_size, Texture& texture) :
    font(texture, "upheaval.ttf"),
    sprite_shader("sprite.vert", "sprite.frag"),
    sprite_index_count(0),
    text_shader("text.vert", "text.frag"),
    text_index_count(0),
    line_shader("line.vert", "line.frag") {
    debug.info("Creating renderer: {} scale, {} max objects, {} max vertices, {} max indices", SCALE, MAX_OBJECTS, MAX_VERTICES, MAX_INDICES);
    sprite_vertices.reserve(MAX_VERTICES);
    text_vertices.reserve(MAX_VERTICES);
    line_vertices.reserve(MAX_VERTICES);

    // ibo
    u32 indices[MAX_INDICES];
    u32 index_offset = 0;

    for (u32 i = 0; i < MAX_INDICES; i += 6, index_offset += 4) {
        indices[i] = index_offset;
        indices[i + 1] = index_offset + 1;
        indices[i + 2] = index_offset + 2;
        indices[i + 3] = index_offset + 2;
        indices[i + 4] = index_offset + 3;
        indices[i + 5] = index_offset;
    }

    debug.info("Creating index buffer with size {} bytes", MAX_INDICES * sizeof(u32));
    glCreateBuffers(1, &index_buffer);
    glNamedBufferStorage(index_buffer, MAX_INDICES * sizeof(u32), indices, 0);

    // sprite renderer
    // vbo
    debug.info("Creatng sprite vertex buffer with size {} bytes", MAX_VERTICES * sizeof(SpriteVertex));
    glCreateBuffers(1, &sprite_vertex_buffer);
    glNamedBufferStorage(sprite_vertex_buffer, MAX_VERTICES * sizeof(SpriteVertex), nullptr, GL_DYNAMIC_STORAGE_BIT);

    // vao
    glCreateVertexArrays(1, &sprite_vertex_array);
    glVertexArrayVertexBuffer(sprite_vertex_array, 0, sprite_vertex_buffer, 0, sizeof(SpriteVertex));
    glVertexArrayElementBuffer(sprite_vertex_array, index_buffer);
    set_vertex_array_attributes(sprite_vertex_array, {
        VertexAttribute::FLOAT2,
        VertexAttribute::FLOAT2,
        VertexAttribute::FLOAT1,
    });

    // text renderer
    // vbo
    debug.info("Creating text vertex buffer with size {} bytes", MAX_VERTICES * sizeof(TextVertex));
    glCreateBuffers(1, &text_vertex_buffer);
    glNamedBufferStorage(text_vertex_buffer, MAX_VERTICES * sizeof(TextVertex), nullptr, GL_DYNAMIC_STORAGE_BIT);

    // vao
    glCreateVertexArrays(1, &text_vertex_array);
    glVertexArrayVertexBuffer(text_vertex_array, 0, text_vertex_buffer, 0, sizeof(TextVertex));
    glVertexArrayElementBuffer(text_vertex_array, index_buffer);
    set_vertex_array_attributes(text_vertex_array, {
        VertexAttribute::FLOAT2,
        VertexAttribute::FLOAT2,
        VertexAttribute::FLOAT4,
        VertexAttribute::FLOAT1,
    });

    // line renderer
    // vbo
    debug.info("Creating line vertex buffer with size {} bytes", MAX_VERTICES * sizeof(LineVertex));
    glCreateBuffers(1, &line_vertex_buffer);
    glNamedBufferStorage(line_vertex_buffer, MAX_VERTICES * sizeof(LineVertex), nullptr, GL_DYNAMIC_STORAGE_BIT);

    // vao
    glCreateVertexArrays(1, &line_vertex_array);
    glVertexArrayVertexBuffer(line_vertex_array, 0, line_vertex_buffer, 0, sizeof(LineVertex));
    set_vertex_array_attributes(line_vertex_array, {
        VertexAttribute::FLOAT2,
        VertexAttribute::FLOAT4,
    });

    // shaders
    // TODO: uniform buffer object
    auto projection = glm::ortho(0.0f, window_size.x / SCALE, window_size.y / SCALE, 0.0f, -1.0f, 1.0f);

    sprite_shader.bind(true);
    sprite_shader.set_uniform("u_projection", projection);
    sprite_shader.set_uniform("u_textures", 0);
    sprite_shader.bind(false);

    text_shader.bind(true);
    text_shader.set_uniform("u_projection", projection);
    text_shader.set_uniform("u_textures", 0);
    text_shader.bind(false);

    line_shader.bind(true);
    line_shader.set_uniform("u_projection", projection);
    line_shader.bind(false);
}

Renderer::~Renderer() {
    glDeleteBuffers(1, &index_buffer);

    // sprite
    glDeleteVertexArrays(1, &sprite_vertex_array);
    glDeleteBuffers(1, &sprite_vertex_buffer);

    // text
    glDeleteVertexArrays(1, &text_vertex_array);
    glDeleteBuffers(1, &text_vertex_buffer);

    // line
    glDeleteVertexArrays(1, &line_vertex_array);
    glDeleteBuffers(1, &line_vertex_buffer);
}

void Renderer::render() {
    // sprite renderer
    if (sprite_index_count != 0) {
        sprite_shader.bind(true);
        glNamedBufferSubData(sprite_vertex_buffer, 0, sprite_vertices.size() * sizeof(SpriteVertex), sprite_vertices.data());
        glBindVertexArray(sprite_vertex_array);
        glDrawElements(GL_TRIANGLES, sprite_index_count, GL_UNSIGNED_INT, nullptr);

        sprite_index_count = 0;
        sprite_vertices.clear();
    }

    // text renderer
    if (text_index_count != 0) {
        text_shader.bind(true);
        glNamedBufferSubData(text_vertex_buffer, 0, text_vertices.size() * sizeof(TextVertex), text_vertices.data());
        glBindVertexArray(text_vertex_array);
        glDrawElements(GL_TRIANGLES, text_index_count, GL_UNSIGNED_INT, nullptr);

        text_index_count = 0;
        text_vertices.clear();
    }

    // line renderer
    if (!line_vertices.empty()) {
        line_shader.bind(true);
        glNamedBufferSubData(line_vertex_buffer, 0, line_vertices.size() * sizeof(LineVertex), line_vertices.data());
        glBindVertexArray(line_vertex_array);
        glDrawArrays(GL_LINES, 0, line_vertices.size());

        line_vertices.clear();
    }
}

void Renderer::draw_quad(const glm::vec2& position, const glm::vec2& size, u32 layer_index, const glm::mat4x2& uv) {
    constexpr glm::vec2 vertex_positions[] = {
        { -0.5f, -0.5f },
        { 0.5f, -0.5f },
        { 0.5f, 0.5f },
        { -0.5f, 0.5f },
    };

    if (sprite_index_count >= MAX_INDICES)
        render();

    if (layer_index == -1) {
        debug.warn("Tried to render sprite but it's texture index is -1");
        return;
    }

    auto modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, { position, 0.0f });
    modelMatrix = glm::scale(modelMatrix, { size, 1.0f });

    for (u32 i = 0; i < 4; i++) {
        auto transformed_position = modelMatrix * glm::vec4(vertex_positions[i], 0.0f, 1.0f);
        sprite_vertices.emplace_back(transformed_position, uv[i], (f32)layer_index);
    }

    sprite_index_count += 6;
}

void Renderer::draw_line(const glm::vec2& start, const glm::vec2& end, const glm::vec4& color) {
    if (line_vertices.size() >= MAX_VERTICES)
        render();

    line_vertices.push_back({ start, color });
    line_vertices.push_back({ end, color });
}

void Renderer::draw_quad_lines(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
    glm::vec2 p0 = glm::vec2(position.x - size.x * 0.5f, position.y - size.y * 0.5f);
    glm::vec2 p1 = glm::vec2(position.x + size.x * 0.5f, position.y - size.y * 0.5f);
    glm::vec2 p2 = glm::vec2(position.x + size.x * 0.5f, position.y + size.y * 0.5f);
    glm::vec2 p3 = glm::vec2(position.x - size.x * 0.5f, position.y + size.y * 0.5f);

    draw_line(p0, p1, color);
    draw_line(p1, p2, color);
    draw_line(p2, p3, color);
    draw_line(p3, p0, color);
}

void Renderer::draw_text(const glm::vec2& position, std::string_view text, const glm::vec4& color) { // TODO: kerning
    auto layer = font.get_texture()->index;
    auto& texture_size = font.get_texture()->parent_size; // because all layers are set to maximum size of all layers

    auto x = position.x;
    for (auto c : text) {
        auto& glyph = font.get_glyph(c);

        glm::vec2 min = { glyph.offset.x / (f32)texture_size.x, glyph.offset.y / (f32)texture_size.y };
        glm::vec2 max = { (glyph.offset.x + glyph.size.x) / (f32)texture_size.x, (glyph.offset.y + glyph.size.y) / (f32)texture_size.y };
        glm::vec2 uv[] = {
                { min.x, min.y },
                { max.x, min.y },
                { max.x, max.y },
                { min.x, max.y },
        };

        add({ x + glyph.bearing.x, position.y + (font.get_max_ascent() - glyph.bearing.y) }, glyph.size, layer, uv, color);

        x += glyph.x_advance;
    }
}

void Renderer::set_line_width(f32 width) {
    glLineWidth(width);
}

void Renderer::set_clear_color(const glm::vec4 &color) {
    glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::add(const glm::vec2 &position, const glm::vec2 &size, u32 texture_layer, glm::vec2 uv[4], const glm::vec4& color) {
    constexpr glm::vec2 vertex_positions[] = { // left-top origin for text only
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f },
        { 0.0f, 1.0f },
    };

    if (text_index_count >= MAX_INDICES)
        render();

    if (texture_layer == -1) {
        debug.warn("Tried to render sprite but it's texture index is -1");
        return;
    }

    auto modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, { position, 0.0f });
    modelMatrix = glm::scale(modelMatrix, { size, 1.0f });

    for (u32 i = 0; i < 4; i++) {
        auto transformed_position = modelMatrix * glm::vec4(vertex_positions[i], 0.0f, 1.0f);
        text_vertices.emplace_back(transformed_position, uv[i], color, (f32)texture_layer);
    }

    text_index_count += 6;
}

void Renderer::set_vertex_array_attributes(u32 vertex_array, const std::vector<VertexAttribute>& attributes) {
    auto offset = 0u;

    for (u32 i = 0; i < attributes.size(); i++) {
        auto count = (i32)attributes[i];

        glEnableVertexArrayAttrib(vertex_array, i);
        glVertexArrayAttribFormat(vertex_array, i, count, GL_FLOAT, GL_FALSE, offset);
        glVertexArrayAttribBinding(vertex_array, i, 0);

        offset += count * sizeof(f32);
    }
}
