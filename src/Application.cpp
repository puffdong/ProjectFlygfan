#define MAIN //related to VectorUtils4 and its dodgy implementation

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"

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
#include "BirdGame/Camera.h"

glm::mat4 proj;
glm::mat4 worldMatrix;
glm::mat4 modelMatrix;
glm::mat4 mvp;

glm::vec3 old_cameraPos = glm::vec3(0, 2, -2);

Camera* camera;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // Close the window when the user presses the ESC key
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // Perform other actions based on key input
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        // Your code to move forward
    }

    if (key == GLFW_KEY_S && action == (GLFW_PRESS)) {
        std::cout << "hej hej" << std::endl;
    }
}

void initstuff() {
    proj = glm::frustum(-0.5f, 0.5f, -0.5f, 0.5f, 1.0f, 256.0f);
    worldMatrix = glm::lookAt(old_cameraPos, glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));

    glm::vec3 cameraPos(0.f, 2.f, -2.f);
    glm::vec3 cameraDir(0.f, -2.f, 2.f);
    camera = new Camera(cameraPos, cameraDir);
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

    glfwSetKeyCallback(window, key_callback);

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

    initstuff();


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

    mvp = proj * camera->getLookAt() * modelMatrix;
    //std::cout << camera->toString() << std::endl;
    //std::cout << "\nCamera lookAt; \n" << glm::to_string(camera->getLookAt()) << std::endl;
    //std::cout << glm::to_string(mvp) << std::endl;
    
    Shader s = Shader("res/shaders/Player.shader");
    s.Bind();
    s.SetUniform4f("u_Color", 0.2f, 1.0f, 0.8f, 1.0f);
    s.SetUniformMat4("u_MVP", mvp);

    ModelObject objey("res/models/bunny.obj");

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
        glfwPollEvents();
        camera->rotate(0.f, 0.1f);
        mvp = proj * camera->getLookAt() * modelMatrix;

        /* Render here */
        renderer.Clear();
        
        s.Bind();
        s.SetUniform4f("u_Color", r, 1.0f, 0.8f, 1.0f);
        s.SetUniformMat4("u_MVP", mvp);
        objey.render();
        //renderer.Draw(va, ib, s);

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