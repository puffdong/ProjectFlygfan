#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"


#include "BirdGame/Space.h"

#include "BirdGame/Player.h"
#include "BirdGame/Camera.h"

#include "BirdGame/Utility/ButtonMap.h"

ButtonMap bm;

Space* space;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Close the window when the user presses the ESC key
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_W:
                bm.W = true;
                break;
            case GLFW_KEY_A:
                bm.A = true;
                break;
            case GLFW_KEY_S:
                bm.S = true;
                break;
            case GLFW_KEY_D:
                bm.D = true;
                break;
            case GLFW_KEY_SPACE:
                bm.Space = true;
                break;
            case GLFW_KEY_UP:
                bm.Up = true;
                break;
            case GLFW_KEY_DOWN:
                bm.Down = true;
                break;
            case GLFW_KEY_LEFT:
                bm.Left = true;
                break;
            case GLFW_KEY_RIGHT:
                bm.Right = true;
                break;
            case GLFW_KEY_LEFT_CONTROL:
                bm.Ctrl = true;
                break;
        }
    }
    else if (action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_W:
                bm.W = false;
                break;
            case GLFW_KEY_A:
                bm.A = false;
                break;
            case GLFW_KEY_S:
                bm.S = false;
                break;
            case GLFW_KEY_D:
                bm.D = false;
                break;
            case GLFW_KEY_SPACE:
                bm.Space = false;
                break;
            case GLFW_KEY_UP:
                bm.Up = false;
                break;
            case GLFW_KEY_DOWN:
                bm.Down = false;
                break;
            case GLFW_KEY_LEFT:
                bm.Left = false;
                break;
            case GLFW_KEY_RIGHT:
                bm.Right = false;
                break;
            case GLFW_KEY_LEFT_CONTROL:
                bm.Ctrl = false;
                break;
        }
    }
}


int main(void) 
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1600, 900, "Project Birdo", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1); // sync with refresh rate

    if (glewInit() != GLEW_OK) {
        std::cout << "glew init Error!" << std::endl;
    }

    glfwSetKeyCallback(window, key_callback);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GLCall(glEnable(GL_DEPTH_TEST));
    //glEnable(GL_CULL_FACE);

    Renderer renderer;

    Texture texture("res/textures/grass.tga");
    texture.Bind();

    space = new Space();
    
    float lastTime = glfwGetTime();


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        renderer.Clear();

        space->tick(deltaTime, bm);
        space->renderWorld(deltaTime);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}