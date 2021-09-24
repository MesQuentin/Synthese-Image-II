#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <cstddef>

using namespace glimac;

struct Vertex2DColor {

    // Attribut [M]
    glm::vec2 position;
    glm::vec3 color;

    //Constructeurs [M]
    Vertex2DColor();
    Vertex2DColor(glm::vec2 position, glm::vec3 color);
};

Vertex2DColor::Vertex2DColor() {
    // Constructeur vide [M]
    this-> position = 0;
    this-> color = 0;
}

Vertex2DColor::Vertex2DColor(glm::vec2 position, glm::vec3 color){
    // Constructeur surchargé [M]
    this->position = position;
    this->color = color;

}


int main(int argc, char** argv) {
    // Initialize SDL and open a window
    SDLWindowManager windowManager(800, 600, "GLImac");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    FilePath applicationPath(argv[0]);
    Program program = loadProgram(applicationPath.dirPath() + "shaders/triangle.vs.glsl",
                                applicationPath.dirPath() + "shaders/triangle.fs.glsl");
    program.use();


    // Code d'introduction [M]
    GLuint vbo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    Vertex2DColor vertices[] = { 
        Vertex2DColor(glm::vec2(-0.5, -0.5), glm::vec3(1, 0, 0)),
        Vertex2DColor(glm::vec2(0.5, -0.5), glm::vec3(0, 1, 0)),
        Vertex2DColor(glm::vec2(0, 0.5), glm::vec3(0, 0, 1))
    }
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex2DColor), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    layout(location = 3) in vec2 aVertexPosition;
    layout(location = 8) in vec3 aVertexColor;

    glGenVertexArrays(GLsizei 1, GLuint &vao);
    glBindVertexArray(GLuint vao);
    const Gluint VERTEX_ATTR_POSITION = 0;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_COLOR);

    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), offsetof(Vertex2DColor, position));
    glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), offsetof(Vertex2DColor, color));

    glBindVertexArray(0);


    // Application loop:
    bool done = false;
    while(!done) {
        // Event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
        }

        // Code Rasteurisation [M]
        glClear(GLbitfield mask);
        glDrawArrays(GLenum GL_TRIANGLES, GLint 0, GLsizei 3);

        // Update the display
        windowManager.swapBuffers();
    }

    glDeleteBuffers();
    glDeleteVertexArrays();

    return EXIT_SUCCESS;
}
