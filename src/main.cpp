#include <iostream>
#include <sstream>
#include <exception>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>

#include "game.h"

/**
 * Called in the event that any GLFW function fails.
 */
void glfw_error_callback(int error, const char* description) {
    std::stringstream str;
    str << "GLFW error " << error << ": " << description;
    throw std::runtime_error(str.str());
}

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
} 

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Game* game = (Game*)glfwGetWindowUserPointer(window);

    if (action == GLFW_PRESS)
        game->key_pressed[key] = true;
    if (action == GLFW_RELEASE)
        game->key_pressed[key] = false;
}

void MousePosCallback(GLFWwindow* window, double mouse_x, double mouse_y) {   
    Game* game = (Game*)glfwGetWindowUserPointer(window);
    if (game->mouse_pressed)
        game->mouse_position = glm::vec2(mouse_x, mouse_y);
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    Game* game = (Game*)glfwGetWindowUserPointer(window);
    if (button == GLFW_MOUSE_BUTTON_LEFT && action != GLFW_RELEASE) {
        game->mouse_pressed = true;
    } else {
        game->mouse_pressed = false;
    }

}

void FramebufferSizeCallback(GLFWwindow* window, int w, int h) {
    Game* game = (Game*)glfwGetWindowUserPointer(window);
    game->handleResize();
}

int main() {
    // setup glfw
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) { throw std::runtime_error("glfwInit failed"); }

    // create window and GL context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(800, 600, "cloth", NULL, NULL);
    if (!window) { throw std::runtime_error("glfwCreateWindow failed"); }
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1); // enable vsync (0 to disable)
    Game game(window);
    glfwSetWindowUserPointer(window, &game);
    // Callback functions
    glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MousePosCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);

    std::cout << "Window creation successful." << std::endl;

    // main loop
    game.init();
    while (!glfwWindowShouldClose(window)) {
        game.update();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

