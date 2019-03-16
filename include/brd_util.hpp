#ifndef _UTIL_HPP
#define _UTIL_HPP

#include <iostream>
#include <SDL2/SDL.h>
#include <sstream>

template<typename T>
std::string ToString(const T &val) {
    std::stringstream ss;
    ss << val;
    return ss.str();
}

// + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = + = +
// sdl utility

void logSDLError(std::ostream &os, const std::string msg);
void logSDLError(const std::string msg);
void throwSDLError(const std::string msg);

/*
 * Recurse through the list of arguments to clean up, cleaning up
 * the first one in the list each iteration.
 */
 template<typename T, typename... Args>
 void cleanup(T *t, Args&... args) {
   cleanup(t); // cleanup the first item in the list
   cleanup(args...); // clean up the remaining arguments
 }
 /*
 * These specializations serve to free the passed argument and also provide the
 * base cases for the recursive call above, eg. when args is only a single
 * item one of the specializations below will be called by
 * cleanup(args...), ending the recursion
 */
template<>
inline void cleanup<SDL_Window>(SDL_Window *win){
	if (!win){
		return;
	}
	SDL_DestroyWindow(win);
}
template<>
inline void cleanup<SDL_Renderer>(SDL_Renderer *ren){
	if (!ren){
		return;
	}
	SDL_DestroyRenderer(ren);
}
template<>
inline void cleanup<SDL_Texture>(SDL_Texture *tex){
	if (!tex){
		return;
	}
	SDL_DestroyTexture(tex);
}
template<>
inline void cleanup<SDL_Surface>(SDL_Surface *surf){
	if (!surf){
		return;
	}
	SDL_FreeSurface(surf);
}
template<>
inline void cleanup<SDL_PixelFormat>(SDL_PixelFormat *format){
	if (!format){
		return;
	}
	SDL_FreeFormat(format);
}

#endif // _UTIL_HPP
