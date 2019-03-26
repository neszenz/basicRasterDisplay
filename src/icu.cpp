#include "icu.hpp"

#include <iostream>

static void handleKeyEvent(SDL_Event &e) {
    switch(e.key.keysym.sym) {
        case SDLK_ESCAPE:
            state.quit = true;
            break;
        case SDLK_f:
            state.showMetaCounter = state.showMetaCounter? false : true;
            break;
        case SDLK_h:
            if (state.tex_width > 0)
                state.tex_width--;
            std::cout << "width: " << state.tex_width << std::endl;
            break;
        case SDLK_l:
            state.tex_width++;
            std::cout << "width: " << state.tex_width << std::endl;
            break;
        case SDLK_k:
            if (state.tex_height > 0)
                state.tex_height--;
            std::cout << "height: " << state.tex_height << std::endl;
            break;
        case SDLK_j:
            state.tex_height++;
            std::cout << "height: " << state.tex_height << std::endl;
            break;
    }
}

static void handleEvent(SDL_Event &e) {
    switch (e.type) {
        case SDL_QUIT:
            state.quit = true;
            break;
        case SDL_KEYDOWN:
            handleKeyEvent(e);
            break;
        case SDL_MOUSEBUTTONDOWN:
            state.mouseIsDown = true;
            break;
        case SDL_MOUSEBUTTONUP:
            state.mouseIsDown = false;
            break;
    }
}

void handleEvents() {
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        handleEvent(e);
    }
}
