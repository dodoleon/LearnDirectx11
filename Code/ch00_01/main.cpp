#include <iostream>
#include <cstdio>
#include <sstream>
#include <chrono>
#include <SDL_main.h>
#include <SDL.h>

using namespace std;
int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Setup window
    int window_width = 1600;
    int window_height = 900;
    std::string title = "ch00_01_creating_a_window";
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width,
        window_height,
        window_flags);

    bool close_window = false;

    while (!close_window)
    {
        SDL_Event sdl_event;
        while (SDL_PollEvent(&sdl_event))
        {
            if (sdl_event.type == SDL_QUIT)
                close_window = true;
            if (sdl_event.type == SDL_WINDOWEVENT && sdl_event.window.event == SDL_WINDOWEVENT_CLOSE && sdl_event.window.windowID == SDL_GetWindowID(window))
                close_window = true;
            if (sdl_event.type == SDL_KEYDOWN && sdl_event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                close_window = true;
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}