#ifndef BRD_HPP
#define BRD_HPP

#include <SDL2/SDL.h>
#include <string>

class BRD {
    public:
        BRD(); // also increments 'm_numOfDisplays'
        ~BRD();// also decrements 'm_numOfDisplays'

        void getViewportDimensions(int &width, int &height);
        void getTextureDimensions(int &width, int &height);
        void lockDisplay(Uint32* pixels);
        void unlockDisplay();
        void render();
    private:
        static int m_numOfDisplays;
        // sdl objects
        SDL_Window*   m_window;
        SDL_Renderer* m_renderer;
        SDL_Texture*  m_raster;

        // window settings
        std::string m_window_name = "win::34942";

        SDL_Texture* createNewRaster();
};

#endif//BRD_HPP
