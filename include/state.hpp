#ifndef STATE_HPP
#define STATE_HPP

#include <SDL2/SDL.h>
#include <string>

// window settings
const int    WINDOW_WIDTH      = 1280;
const int    WINDOW_HEIGHT     = 720;
const int    WINDOW_POS_X      = SDL_WINDOWPOS_CENTERED;
const int    WINDOW_POS_Y      = SDL_WINDOWPOS_CENTERED;
const int    WINDOW_BACKGROUND = 0xffffff;
const std::string WINDOW_NAME  = "win::34942";

struct State {
    double fps = 0;
    Uint32 deltaTime = 0;
    Uint32 smoothDeltaTime = 0;

    bool invert = false;
    bool reset = false;
    bool quit = false;
    bool animate = false;
    bool mouseIsDown = false;
    float zPosition = 0.0f;

    Uint32 bkgd_color;
    Uint32 line_color;
    bool depth = false;
};

extern struct State state;

#endif
