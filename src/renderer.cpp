#include "renderer.h"
#include <iostream>

Renderer::Renderer(const std::string& title, int width, int height) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "[+] SDL_Init Error: " << SDL_GetError() << std::endl;
		std::exit(1);
	}

	m_window = SDL_CreateWindow(title.c_str(), 100, 100, width, height, SDL_WINDOW_SHOWN);
	if (!m_window) {
		std::cerr << "[+] SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		std::exit(1);
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	if (!m_renderer) {
		std::cerr << "SDLCreateRenderer Error: " << SDL_GetError() << std::endl;
		std::exit(1);
	}
}

Renderer::~Renderer() {
	if (m_renderer) SDL_DestroyRenderer(m_renderer);
	if (m_window) SDL_DestroyWindow(m_window);
	SDL_Quit();
}


void Renderer::clear() {
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);
}

void Renderer::present() {
    SDL_RenderPresent(m_renderer);
}

bool Renderer::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return false;
        }
    }
    return true;
}
