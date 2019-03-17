#include "brd_display.hpp"

#include "brd_util.hpp"

using namespace brd;

// hard coded config
#define PIXEL_FORMAT SDL_PIXELFORMAT_RGBA32
// if tex dimensions are <= 0, the are linked to win dimensions
const int  TEXTURE_WIDTH    = -1;
const int  TEXTURE_HEIGHT   = -1;
const int  WINDOW_WIDTH     = 1280;
const int  WINDOW_HEIGHT    = 720;
const int  WINDOW_POS_X     = SDL_WINDOWPOS_CENTERED;
const int  WINDOW_POS_Y     = SDL_WINDOWPOS_CENTERED;
const bool WINDOW_RESIZABLE = true;

Display::Display() {
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

Display::~Display() {
    cleanup(m_renderer, m_window, m_raster, m_format);
}

void Display::getViewportDimensions(int &width, int &height) {
    SDL_Rect rect;

    SDL_RenderGetViewport(m_renderer, &rect);

    width = rect.w;
    height = rect.h;
}
void Display::getTextureDimensions(int &width, int &height) {
    Uint32 format;
    int access;
    SDL_QueryTexture(m_raster, &format, &access, &width, &height);
}

void Display::setWindowName(std::string name) {
    m_window_name = name;
}
void Display::showMetaCounter(bool setting) {
    m_show_meta = setting;
}

// new raster if dimensions differ from viewport (e.g. after resize)
bool Display::updateRasterDimensions() {
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

void Display::lock(Uint32** pixels) {
    int pitch;
    void* void_pixels;
    if (SDL_LockTexture(m_raster, NULL, &void_pixels, &pitch) != 0) {
        throwSDLError("SDL_LockTexture()");
    }

    *pixels = (Uint32*)void_pixels;
}
void Display::unlock() {
    SDL_UnlockTexture(m_raster);
}

void Display::render() {
    //SDL_RenderClear(m_renderer); // kinda obsolete b/ we copy the raster whole
    SDL_RenderCopy(m_renderer, m_raster, NULL, NULL);
    SDL_RenderPresent(m_renderer);

    // update meta data and window title
    computeFPS();
    computeDeltaTime();
    if (m_show_meta) {
        SDL_SetWindowTitle(m_window, this->generateTitle().c_str());
    } else {
        SDL_SetWindowTitle(m_window, m_window_name.c_str());
    }
}

void Display::computeFPS() {
    Uint32 time_since_last_slice = SDL_GetTicks() - m_meta.last_slice_timestamp;
    m_meta.curr_slice_frames++;

    Uint32 slice_size = 250; // milliseconds
    if (time_since_last_slice > slice_size) {
        m_fps = 1000.0 * m_meta.curr_slice_frames / time_since_last_slice;
        // reset
        m_meta.last_slice_timestamp = SDL_GetTicks();
        m_meta.curr_slice_frames = 0;
    }
}
void Display::computeDeltaTime() {
    Uint32 curr_timestamp = SDL_GetTicks();

    m_deltaTime = curr_timestamp - m_meta.last_timestamp;
    m_meta.last_timestamp = curr_timestamp;
}
Uint32 Display::computeSmoothDeltaTime() {
    m_meta.history.insert(m_meta.history.begin(), m_deltaTime);
    m_meta.history.pop_back();

    Uint32 sum = 0;
    for (Uint32 d : m_meta.history) {
        sum += d;
    }

    Uint32 smoothDeltaTime = 0;
    if (m_meta.history.size() > 0) {
        smoothDeltaTime = sum / m_meta.history.size();
    }

    return smoothDeltaTime;
}
std::string Display::generateTitle() {
    std::string title = m_window_name;
    title += " [";
    title += "fps: " + ToString(round(m_fps));
    title += " | ";
    title += "delta: " + ToString(this->computeSmoothDeltaTime());
    title += "]";

    return title;
}

bool Display::rasterNeedsUpdate() {
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
SDL_Texture* Display::createNewRaster() {
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
