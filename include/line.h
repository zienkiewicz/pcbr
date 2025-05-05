#pragma once
#include "sexpr.h"
#include "primitive.h"

class Line : protected Primitive {
public:
	Line(const SEXPR::SEXPR_LIST* line);

	Line(Primitive::SCOPE scope,
		std::string layer,
		double width,
		std::pair<double, double> start,
		std::pair<double, double> end,
		std::optional<double> angle = std::nullopt
	) : Primitive(scope, std::move(layer), width, start, end, angle) {} 

// TODO
//	void draw(SDL_Renderer *renderer) const override;
};
