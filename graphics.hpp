#pragma once 
#include <SDL2/SDL.h>



class graphics {

    private :

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* texture;

    public :


    graphics(char const* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight);
    ~graphics();
	void Update(void const* buffer, int pitch);
    bool ProcessInput(uint8_t* keys);
	
};