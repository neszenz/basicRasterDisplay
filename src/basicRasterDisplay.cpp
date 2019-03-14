#include "basicRasterDisplay.hpp"

#include "util.hpp"

#define PIXEL_FORMAT SDL_PIXELFORMAT_RGBA32

const int  TEXTURE_WIDTH    = -1;
const int  TEXTURE_HEIGHT   = 10;
const int  WINDOW_WIDTH     = 1280;
const int  WINDOW_HEIGHT    = 720;
const int  WINDOW_POS_X     = SDL_WINDOWPOS_CENTERED;
const int  WINDOW_POS_Y     = SDL_WINDOWPOS_CENTERED;
const bool WINDOW_RESIZABLE = true;

BRD::BRD() {
    // create window based on config
    m_window = SDL_CreateWindow(m_window_name.c_str(),
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
    m_renderer = SDL_CreateRenderer(m_window, -1,
                                                  SDL_RENDERER_ACCELERATED |
                                                  SDL_RENDERER_PRESENTVSYNC);
    if(m_renderer == nullptr) {
        throwSDLError("SDL_CreateRenderer()");
    }

    // create texture that represents all display pixel
    m_raster = this->createNewRaster();
    if (m_raster == nullptr) {
        throwSDLError("createNewRaster()");
    }

    // create pixel format for rgba to Uint32 mapping
    m_format = SDL_AllocFormat(PIXEL_FORMAT);
    if (m_format == nullptr) {
        throwSDLError("SDL_AllocFormat()");
    }
}

BRD::~BRD() {
    cleanup(m_renderer, m_window, m_raster, m_format);
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

void BRD::setTitle(std::string title) {
    SDL_SetWindowTitle(m_window, title.c_str());
}

// new raster if dimensions differ from viewport (e.g. after resize)
bool BRD::updateRasterDimensions() {
    if (rasterNeedsUpdate() == false) {
        // no need to update
        return false;
    }

    // destroy current
    SDL_DestroyTexture(m_raster);

    // create new
    m_raster = this->createNewRaster();
    if (m_raster == nullptr) {
        throwSDLError("SDL_CreateTexture()");
    }

    return true;
}

void BRD::lock(Uint32** pixels) {
    int pitch;
    void* void_pixels;
    if (SDL_LockTexture(m_raster, NULL, &void_pixels, &pitch) != 0) {
        throwSDLError("SDL_LockTexture()");
    }

    *pixels = (Uint32*)void_pixels;
}
void BRD::unlock() {
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

bool BRD::rasterNeedsUpdate() {
    // acquire information
    int viewport_width;
    int viewport_height;
    this->getViewportDimensions(viewport_width, viewport_height);
    int texture_width;
    int texture_height;
    this->getTextureDimensions(texture_width, texture_height);

    // decide
    if (TEXTURE_WIDTH <= 0 && viewport_width != texture_width) {
        return true;
    }

    if (TEXTURE_HEIGHT <= 0 && viewport_height != texture_height) {
        return true;
    }

    return false;
}
