#include <SDL2/SDL.h>

#include "brd_display.hpp"
#include "brd_pixel.hpp"
#include "icu.hpp"
#include "state.hpp"
#include "util.hpp"

#include <iostream>
#include <vector>

// global engine state
struct State state;

// modul wide visiable ostream for logSDLError()
static std::ostream &LOG_OS = std::cerr;

/** draw a texture to renderer at pos x,y, w/ desired w and h
 * @param tex - the source texture we want to draw
 * @param ren - the renderer we want to draw to
 * @param x - the x coordinate to draw to
 * @param y - the y coordinate to draw to
 * @param w - the width to stretch to (-1 for nativ width)
 * @param h - the height to stretch to (-1 for nativ height)
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h) {
    // setup the destination rectangle to be at the position we want
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;

    // query the texture to get its width and height to use
    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);

    if(w > -1) dst.w = w;
    if(h > -1) dst.h = h;

    SDL_RenderCopy(ren, tex, NULL, &dst);
}
/** draw texture to renderer at x,y preserving w and h, plus clipping if desired
 * if clip is passed, the clip's w and h will be used instead of	the texture's
 * @param tex - the source texture we want to draw
 * @param ren - the renderer we want to draw to
 * @param x - the x coordinate to draw to
 * @param y - the y coordinate to draw to
 * @param clip - the sub-section of the texture to draw (clipping rect)
 *               default of nullptr draws the entire texture
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    if (clip != nullptr){
        dst.w = clip->w;
        dst.h = clip->h;
    }
    else {
        SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    }
    SDL_RenderCopy(ren, tex, clip, &dst);
}

/** draw a texture to renderer at pos 0,0 and repeat to fill window
* @param tex - the source texture we want to draw
* @param ren - the renderer we want to draw to
* @param w - window width
* @param h - window hight
*/
void renderTextureTiled(SDL_Texture *tex, SDL_Renderer *ren, const int w, const int h) {
    if(w==0 || h==0 || tex==nullptr || ren==nullptr) { // jobs' done already!
        logSDLError(LOG_OS, "renderTextureTiled() bad imput");
        return;
    }

    // query texture attributes
    SDL_Rect dst;
    dst.x = 0;
    dst.y = 0;
    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);

    // calculate how often the texture needs to be repeated
    int repeatX = ceil(double(w) / dst.w);
    int repeatY = ceil(double(h) / dst.h);

    //draw texture to renderer
    for(int iy = 0; iy < repeatY; ++iy) {
        for(int ix = 0; ix < repeatX; ++ix) {
            SDL_RenderCopy(ren, tex, NULL, &dst);
            dst.x += dst.w;
        }
        dst.x = 0;
        dst.y += dst.h;
    }
}

/** draw a texture to renderer at the center of the window specified by w,h
 * @param tex - the source texture we want to draw
 * @param ren - the renderer we want to draw to
 * @param w - window width
 * @param h - window hight
 */
void renderTextureCentered(SDL_Texture *tex, SDL_Renderer *ren, const int w, const int h) {
    if(w==0 || h==0 || tex==nullptr || ren==nullptr) { // jobs' done already!
        logSDLError(LOG_OS, "renderTextureTiled() bad imput");
        return;
    }

    // query texture attributes
    SDL_Rect dst;
    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);

    // calculate x,y so that the texture w/ nativ w,h is centered
    dst.x = (w/2) - (dst.w/2);
    dst.y = (h/2) - (dst.h/2);

    // draw texture to renderer
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

int main(int argc, const char* argv[]) {
    // must be done to use brd::Display
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        logSDLError(LOG_OS, "SDL_Init()");
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

        // lock the raster texture to get pixel access
        Uint32* pixels;
        display.lock(&pixels);

        int t_width;
        int t_height;
        display.getTextureDimensions(t_width, t_height);

        if (state.update) {
            // now we can write colors to the local representation of our raster
            for (int y = 0; y < t_height; ++y) {
                for (int x = 0; x < t_width; ++x) {
                    float red = float(x) / t_width;
                    float green = float(y) / t_height;
                    brd::Pixel pixel(red, green, 0.0f);
                    pixels[y * t_width + x] = pixel.toUint32();
                }
            }
            // 60fps suffers after at best 1.8 mio. pixels if update every time
            state.update = false;
        }

        // end write mode and thereby upload changes to texture
        display.unlock();

        // render raster display
        display.render();
    }

    SDL_Quit();
    return 0;
}
