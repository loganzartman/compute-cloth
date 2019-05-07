#include <iostream>
#include <random>
#include <cmath>
#include <array>
#include <chrono>

#include <glad/glad.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include "game.h"
#include "gfx/program.h"
#include "gfx/vao.h"

using namespace std::chrono;

void Game::init() {
    skybox_program.vertex({"skybox.vs"}).fragment({"perlin.glsl", "skybox.fs"}).compile();
    cloth_program.vertex({"cloth.vs"}).fragment({"cloth.fs"}).compile();

    // generate cloth vertices
    cloth.add_attribs({3});
    std::vector<glm::vec3> cloth_vertices;
    for (int i=-5; i<5; i++) {
        for (int j=-5; j<5; j++) {
            cloth_vertices.push_back(glm::vec3(i,j,0));
        }
    }
    cloth.vertices.set_data(cloth_vertices);

    // generate indices for cloth triangles
    auto cloth_index = [](int i, int j){return i*10+j;};
    for (int i=0; i<9; ++i) {
        for (int j=0; j<9; ++j) {
            // top right triangle
            cloth_indices.push_back(cloth_index(i,j));
            cloth_indices.push_back(cloth_index(i+1,j+1));
            cloth_indices.push_back(cloth_index(i+1,j));

            // top right triangle
            cloth_indices.push_back(cloth_index(i,j));
            cloth_indices.push_back(cloth_index(i,j+1));
            cloth_indices.push_back(cloth_index(i+1,j+1));
        }
    }

    skybox.add_attribs({3});
    skybox.vertices.set_data(std::vector<glm::vec3>{
        {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 1.0f}, 
        {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f},
        {1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}
    });
    handleResize();
}

void Game::handleResize() {
}

void Game::update() {
    int window_w, window_h;
    glfwGetFramebufferSize(window, &window_w, &window_h);

    glm::mat4 view_matrix = glm::lookAt(eye, look, up);
    glm::mat4 projection_matrix = glm::perspective(
        glm::radians(80.f),
        ((float)window_w)/window_h,
        0.1f,
        1000.f
    );

    bool first = mouse_prev == glm::vec2(-1,-1);
    if (first) {
        mouse_prev = mouse_position;
    } else {
        mouse_pos_vector = -(mouse_position - mouse_prev);
        mouse_prev = mouse_position;
    }

    glViewport(0, 0, window_w, window_h);
    glClearColor(0.f,0.f,0.f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    // glEnable(GL_BLEND);

    skybox_program.use();
    const glm::mat4 skybox_transform = 
        glm::translate(eye) * 
        glm::scale(glm::vec3(1000.f, 1000.f, 1000.f)) * 
        glm::translate(glm::vec3(-0.5f, -0.5f, -0.5f));
    glUniformMatrix4fv(skybox_program.uniform_loc("projection"), 1, false, glm::value_ptr(projection_matrix));
    glUniformMatrix4fv(skybox_program.uniform_loc("view"), 1, false, glm::value_ptr(view_matrix));
    glUniformMatrix4fv(skybox_program.uniform_loc("transform"), 1, false, glm::value_ptr(skybox_transform));
    glUniform1f(skybox_program.uniform_loc("time"), glfwGetTime());
    glUniform3fv(skybox_program.uniform_loc("camera_position"), 1, glm::value_ptr(eye));
    skybox.bind();
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, std::array<uint, 36>{
        0, 3, 2, 2, 1, 0, 0, 5, 4, 4, 3, 0, 0, 1, 6, 6, 5, 0, 5, 6, 7, 7,
        4, 5, 1, 2, 7, 7, 6, 1, 3, 4, 7, 7, 2, 3
    }.data());
    skybox.unbind();

    // Turn on wireframe mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    cloth_program.use();
    glUniformMatrix4fv(cloth_program.uniform_loc("projection"), 1, false, glm::value_ptr(projection_matrix));
    glUniformMatrix4fv(cloth_program.uniform_loc("view"), 1, false, glm::value_ptr(view_matrix));
    cloth.bind();
    glDrawElements(GL_TRIANGLES, cloth_indices.size(), GL_UNSIGNED_INT, cloth_indices.data());
    cloth.unbind();

    // Turn off wireframe mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

