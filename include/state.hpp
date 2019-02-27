#ifndef STATE_HPP
#define STATE_HPP

#include <SDL2/SDL.h>
#include <string>

// window settings
#define PIXEL_FORMAT SDL_PIXELFORMAT_RGBA32
const int    TEXTURE_WIDTH     = -1;
const int    TEXTURE_HEIGHT    = 10;
const int    WINDOW_WIDTH      = 1280;
const int    WINDOW_HEIGHT     = 720;
const int    WINDOW_POS_X      = SDL_WINDOWPOS_CENTERED;
const int    WINDOW_POS_Y      = SDL_WINDOWPOS_CENTERED;
const bool   WINDOW_RESIZABLE  = true;
const std::string WINDOW_NAME  = "win::34942";

struct State {
    double fps = 0;
    Uint32 deltaTime = 0;
    Uint32 smoothDeltaTime = 0;

    bool quit = false;
    bool update = true;
    bool mouseIsDown = false;
};

extern struct State state;

#endif
