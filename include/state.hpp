#ifndef STATE_HPP
#define STATE_HPP

#include <SDL2/SDL.h>
#include <string>

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
