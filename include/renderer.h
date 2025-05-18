#pragma once
#include "sexpr.h"
#include "layer.h"
#include "geometry.h"
#include <SDL2/SDL.h>
#include <string>
#include <map>

class Renderer {
	public:
	Renderer(const std::string& title, int width, int height);
	Renderer(const std::string& title, int width, int height, const SEXPR::SEXPR_LIST *sexpr);
	~Renderer();

	void clear();
	void present();
	bool handleEvents();

	void drawAll();
	void drawLayer(std::string& layer) const;

	void initializeLayers(const SEXPR::SEXPR_LIST *sexpr);
	void listLayersMembers(void) const;

	bool addPrimitive(const SEXPR::SEXPR_LIST *primitive);

	BoundingBox getBoundingBox();

	private:
	SDL_Window* m_window = nullptr;
	SDL_Renderer* m_renderer = nullptr;

	BoundingBox m_globalBox;
	bool m_dirty = true;

	std::map<std::string, std::unique_ptr<Layer>> m_layers;

	void updateBoundingBox();
};
