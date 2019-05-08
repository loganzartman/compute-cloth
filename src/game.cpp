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

struct ClothVertex {
    glm::vec3 position;
    float _pad0;
};

void Game::init() {
    skybox_program.vertex({"skybox.vs"}).fragment({"perlin.glsl", "skybox.fs"}).compile();
    cloth_program.vertex({"cloth.vs"}).geometry({"cloth.gs"}).fragment({"cloth.fs"}).compile();
    cloth_compute_program.compute({"cloth_compute.glsl"}).compile();

    // generate cloth vertices
    // note that attribs MUST be vec4 aligned due to buffer packing in compute shader
    cloth.add_attribs({3,1});
    auto cloth_index = [&](int i, int j){return j*cloth_dimension.y+i;};
    std::vector<ClothVertex> cloth_vertices(cloth_dimension.x * cloth_dimension.y);
    for (int i=0; i<cloth_dimension.x; i++) {
        for (int j=0; j<cloth_dimension.y; j++) {
            ClothVertex& cloth_vertex = cloth_vertices[cloth_index(i,j)];
            cloth_vertex.position = (glm::vec3(i,j,0) - glm::vec3(cloth_dimension.x, cloth_dimension.y, 0)*0.5f) * 0.1f;
        }
    }
    cloth.vertices.set_data(cloth_vertices);

    // generate indices for cloth triangles
    for (int i=0; i<cloth_dimension.x-1; ++i) {
        for (int j=0; j<cloth_dimension.y-1; ++j) {
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

    // map cloth VBO to cloth shader storage buffer object
    glGenBuffers(1, &cloth_ssbo_id);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, cloth_ssbo_id);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, cloth.vertices.id);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

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
        updateOrientation();
        mouse_prev = mouse_position;
    }

    // dispatch compute shader to simulate cloth
    cloth_compute_program.use();
    glUniform2uiv(cloth_compute_program.uniform_loc("cloth_dimension"), 2, glm::value_ptr(cloth_dimension));
    glUniform1f(cloth_compute_program.uniform_loc("time"), glfwGetTime());
    glDispatchCompute(cloth_dimension.x,cloth_dimension.y,1); // literally the dimensions of the cloth

    glViewport(0, 0, window_w, window_h);
    glClearColor(0.f,0.f,0.f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
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

    // need barrier synchronization to ensure availability of changes made to vertex buffer
    glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Turn on wireframe mode
    cloth_program.use();
    glUniformMatrix4fv(cloth_program.uniform_loc("projection"), 1, false, glm::value_ptr(projection_matrix));
    glUniformMatrix4fv(cloth_program.uniform_loc("view"), 1, false, glm::value_ptr(view_matrix));
    cloth.bind();
    glDrawElements(GL_TRIANGLES, cloth_indices.size(), GL_UNSIGNED_INT, cloth_indices.data());
    cloth.unbind();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Turn off wireframe mode
}

void Game::updateOrientation() {
    if (glm::length(mouse_pos_vector) == 0) {
		return;}
	mouse_pos_vector.x *= -1.f;
    mouse_pos_vector *= mouse_speed;
    if (abs(pitch + mouse_pos_vector.y) <= 85) {
        pitch += mouse_pos_vector.y;
    }
    yaw += mouse_pos_vector.x;
    
    glm::vec4 base_vector = glm::rotate(-yaw, glm::vec3(0,1,0)) * glm::rotate(-pitch, glm::vec3(1,0,0)) * glm::vec4(0,0,-10,1);
    eye = base_vector; 
}
