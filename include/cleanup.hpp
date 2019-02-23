#ifndef CLEANUP_H
#define CLEANUP_H

#include <iostream>
#include <SDL2/SDL.h>

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

 #endif // CLEANUP_H
