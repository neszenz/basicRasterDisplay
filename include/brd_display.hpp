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
#include <vector>

namespace brd {

    class Display {
        public:
            Display();
            ~Display();

            void getViewportDimensions(int &width, int &height);
            void getRasterDimensions(int &width, int &height);

            float getAspectRatio();

            void setRasterDimensions(int width, int height);

            void setWindowName(std::string name);
            void showMetaCounter(bool setting);

            bool updateRasterDimensions();

            // raw pixel write access
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
            std::string m_window_name = "brd::display";
            int m_tex_width =  -1;
            int m_tex_height = -1;

            // render meta data
            bool m_show_meta = false;
            double m_fps = 0;
            Uint32 m_deltaTime = 0;
            Uint32 m_smoothDeltaTime = 0;
            struct {
                // computeFPS() static variables workaround
                Uint32 last_slice_timestamp = 0;
                unsigned curr_slice_frames = 0;
                // computeDeltaTime() static variables workaround
                Uint32 last_timestamp = 0;
                // computeSmoothDeltaTime() static variables workaround
                std::vector<Uint32> history = std::vector<Uint32>(10);
            } m_meta;

            // render meta data methods
            void computeFPS();
            void computeDeltaTime();
            Uint32 computeSmoothDeltaTime();
            std::string generateTitle();

            bool rasterNeedsUpdate();
            SDL_Texture* createNewRaster();
    };

}

#endif//BRD_DISPLAY_HPP
