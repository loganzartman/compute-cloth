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

    const uint sub_steps = 5;
    const glm::uvec2 cloth_dimension = glm::uvec2(25,25);

    //  Window, mouse, keyboard attributes
    GLFWwindow* window;
    std::unordered_map<int, bool> key_pressed;
    glm::vec2 mouse_position = glm::vec2(-1,-1);
    glm::vec2 mouse_prev = glm::vec2(-1,-1);
    glm::vec2 mouse_pos_vector = glm::vec2(0,0);
    glm::vec3 sphere_pos = glm::vec3(1.41,-10,-8);
    float pitch = 0;
    float yaw = 1.57;
    float mouse_speed = 1;
    float prev_time = glfwGetTime();
    float cam_dist = -20.0f;
    bool mouse_pressed = false;
    bool moving = false;
    bool freeze_sphere = false;
    bool wireframe = false;
    bool enable_specular = false;
    glm::vec3 eye = glm::vec3(0,0,cam_dist);
    glm::vec3 look = glm::vec3(0,0,0);
    glm::vec3 up = glm::vec3(0,1,0);
    gfx::Program cloth_program = gfx::Program("cloth");
    gfx::Program cloth_verlet_program = gfx::Program("compute_verlet");
    gfx::Program cloth_constraints_program = gfx::Program("compute_constraints");
    gfx::Program cloth_apply_accel_program = gfx::Program("compute_apply_accel");
    GLuint cloth_ssbo_id = 0;
    gfx::VAO cloth;
    std::vector<uint> cloth_indices;

    gfx::Program skybox_program = gfx::Program("skybox");
    gfx::VAO skybox;

    gfx::Program sphere_program = gfx::Program("sphere");
    gfx::VAO sphere;
    std::vector<glm::uvec3> sphere_indices;
};
