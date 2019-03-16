#include "brd_pixel.hpp"

#include "brd_util.hpp"

using namespace brd;

#define PIXEL_FORMAT SDL_PIXELFORMAT_RGBA32

void Pixel::initFormat() {
    m_format = SDL_AllocFormat(PIXEL_FORMAT);
    if (m_format == nullptr) {
        throwSDLError("brd::Pixel::SDL_AllocFormat()");
    }
}

Pixel::Pixel(float r, float g, float b, float a) {
    this->r = 255 * r;
    this->g = 255 * g;
    this->b = 255 * b;
    this->a = 255 * a;

    this->initFormat();
}

Pixel::Pixel(Uint8 r, Uint8 g, Uint8 b, Uint8 a) : r(r), g(g), b(b), a(a) {
    this->initFormat();
}

Pixel::Pixel(Uint32 pixel) {
    this->initFormat();

    SDL_GetRGBA(pixel, m_format, &r, &g, &b, &a);
}

Pixel::~Pixel() {
    cleanup(m_format);
}

Uint32 Pixel::toUint32() {
    return SDL_MapRGBA(m_format, r, g, b, a);
}

void Pixel::setRGBA(Uint32 pixel) {
    SDL_GetRGBA(pixel, m_format, &r, &g, &b, &a);
}
