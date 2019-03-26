#ifndef STATE_HPP
#define STATE_HPP

#include <SDL2/SDL.h>
#include <string>

const std::string WINDOW_NAME  = "win::34942";

struct State {
    bool quit = false;
    bool update = true;
    bool mouseIsDown = false;
    bool showMetaCounter = true;

    int tex_width = 100;
    int tex_height = 100;
};

extern struct State state;

#endif
