#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gfx/program.h"
#include "gfx/vao.h"

class TexQuad {
public:
    TexQuad() {
        program.vertex({"texquad.vs"}).fragment({"texquad.fs"}).compile();
        const std::vector<float> vertices{
            // vertex, uv
            0, 0, 0, 0,
            1, 0, 1, 0,
            1, 1, 1, 1,
            1, 1, 1, 1,
            0, 1, 0, 1,
            0, 0, 0, 0
        };

        vao.add_attribs({2, 2});
        vao.vertices.set_data(vertices);
    }
    ~TexQuad() {
        vao.destroy();
    }
    explicit TexQuad(int id) : TexQuad() { set_texture(id); }

    TexQuad& set_texture(GLuint id) {
        tex_id = id;
        return *this;
    }

    TexQuad& set_depth(GLuint id) {
        depth_id = id;
        return *this;
    }

    TexQuad& set_corners(glm::vec2 bottom_left, glm::vec2 top_right) {
        this->bottom_left = bottom_left;
        this->top_right = top_right;
        return *this;
    }

    TexQuad& draw() {
        program.use();
        glUniform2fv(program.uniform_loc("bottom_left"), 1, glm::value_ptr(bottom_left));
        glUniform2fv(program.uniform_loc("top_right"), 1, glm::value_ptr(top_right));
        glUniform1i(program.uniform_loc("write_depth"), depth_id != 0);
        glUniform1i(program.uniform_loc("tex"), 0);
        glUniform1i(program.uniform_loc("depth"), 1);

        vao.bind();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex_id);
        if (depth_id) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, depth_id);
        }
        glDrawArrays(GL_TRIANGLES, 0, vao.vertices.size());
        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        vao.unbind();
        return *this;
    }

    glm::vec2 bottom_left = glm::vec2(-1, -1);
    glm::vec2 top_right = glm::vec2(1, 1);
    GLuint tex_id = 0, depth_id = 0;
    gfx::Program program;
    gfx::VAO vao;
};