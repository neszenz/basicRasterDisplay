#include "basicRasterDisplay.hpp"

#include "util.hpp"

#define PIXEL_FORMAT SDL_PIXELFORMAT_RGBA32

const int  TEXTURE_WIDTH    = -1;
const int  TEXTURE_HEIGHT   = -1;
const int  WINDOW_WIDTH     = 1280;
const int  WINDOW_HEIGHT    = 720;
const int  WINDOW_POS_X     = SDL_WINDOWPOS_CENTERED;
const int  WINDOW_POS_Y     = SDL_WINDOWPOS_CENTERED;
const bool WINDOW_RESIZABLE = true;

// init static member
int BRD::m_numOfDisplays = 0;

BRD::BRD() {
    if (m_numOfDisplays == 0) {
        // init SDL lib and specified subsystems
        if(SDL_Init(SDL_INIT_VIDEO) != 0){
            throwSDLError("SDL_Init()");
        }
    }

    // create window based on config
    SDL_Window* m_window = SDL_CreateWindow(m_window_name.c_str(),
                                            WINDOW_POS_X,
                                            WINDOW_POS_Y,
                                            WINDOW_WIDTH,
                                            WINDOW_HEIGHT,
                                            0);
    if(m_window == nullptr) {
        throwSDLError("SDL_CreateWindow()");
    }

    if (WINDOW_RESIZABLE) {
        SDL_SetWindowResizable(m_window, SDL_TRUE);
    } else {
        SDL_SetWindowResizable(m_window, SDL_FALSE);
    }

    // create 2D-renderer for window
    SDL_Renderer* m_renderer = SDL_CreateRenderer(m_window, -1,
                                                  SDL_RENDERER_ACCELERATED |
                                                  SDL_RENDERER_PRESENTVSYNC);
    if(m_renderer == nullptr) {
        throwSDLError("SDL_CreateRenderer()");
    }

    // create texture that represents all display pixel
    SDL_Texture* m_raster = this->createNewRaster();
    if (m_raster == nullptr) {
        throwSDLError("createNewRaster()");
    }

    m_numOfDisplays++;
}

BRD::~BRD() {
    cleanup(m_renderer, m_window, m_raster);
    if (m_numOfDisplays <= 1) {
        SDL_Quit();
    }

    m_numOfDisplays--;
}

void BRD::getViewportDimensions(int &width, int &height) {
    SDL_Rect rect;

    SDL_RenderGetViewport(m_renderer, &rect);

    width = rect.w;
    height = rect.h;
}

void BRD::getTextureDimensions(int &width, int &height) {
    Uint32 format;
    int access;
    SDL_QueryTexture(m_raster, &format, &access, &width, &height);
}

void BRD::lockDisplay(Uint32* pixels) {
    int pitch;
    void* void_pixels = (void*)pixels;
    if (SDL_LockTexture(m_raster, NULL, &void_pixels, &pitch) != 0) {
        throwSDLError("SDL_LockTexture()");
    }
}

void BRD::unlockDisplay() {
    SDL_UnlockTexture(m_raster);
}

void BRD::render() {
    SDL_RenderClear(m_renderer); // kinda obsolete b/ we copy the raster whole
    SDL_RenderCopy(m_renderer, m_raster, NULL, NULL);
    SDL_RenderPresent(m_renderer);
}

SDL_Texture* BRD::createNewRaster() {
    int width = TEXTURE_WIDTH;
    int height = TEXTURE_HEIGHT;

    // query viewport dimensions
    int v_width;
    int v_height;
    this->getViewportDimensions(v_width, v_height);

    if (width <= 0)
        width = v_width;
    if (height <= 0)
        height = v_height;

    SDL_Texture* raster = SDL_CreateTexture(m_renderer,
                                            PIXEL_FORMAT,
                                            SDL_TEXTUREACCESS_STREAMING,
                                            width,
                                            height);

    return raster;
}
