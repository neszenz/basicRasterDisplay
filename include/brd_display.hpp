/** BASIC RASTER DISPLAY (BRD) - DISPLAY
 *  It is meant to abstract an SDL created window with pixel access for software
 *  rendering and 2D graphics programming.
 *
 *  SDL_INIT_VIDEO needs to be initialized beforehand.
 *
 *  Every brd::Display has a window (in the future maybe alternatievly a
 *  monitor), a renderer and a texture that gets rendert to the whole window/
 *  monitor.
 *  The window can be configured to be resizeable and the texture dimensions can
 *  diverge from the window/ viewport dimensions or be linked to them. In the
 *  latter case one needs to call the updateRasterDimensions() to replace the
 *  current texture with a new one synct in dimensions. It is not updated auto-
 *  matically even if dimensions are linked.
 *
 *  Direct raw pixel access is granted by lock() and unlock() methods which work
 *  analogous to SDL_LockTexture() and SDL_UnlockTexture().
 */
#ifndef BRD_DISPLAY_HPP
#define BRD_DISPLAY_HPP

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

#endif//BRD_DISPLAY_HPP
