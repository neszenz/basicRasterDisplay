#include <SDL2/SDL.h>

#include <iostream>
#include <vector>

#include "basicRasterDisplay.hpp"
#include "icu.hpp"
#include "state.hpp"
#include "util.hpp"

// global engine state
struct State state;

// modul wide visiable ostream for logSDLError()
static std::ostream &LOG_OS = std::cerr;

void computeFPS() {
    static Uint32 last_second_timestamp = 0;
    static unsigned curr_second_frames = 0;

    Uint32 time_since_last_second = SDL_GetTicks() - last_second_timestamp;
    curr_second_frames++;

    if (time_since_last_second > 250) {
        state.fps = 1000.0 * curr_second_frames / time_since_last_second;
        // reset
        last_second_timestamp = SDL_GetTicks();
        curr_second_frames = 0;
    }

    return;
}
void computeDeltaTime() {
    static Uint32 last_timestamp = 0;
    Uint32 curr_timestamp = SDL_GetTicks();

    state.deltaTime = curr_timestamp - last_timestamp;
    last_timestamp = curr_timestamp;

    return;
}
Uint32 computeSmoothDeltaTime() {
    static std::vector<Uint32> history(10);

    history.insert(history.begin(), state.deltaTime);
    history.pop_back();

    Uint32 sum = 0;
    for (Uint32 d : history) {
        sum += d;
    }

    Uint32 smoothDeltaTime = 0;
    if (history.size() > 0) {
        smoothDeltaTime = sum / history.size();
    }

    return smoothDeltaTime;
}
static std::string generateTitle() {
    std::string title = WINDOW_NAME;
    title += " [";
    title += "fps: " + ToString(round(state.fps));
    title += " | ";
    title += "delta: " + ToString(computeSmoothDeltaTime());
    title += "]";

    return title;
}

void getViewportDimensions(SDL_Renderer* renderer, int &width, int &height) {
    SDL_Rect rect;

    SDL_RenderGetViewport(renderer, &rect);

    width = rect.w;
    height = rect.h;
}
void getTextureDimensions(SDL_Texture* texture, int &width, int &height) {
    Uint32 format;
    int access;
    SDL_QueryTexture(texture, &format, &access, &width, &height);
}

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
    // init SDL lib and specified subsystems
    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        logSDLError(LOG_OS, "SDL_Init()");
        SDL_Quit();
        return 1;
    }

    BRD display;

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
                    Uint8 red = 255 * (float(x) / t_width);
                    Uint8 green = 255 * (float(y) / t_height);
                    Uint32 color = 0xff000000 | (red << 0) | (green << 8);
                    pixels[y * t_width + x] = color;
                }
            }
            // 60fps suffers after at best 1.8 mio. pixels if update every time
            state.update = false;
        }

        // end write mode and thereby upload changes to texture
        display.unlock();

        // render raster display
        display.render();

        // + post-rendering  = + = + = + = + = + = + = + = + = + = + = + = + = +
        computeFPS();
        computeDeltaTime();
        display.setTitle(generateTitle());
    }

    SDL_Quit();
    return 0;
}
