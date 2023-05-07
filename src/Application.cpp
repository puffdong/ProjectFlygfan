#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"



int main(void) 
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Project Birdo", NULL, NULL);
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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float positions[] = {
        100.0f, 100.0f, 0.0f, 0.0f, //cube vertices
        200.0f, 100.0f, 1.0f, 0.0f,
        200.0f, 200.0f, 1.0f, 1.0f,
        100.0f, 200.0f, 0.0f, 1.0f
    };



    unsigned int indices[] = {
        0,1,2,2,3,0 //info for which vertices correlate to each other to make up a cube
    };


    // ID of generated buffer in OpenGL
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    VertexArray va;

    VertexBuffer vb(positions, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push(GL_FLOAT, 2);
    layout.Push(GL_FLOAT, 2);
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    glm::mat4 proj = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f, 0.0f, 0.0f));
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));
    glm::mat4 mvp = proj * view * model;
    
    Shader s = Shader("res/shaders/Basic.shader");
    s.Bind();
    s.SetUniform4f("u_Color", 0.2f, 1.0f, 0.8f, 1.0f);
    s.SetUniformMat4("u_MVP", mvp);

    Texture texture("res/textures/grass.tga");
    texture.Bind();
    s.SetUniform1i("u_Texture", 0);

    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    s.Unbind();

    Renderer renderer;

    
    

    float r = 0.0f;
    float increment = 0.05f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        renderer.Clear();
        
        s.Bind();
        s.SetUniform4f("u_Color", r, 1.0f, 0.8f, 1.0f);
        
        renderer.Draw(va, ib, s);

        if (r > 1.0f) {
            increment = -0.05f;
        }
        else if (r < 0.0f) {
            increment = 0.05f;
        }

        r += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}