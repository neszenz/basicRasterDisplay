#include <iostream>
#include <SDL2/SDL.h>
#include <vector>

#include "brd_display.hpp"
#include "brd_pixel.hpp"
#include "brd_util.hpp"
#include "icu.hpp"
#include "state.hpp"

// global engine state
struct State state;

int main(int argc, const char* argv[]) {
    // must be done to use brd::Display
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        logSDLError(std::cerr, "SDL_Init()");
        SDL_Quit();
        return 1;
    }

    brd::Display display;
    display.setWindowName(WINDOW_NAME);

    while(!state.quit) {
        // + pre-rendering + = + = + = + = + = + = + = + = + = + = + = + = + = +
        handleEvents();

        // + rendering + = + = + = + = + = + = + = + = + = + = + = + = + = + = +
        // new raster, if w/h override is requested and dimensions are different
        state.update = (display.updateRasterDimensions())? true : state.update;

        // if new raster was created, redraw it
        if (state.update) {
            // lock the raster texture to get pixel access
            Uint32* pixels;
            display.lock(&pixels);

            int t_width;
            int t_height;
            display.getTextureDimensions(t_width, t_height);

            // now we can write colors to the local representation of our raster
            for (int y = 0; y < t_height; ++y) {
                for (int x = 0; x < t_width; ++x) {
                    float red = float(x) / t_width;
                    float green = float(y) / t_height;
                    brd::Pixel pixel(red, green, 0.0f);
                    pixels[y * t_width + x] = pixel.toUint32();
                }
            }

            // end write mode and thereby upload changes to texture
            display.unlock();

            // 60fps suffers after at best 1.8 mio. pixels if update every time
            state.update = false;
        }

        // render raster display
        display.render();
    }

    SDL_Quit();
    return 0;
}
