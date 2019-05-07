#pragma once

#include <string>
#include <initializer_list>

#include <glad/glad.h>

#include "vbo.h"

namespace gfx {
/**
 * An abstraction for a vertex attribute object that supports instanced 
 * rendering. Contains a buffer for non-instanced vertex data, and, optionally,
 * a buffer for instanced (per-instance) vertex data.
 *
 * Vertex attributes must be added in order (as specified using the layout 
 * specifier in a vertex shader) using either add_attribs() for vertex 
 * attributes which have one set of values for all instances, or 
 * add_instranced_attribs() for attributes which have one set of values for
 * each instance. The size (in vector components) for each attribute must be
 * specified.
 */
class VAO {
public:
    VAO() {}

    VAO& create() {
        glGenVertexArrays(1, &id);
        return *this;
    }

    void destroy() {
        glDeleteVertexArrays(1, &id);
    }

    /**
     * Add non-instanced attributes.
     * Specify the size of each attribute.
     */
    VAO& add_attribs(std::initializer_list<size_t> sizes) {
        add_attribs(sizes, false);
        return *this;
    }

    /**
     * Add instanced attributes.
     * Specify the size of each attribute.
     */
    VAO& add_instanced_attribs(std::initializer_list<size_t> sizes) {
        add_attribs(sizes, true);
        return *this;
    }

    void bind() {
        if (!id) { create(); }
        glBindVertexArray(id);
    }

    void unbind() {
        glBindVertexArray(0);
    }

    GLuint id = 0;
    VBO vertices;  // non-instanced vertex data
    VBO instances; // per-instance data
    size_t attrib_index = 0;

private:
    /**
     * Automatically computes glVertexAttribPointer parameters and sets vertex
     * attribute divisor if using instanced rendering.
     */
    void add_attribs(std::initializer_list<size_t> sizes, bool instanced) {
        bind();
        if (instanced) { instances.bind(); }
        else { vertices.bind(); }

        size_t stride = 0;
        for (size_t size : sizes) {
            stride += size;
        }

        size_t offset = 0;
        for (size_t size : sizes) {
            glEnableVertexAttribArray(attrib_index);
            // arbitrarily represent all data as floats
            glVertexAttribPointer(attrib_index, size, GL_FLOAT, false, stride * sizeof(float), (void*)offset);
            if (instanced) { glVertexAttribDivisor(attrib_index, 1); }
            offset += size * sizeof(float);
            ++attrib_index;
        }

        unbind();
    }
};
}
