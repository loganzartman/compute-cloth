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

    //  Window, mouse, keyboard attributes
    GLFWwindow* window;
    std::unordered_map<int, bool> key_pressed;
    glm::vec2 mouse_position = glm::vec2(-1,-1);
    glm::vec2 mouse_prev = glm::vec2(-1,-1);
    glm::vec2 mouse_pos_vector = glm::vec2(0,0);
    float pitch = 0;
    float yaw = 0;
    glm::vec3 eye = glm::vec3(0,0,-5);
    glm::vec3 look = glm::vec3(0,0,0);
    glm::vec3 up = glm::vec3(0,1,0);

    gfx::Program skybox_program = gfx::Program("skybox");
    gfx::VAO skybox;
};
