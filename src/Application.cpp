#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath) {
    std::ifstream stream(filepath);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    
    std::string line;
    std::stringstream ss[2]; // two types of shaders
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) 
    {
        if (line.find("#shader") != std::string::npos) // If we find on a line we need to specify a type
        {
            if (line.find("vertex") != std::string::npos) 
            {
                type = ShaderType::VERTEX; // Set mode to vertex
            }
            else if (line.find("fragment") != std::string::npos) 
            {
                type = ShaderType::FRAGMENT;// Set mode to fragment
            }
        }
        else 
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() }; // Build and return the ShaderProgramSource struct
}

static unsigned int CompileShader(const std::string& source, unsigned int type) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); // Pointer to beginning of data
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id); // Compile the shader

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); // Get info on the compilation
    
    if (result == GL_FALSE) {
        int length; 
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length); // Get the length of the message
        char* message = (char*) alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message); // Get the message
        std::cout << "Failed " << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " shader compilation!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    // Compile shader from code
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs); //attaching to program
    glAttachShader(program, fs); 
    glLinkProgram(program); // Linking
    glValidateProgram(program); // Validating

    glDeleteShader(vs); // remove overflow thing, we already compiled the program that is used!
    glDeleteShader(fs); // hmm... detachshader is something to look into

    return program;
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

    float positions[] = {
        -0.5f, -0.5f, //cube vertices
        0.5f, -0.5f,
        0.5f, 0.5f,
        -0.5f, 0.5f
    };

    unsigned int indices[] = {
        0,1,2,2,3,0 //info for which vertices correlate to each other to make up a cube
    };




     // ID of generated buffer in OpenGL
    unsigned int VAO;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

   
    
    VertexBuffer vb(positions, 4 * 2 * sizeof(float));
    IndexBuffer ib(indices, 6);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0); // when specifying this vertex attrib poiner we are binding the vao to the beginning of the last bound buffer

   
    
    
    
    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
    
    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader);

    int location = glGetUniformLocation(shader, "u_Color");
    ASSERT(location != 1);
    glUniform4f(location, 0.2f, 1.0f, 0.8f, 1.0f);

    glBindVertexArray(0);
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    float r = 0.0f;
    float increment = 0.05f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shader);
        glUniform4f(location, r, 1.0f, 0.8f, 1.0f);

        glBindVertexArray(VAO);
        ib.Bind();

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // draws the bound (selected) buffer

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

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}