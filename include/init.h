#pragma once

#ifdef _WIN32
    #include <windows.h>
#endif
#define _USE_MATH_DEFINES
#include <cmath>
#include <SDL2/SDL.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>

SDL_Window* init() {
   
    int windowWidth=1000;
    int windowHeight=700;
    /* Init SDL */
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
        return nullptr;
    }

    /* Init OpenGL */
    SDL_GL_LoadLibrary(NULL); // Use 4.4 to get debug infos
  
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
    SDL_GL_SetSwapInterval(1);

    /* Create Window */
    SDL_Window* window = SDL_CreateWindow(
        "Tower defense",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        windowWidth, windowHeight,
        SDL_WINDOW_OPENGL);
    if (window == nullptr) {
        return nullptr;
    }

    /* Create OpenGl context */
    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context == nullptr) {
        return nullptr;
    }
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, windowWidth, windowHeight, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);


    return window;
}