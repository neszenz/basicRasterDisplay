/** BASIC RASTER DISPLAY (BRD) - COLOR
 * To encounter byte order ambiguity, this class can be used to convert between
 * rgba and Uint32 representation of display colors.
 * TODO heavily under construction
 */
#ifndef BRD_COLOR_HPP
#define BRD_COLOR_HPP

#include <SDL2/SDL.h>

namespace brd {

    class Color {
        public:
            int r;
            int g;
            int b;
            int a;
    };

}

#endif//BRD_COLOR_HPP
