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

	private:
		SDL_Window* m_window = nullptr;
		SDL_Renderer* m_renderer = nullptr;
};
