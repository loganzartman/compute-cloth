#pragma once

#include <string>
#include <sstream>

#include <glad/glad.h>

namespace gfx {
class VBO {
public:
    VBO() {}

    VBO& create() {
        glGenBuffers(1, &id);
        return *this;
    }

    void destroy() {
        glDeleteBuffers(1, &id);
    }

    void bind() {
        if (!id) { create(); }
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }

    void unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    template <typename T>
    VBO& set_data(const std::vector<T>& vertices, GLenum usage = GL_STATIC_DRAW) {
        bind();
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), vertices.data(), usage);
        unbind();
        _size = vertices.size();
        return *this;
    }

    template <typename T>
    VBO& update_data(const std::vector<T>& vertices, size_t index) {
        if (vertices.size() + index > size()) {
            std::stringstream str = "VBO update range from ";
            str << index << " to " << vertices.size() << " exceeds length " << size();
            throw std::runtime_error(str.str());
        }
        bind();
        glBufferSubData(GL_ARRAY_BUFFER, index, vertices.size() * sizeof(T), vertices.data());
        unbind();
        return *this;
    }

    size_t size() {
        return _size;
    }

    GLuint id = 0;
private:
    size_t _size = 0;
};
}