#include "line.h"
#include "sexpr_utils.h"

#include <iostream>
#include <sstream>

#define noop

void Line::draw(SDL_Renderer *renderer) const {
	std::cout << "[+] START: " << m_start.value().first << ' ' << m_start.value().second << std::endl;
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
		noop; // todo	
	}

	const SEXPR::SEXPR_LIST* width = find_sub_sexpr(line, "width");
	if (nullptr == width) {
		noop; // todo	
	}
	return;
}
