#pragma once

#include <unordered_map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "gfx/program.h"
#include "gfx/vao.h"

struct Game {
    Game(GLFWwindow* window) : window(window) {}
    void init();
    void handleResize();
    void update();
    void updateOrientation();

    const glm::uvec2 cloth_dimension = glm::uvec2(10,10);

    //  Window, mouse, keyboard attributes
    GLFWwindow* window;
    std::unordered_map<int, bool> key_pressed;
    glm::vec2 mouse_position = glm::vec2(-1,-1);
    glm::vec2 mouse_prev = glm::vec2(-1,-1);
    glm::vec2 mouse_pos_vector = glm::vec2(0,0);
    float pitch = 0;
    float yaw = 0;
    float mouse_speed = 0.01;
    float prev_time = 0.f;
    bool mouse_pressed = false;
    glm::vec3 eye = glm::vec3(0,0,-10);
    glm::vec3 look = glm::vec3(0,0,0);
    glm::vec3 up = glm::vec3(0,1,0);
    gfx::Program cloth_program = gfx::Program("cloth");
    gfx::Program cloth_compute_program = gfx::Program("cloth_compute");
    gfx::Program cloth_compute_accels_program = gfx::Program("compute_forces.glsl");
    GLuint cloth_ssbo_id = 0;
    gfx::VAO cloth;
    std::vector<uint> cloth_indices;

    gfx::Program skybox_program = gfx::Program("skybox");
    gfx::VAO skybox;
};
