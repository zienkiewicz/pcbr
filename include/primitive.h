#pragma once
#include <SDL2/SDL.h>
#include <optional>
#include <string>
#include <vector>
#include <utility>

// A base class for all the graphical primitives
class Primitive {
	public:
		enum class SCOPE { global, footprint };
	protected:
		std::string m_layer;
		SCOPE m_scope;

		std::optional<double> m_width;
		std::optional<std::pair<double,double>> m_start;
		std::optional<std::pair<double,double>> m_end;
		std::vector<std::pair<double,double>> m_pts;
		std::optional<double> m_angle;
	public:
		virtual ~Primitive() = default;
		virtual void draw(SDL_Renderer *renderer) const = 0;
};
