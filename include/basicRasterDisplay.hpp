#ifndef BASIC_RASTER_DISPLAY_HPP
#define BASIC_RASTER_DISPLAY_HPP

#include <SDL2/SDL.h>
#include <string>

namespace brd {

    class Display {
        public:
            Display();
            ~Display();

            void getViewportDimensions(int &width, int &height);
            void getTextureDimensions(int &width, int &height);

            void setTitle(std::string title);

            bool updateRasterDimensions();

            void lock(Uint32** pixels);
            void unlock();

            void render();

        private:
            // sdl objects
            SDL_Window*      m_window;
            SDL_Renderer*    m_renderer;
            SDL_Texture*     m_raster;
            SDL_PixelFormat* m_format;

            // window settings
            std::string m_window_name = "win::34942";

            SDL_Texture* createNewRaster();

            bool rasterNeedsUpdate();
    };

}

#endif//BASIC_RASTER_DISPLAY_HPP
