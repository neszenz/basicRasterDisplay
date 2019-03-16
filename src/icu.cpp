#include "icu.hpp"

static void handleKeyEvent(SDL_Event &e) {
    switch(e.key.keysym.sym) {
        case SDLK_ESCAPE:
            state.quit = true;
            break;
        case SDLK_f:
            state.showMetaCounter = state.showMetaCounter? false : true;
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
