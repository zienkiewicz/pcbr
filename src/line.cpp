#include "line.h"
#include "sexpr_utils.h"
#include "transform_utils.h"

#include <iostream>
#include <sstream>
#include <SDL2/SDL.h>

#define noop

void Line::draw(SDL_Renderer *renderer, Transform& t) const {
	// Green, full opacity
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

	auto p1 = worldToScreen(m_start.first, m_start.second, t);
	auto p2 = worldToScreen(m_end.first, m_end.second, t);

	SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
}

// FIXME: This is so shit

// Parses out xy from (start x y), (end x y), etc...
std::pair<double, double> extractXY(const SEXPR::SEXPR_LIST* sexpr) {
	std::stringstream errorMessage;
	errorMessage << "malformed xy pair s-expr: " << sexpr->AsString();

	if (3 > sexpr->GetNumberOfChildren()) {
		throw std::runtime_error(errorMessage.str());
	}

	double x, y;
	auto childx = sexpr->GetChild(1), childy = sexpr->GetChild(2);
	
	if (childx->IsInteger()) {
		x = childx->GetInteger();
	} else if (childx->IsDouble()) {
		x = childx->GetDouble();
	} else {
		throw std::runtime_error(errorMessage.str());
	}

	if (childy->IsInteger()) {
		y = childy->GetInteger();
	} else if (childx->IsDouble()) {
		y = childy->GetDouble();
	} else {
		throw std::runtime_error(errorMessage.str());
	}

	return std::pair<double, double>{x,y};
}

Line::Line(const SEXPR::SEXPR_LIST* line) {
	std::stringstream errorMessage;
	errorMessage << "malformed line s-expr: " << line->AsString();
	// Assume at least 5 members required (line symbol included)
	// * start x y
	// * end x y
	// * layer
	// * width
	// Not taking UUID into account

	int64_t children = line->GetNumberOfChildren();
	if (children < 5) {
		throw std::runtime_error(errorMessage.str());	
	}	
	auto child0 = line->GetChild(0);
	if (!child0->IsSymbol()) {
		throw std::runtime_error(errorMessage.str());	
	}

	std::string name = child0->GetSymbol();
	if ("gr_line" == name) {
		m_scope = Primitive::SCOPE::global;
	} else if ("fp_line" == name) {
		m_scope = Primitive::SCOPE::footprint;
	} else {
		throw std::runtime_error(errorMessage.str());	
	}
	m_name = name;

	const SEXPR::SEXPR_LIST* start = find_sub_sexpr(line, "start");
	if (nullptr == start) {
	}
	m_start = extractXY(start);

	const SEXPR::SEXPR_LIST* end = find_sub_sexpr(line, "end");
	if (nullptr == end) {
		throw std::runtime_error(errorMessage.str());	
	}
	m_end = extractXY(end);

	const SEXPR::SEXPR_LIST* angle = find_sub_sexpr(line, "angle");
	if (nullptr != angle) {
	}

	const SEXPR::SEXPR_LIST* layer = find_sub_sexpr(line, "layer");
	if (nullptr == layer) {
#ifdef DEBUG
		std::cerr << "[+] LINE: layer is " << layer->AsString() << std::endl;
#endif
	} else {
		// FIXME: hardcoded index for layer name
#ifdef DEBUG
		m_layer = layer->GetChild(1)->GetSymbol();
		std::cerr << "[*] " << __func__ << ": " << m_name << "is inside layer " << m_layer << std::endl;
#endif
	}

	const SEXPR::SEXPR_LIST* width = find_sub_sexpr(line, "width");
	if (nullptr == width) {
		noop; // todo	
	}
#ifdef DEBUG
	std::cerr << "[+] Line constructor created: " << m_start.first << " " << m_start.second << " " << m_end.first << " " << m_end.second << std::endl;
#endif
	return;
}

BoundingBox Line::getBoundingBox() const {
	return {
		static_cast<float>(std::min(m_start.first, m_end.first)),
		static_cast<float>(std::min(m_start.second, m_end.second)),
		static_cast<float>(std::max(m_start.first, m_end.first)),
		static_cast<float>(std::max(m_start.second, m_end.second))
	};
}
