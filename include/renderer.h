#pragma once

#include <SDL2/SDL.h>
#include <string>

class Renderer {
	public:
		Renderer(const std::string& title, int width, int height);
		~Renderer();

		void clear();
		void present();
		bool handleEvents();
		SDL_Renderer* getRendererPtr() { return m_renderer; }

	private:
		SDL_Window* m_window = nullptr;
		SDL_Renderer* m_renderer = nullptr;
};
