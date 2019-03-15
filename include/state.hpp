#ifndef STATE_HPP
#define STATE_HPP

#include <SDL2/SDL.h>
#include <string>

const std::string WINDOW_NAME  = "win::34942";

struct State {
    bool quit = false;
    bool update = true;
    bool mouseIsDown = false;
};

extern struct State state;

#endif
