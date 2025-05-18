#pragma once
#include "sexpr.h"
#include "primitive.h"
#include "geometry.h"
#include "transform_utils.h"

class Line : public Primitive {
	private:
	double m_width;
	std::pair<double, double> m_start;
	std::pair<double, double> m_end;
	std::optional<double> m_angle;

	public:
	Line(const SEXPR::SEXPR_LIST* line);

	Line(Primitive::SCOPE scope,
		std::string layer,
		double width,
		std::pair<double, double> start,
		std::pair<double, double> end,
		std::optional<double> angle = std::nullopt
	) : Primitive{scope, layer}, m_width{width}, m_start{start}, m_end{end}, m_angle{angle} {} 
	
	BoundingBox getBoundingBox() const;
	void draw(SDL_Renderer *renderer, Transform& t) const override;
};
