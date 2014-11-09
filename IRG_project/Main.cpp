/*
* Copyright (c) 2014 Jure Ratkovic
*/

#include <iostream>
#include <GL/glew.h>
#include <Engine/Engine.h>
#include <Engine/Common/ErrorCheck.h>

using namespace engine;
using namespace glm;
using namespace std;

void RenderingLoop()
{
    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); 

    Camera cam(vec3(4.0f, 3.0f, 3.0f), 4.0f / 3.0f, 60.0f);
    DefaultCameraHandler camera(cam, 4.0f, 0.0025f);
    EventHandler::AddEventListener(&camera);
    EventHandler::AddUpdateable(&camera);

    static const GLfloat g_vertex_buffer_data[] = { 
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        1.0f,  1.0f, 0.0f,

        1.0f,  1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
    };

    static const GLfloat uv_data[] = { 
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,

        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
    };

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data) + sizeof(uv_data), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(g_vertex_buffer_data), g_vertex_buffer_data);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), sizeof(uv_data), uv_data);

    glEnableVertexAttribArray(0);
    GLCheckStmt(glVertexAttribPointer(
        0,                  // The attribute we want to configure
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (const GLvoid*)0    // array buffer offset
        ));


    glEnableVertexAttribArray(1);
    GLCheckStmt(glVertexAttribPointer(
        1,                  // The attribute we want to configure
        2,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (const GLvoid*)sizeof(g_vertex_buffer_data)    // array buffer offset
        ));


    Program program("SimpleShader");
    program.Use();
    program.SetUniform("color_map", 0);

    Texture trollface;
    glActiveTexture(GL_TEXTURE0);
    trollface.LoadFromFile("trollface.png");
    trollface.Bind();
    trollface.TexParami(GL_TEXTURE_WRAP_S, GL_REPEAT);
    trollface.TexParami(GL_TEXTURE_WRAP_T, GL_REPEAT);
    trollface.TexParami(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    trollface.TexParami(GL_TEXTURE_MIN_FILTER, GL_LINEAR);


    glClearColor(0.0f, 0.0f, 0.5f, 1.0f);

    do
    {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const mat4 P = camera.GetProjectionMatrix();
        const mat4 V = camera.GetViewMatrix();
        const mat4 M(1.0f);
        program.SetUniform("MVP", P * V * M);
        
        glDrawArrays(GL_TRIANGLES, 0, 6); // 6 indices starting at 0 -> 2 triangles

        // Swap buffers
        SDLHandler::SwapBuffers();
        EventHandler::ProcessPolledEvents();
        EventHandler::Update();

    } while(!EventHandler::Quit());

    // Cleanup VBO
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(program.id);
    glDeleteVertexArrays(1, &VAO);
}

int main(int argc, char *argv[])
{
    SDLHandler::Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

    SDLHandler::CreateWindow(
        "Test",                    // window title
        SDL_WINDOWPOS_UNDEFINED,   // initial x position
        SDL_WINDOWPOS_UNDEFINED,   // initial y position
        640,                       // width, in pixels
        480,                       // height, in pixels
        SDL_WINDOW_OPENGL |        // flags
        SDL_WINDOW_RESIZABLE |
        SDL_WINDOW_SHOWN           
        );

    SDLHandler::InitGL();
    SDLHandler::PrintSoftwareVersions();

    RenderingLoop();

    SDLHandler::CleanUp();
    return 0;
}
