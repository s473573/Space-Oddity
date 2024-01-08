#include <iostream>
#include "Gl/glew.h"
#include <GLFW/glfw3.h>

//#include "Shader_Loader.h"
//#include "Render_Utils.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Renderer.h>
using namespace so;
using namespace std;

void checkGLError(const std::string& location) {
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error at " << location << ": ";
        switch (error) {
            case GL_INVALID_ENUM:      std::cerr << "GL_INVALID_ENUM";      break;
            case GL_INVALID_VALUE:     std::cerr << "GL_INVALID_VALUE";     break;
            case GL_INVALID_OPERATION: std::cerr << "GL_INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:    std::cerr << "GL_STACK_OVERFLOW";    break;
            case GL_STACK_UNDERFLOW:   std::cerr << "GL_STACK_UNDERFLOW";   break;
            case GL_OUT_OF_MEMORY:     std::cerr << "GL_OUT_OF_MEMORY";     break;
            default:                   std::cerr << "Unknown error";        break;
        }
        std::cerr << std::endl;
        assert(false); // You may want to handle this differently in a production environment
    }
}

bool checkShaderCompilation(GLuint shader, const std::string& shaderName) {
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);

        std::cerr << "Error: " << shaderName << " compilation failed.\n" << infoLog << std::endl;

        return false;
    }

    return true;
}

bool
checkProgramLinking(GLuint program, const std::string& programName) {
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(program, sizeof(infoLog), nullptr, infoLog);

        std::cerr << "Error: " << programName << " linking failed.\n" << infoLog << std::endl;

        return false;
    }

    return true;
}

Renderer::Renderer() {}
Renderer
&Renderer::instance()
{
    static Renderer renderer;
    return renderer;
}

void
Renderer::init()
{
    // glew loading
    glewExperimental = true;
    glewInit();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_CULL_FACE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // paths in accordance with running from build/ directory
    mainShader = loadShaderProgram("../space oddity/shaders/shader_main.vert", "../space oddity/shaders/shader_main.frag");
    
    if (!checkShaderCompilation(mainShader, "Main Shader")) {
        std::cerr << "Shader compilation error in main shader!" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    glUseProgram(mainShader);
    
    // if (!checkProgramLinking(mainShader, "Main Shader Program")) {
    //     std::cerr << "Shader program linking error!" << std::endl;
    //     exit(EXIT_FAILURE);
    // }

    //shadelessLocation = getUniformLocation("shadeless");

    glUseProgram(0);
    // deal with HUD
    // HUD.setShaderProgram(loadShaderProgram("Shaders/hud.vert", "Shaders/hud.frag"));
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    assert(glGetError() == 0);

    // move this to wherever is next the pipeline, load a window
    // loadModelToContext("./models/sphere.obj", sphereContext);
    // loadModelToContext("./models/spaceship.obj", shipContext);
}

void Renderer::renderScene(Scene *scene)
{
    glUseProgram(mainShader);
    scene->camera.update();
    // for (unsigned i = 0; i < scene->lights.size(); ++i)
    // {
    //     scene->lights[i].update();
    // }
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // glUniform1i(shadelessLocation, 1);
    // scene->skybox.draw();
    // glUniform1i(shadelessLocation, 0);
    // for (unsigned i = 0; i < scene->gameObjects.size(); ++i)
    // {
    //     if (i == 1)
    //         continue; // muzzle
    //     if (scene->gameObjects[i]->getClass() == CLASSES::ANT)
    //         continue; // Draw ants last for better transpernecy
    //     scene->gameObjects[i]->draw();
    // }
    // for (unsigned i = 0; i < scene->gameObjects.size(); ++i)
    // {
    //     if (scene->gameObjects[i]->getClass() != CLASSES::ANT)
    //         continue;
    //     scene->gameObjects[i]->draw();
    // }
    // 
    // glUniform1i(shadelessLocation, 1);
    // scene->gameObjects[1]->draw();
    // glUniform1i(shadelessLocation, 0);

    // HUD.draw();
    assert(glGetError() == 0);
    WINDOW.internal.display();
    printf("Something should be rendered this far.");
}

GLuint
Renderer::getMainShader()
{
    return mainShader;
}
void
Renderer::terminate()
{
}
GLuint
Renderer::getUniformLocation(const char *uniform)
{
    glUseProgram(mainShader);
    return glGetUniformLocation(mainShader, uniform);
}
GLuint
Renderer::loadShaderProgram(const char *vertexShaderPath, const char *fragmentShaderPath)
{
    GLuint program;
    vector<char> vertexShaderCode = readBinFile(vertexShaderPath);
    vector<char> fragmentShaderCode = readBinFile(fragmentShaderPath);
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLint vertexShaderSize = vertexShaderCode.size();
    GLint fragmentShaderSize = fragmentShaderCode.size();
    char *pVertexShaderCode = vertexShaderCode.data();
    char *pFragmentShaderCode = fragmentShaderCode.data();
    glShaderSource(vertexShader, 1, &pVertexShaderCode, &vertexShaderSize);
    glShaderSource(fragmentShader, 1, &pFragmentShaderCode, &fragmentShaderSize);
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);
#ifdef DEBUG
    GLint infoLogLen;
    glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLen);
    vector<char> infoLog(infoLogLen);
    glGetShaderInfoLog(vertexShader, infoLog.size(), nullptr, infoLog.data());
    infoLog.push_back('\0');
    printf(infoLog.data());
    infoLog.resize(0);
    glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLen);
    infoLog.resize(infoLogLen);
    glGetShaderInfoLog(fragmentShader, infoLog.size(), nullptr, infoLog.data());
    infoLog.push_back('\0');
    printf(infoLog.data());
    infoLog.resize(0);
#endif
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
#ifdef DEBUG
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
    infoLog.resize(infoLogLen);
    glGetProgramInfoLog(program, infoLogLen, nullptr, infoLog.data());
    infoLog.push_back('\0');
    printf(infoLog.data());
#endif
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    assert(glGetError() == 0);
    return program;
}