#pragma once

#include <fstream>
#include <string>
#include <cmath>
#include <glm/glm.hpp>

static std::string file_read(std::string path) {
    /// http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
    if (path == "") return "";
    std::ifstream f(path, std::ios::in | std::ios::binary);
    if (f) {
        std::string contents;
        f.seekg(0, std::ios::end);  // read to the end of the file
        contents.resize(f.tellg()); // get position of the last character (size of file)

        f.seekg(0, std::ios::beg); // go to beginning
        f.read(&contents[0], contents.size());
        f.close(); // read whole file and close

        return contents;
    }
    return "";
}

template <typename II>
static std::string join(II b, II e, std::string delim) {
    std::stringstream s;
    bool first = true;
    for (auto it = b; it != e; ++it) {
        if (first) { first = false; }
        else { s << delim; }
        s << *it;
    }
    return s.str();
}

static void create_sphere(float radius, std::vector<glm::vec3> &s_vertices, std::vector<glm::uvec3>& sphere_indices, uint resolution=20) {
    int stacks = resolution;
    int slices = resolution;
    s_vertices.clear();

    for (int i = 0; i <= stacks; ++i) {
        float V = i / (float)stacks;
        float phi = V * glm::pi<float>();
        for (int j = 0; j <= slices; ++j) {
        float U = j / (float)slices;
        float theta = U * (glm::pi<float>() * 2);
        float x = cosf(theta) * sinf(phi);
        float y = cosf(phi);
        float z = sinf(theta) * sinf(phi);
        s_vertices.push_back(glm::vec3(x, y, z) * radius);
        }
    }

    for (int i = 0; i < slices * stacks + slices; ++i) {
        sphere_indices.emplace_back(glm::uvec3(i, i + slices + 1, i + slices));
        sphere_indices.emplace_back(glm::uvec3(i + slices + 1, i, i + 1));
    }
}
