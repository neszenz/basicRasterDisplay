/** BASIC RASTER DISPLAY (BRD) - COLOR
 * To encounter byte order ambiguity, this class can be used to convert between
 * rgba and Uint32 representation of display colors.
 */
#ifndef BRD_PIXEL_HPP
#define BRD_PIXEL_HPP

#include <SDL2/SDL.h>

namespace brd {

    class Pixel {
        public:
            Uint8 r;
            Uint8 g;
            Uint8 b;
            Uint8 a;

            Pixel(float r, float g, float b, float a = 1.0f);
            Pixel(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
            Pixel(Uint32 pixel);

            ~Pixel();

            Uint32 toUint32();
            void setRGBA(Uint32 pixel);

        private:
            SDL_PixelFormat* m_format;

            void initFormat();
    };

}

#endif//BRD_PIXEL_HPP
