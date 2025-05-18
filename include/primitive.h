#pragma once
#include "geometry.h"
#include "transform_utils.h"
#include <SDL2/SDL.h>
#include <optional>
#include <string>
#include <vector>
#include <utility>

// A base class for all the graphical primitives
class Primitive {
	public:
	enum class SCOPE { global, footprint };
	std::string m_name;
	std::string m_layer;
	SCOPE m_scope;

	Primitive(SCOPE scope, std::string layer) : m_layer{layer}, m_scope{scope} {}
	Primitive() {}
	virtual ~Primitive() = default;

	std::string getLayerName() const { return m_layer;}
	std::string getPrimitiveName() const { return m_name; }

	virtual void draw(SDL_Renderer *renderer, Transform& t) const = 0;
	virtual BoundingBox getBoundingBox() const = 0;
};
