#include "renderer.h"
#include "layer_utils.h"
#include "sexpr_utils.h"
#include "primitive_factory.h"
#include "transform_utils.h"
#include "assertm.h"
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

Renderer::Renderer(const std::string& title, int width, int height, const SEXPR::SEXPR_LIST *sexpr) : Renderer{title, width, height} {
	initializeLayers(sexpr);
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

void Renderer::initializeLayers(const SEXPR::SEXPR_LIST *sexpr) {
	auto layers = find_sub_sexpr(sexpr, "layers");
	LayerUtils::assertSection(layers);

#ifdef DEBUG
	std::cerr << "[*] " << __func__ << ": " << layers->AsString() << std::endl;
#endif

	for (int64_t i = 1, children = layers->GetNumberOfChildren(); i < children; i++) {
		auto child = layers->GetChild(i);
#ifdef DEBUG
		std::cerr << "[*] " << __func__ << ": child " << child->AsString() << std::endl;
#endif
		std::string s1{__func__};
		std::string s2{" child of layers must be a list"};
		auto s3 = s1 + s2;
		assertm(s3.c_str(), child->IsList());

		Layer* l = new Layer(child->GetList());
		m_layers[l->getCanonicalName()] = std::unique_ptr<Layer>{l};
	}
}

void Renderer::listLayersMembers(void) const {
	for (auto& a : m_layers) {
		std::cout << "[*] " << __func__ << ": Layer with a canoncial name of: " << a.first << ':' << std::endl;
		a.second->listPrimitives();
	}
}

bool Renderer::addPrimitive(const SEXPR::SEXPR_LIST *primitive) {
	m_dirty = true;
	auto child = primitive->GetChild(0);

	std::string s1{__func__};
	std::string s2{" 0th child of primitive must be a symbol"};
	auto s3 = s1 + s2;
	assertm(s3, child->IsSymbol());

	auto resultPrimitive = PrimitiveFactory::instance().create(child->GetSymbol(), primitive);
	if (nullptr == resultPrimitive) {
#ifdef DEBUG
		std::cerr << "[*] " << __func__ << ": failed to create primitive: " << child->GetSymbol() << " not registered" << std::endl;
#endif
		return false;
	}
	auto layerName = resultPrimitive->getLayerName();
	if (m_layers.find(layerName) != m_layers.end()) {
		m_layers[layerName]->addPrimitive(std::move(resultPrimitive));
	} else {
#ifdef DEBUG
		std::cerr << "[*] " << __func__ << ": failed to create primitive: layer " << layerName << " not in m_layers" << std::endl;
#endif
		return false;
	}

	return true;
}

void Renderer::drawAll() {
	BoundingBox box = getBoundingBox();
	int w, h;
	SDL_GetWindowSize(m_window, &w, &h);
	Transform t = makeTransform(box, w, h);

	for (const auto& [layer, ptr] : m_layers) {
#if DEBUG
		std::cerr << "[*] " << __func__ << ": drawing ALL PRIMITIVES inside " << layer << std::endl;
#endif
		ptr->drawAll(m_renderer, t);	
	}
}

void Renderer::updateBoundingBox() {
	double inf = std::numeric_limits<double>::infinity();
        double ninf = -std::numeric_limits<double>::infinity();
	m_globalBox = {inf, inf, ninf, ninf};
	for (const auto& [_, layer] : m_layers) {
		BoundingBox b = layer->getBoundingBox();
		m_globalBox.expandToInclude(b);
	}
	m_dirty = false;
}

BoundingBox Renderer::getBoundingBox() {
	if (m_dirty) {
		updateBoundingBox();
	}
	return m_globalBox;
}
