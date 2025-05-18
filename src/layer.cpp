#include "layer.h"
#include "layer_utils.h"
#include <exception>
#include <sstream>
#include <iostream>
#include <limits>

Layer::Layer(const SEXPR::SEXPR_LIST *layer) {
#ifdef DEBUG
	std::cerr << "[+] Layer(const SEXPR::SEXPR_LIST *layer)" << std::endl << '\t' << layer->AsString() << std::endl;
#endif

	bool ordinalSeen = false, canonicalNameSeen = false, typeSeen = false, userNameSeen = false;
	for (int64_t i = 0; i < layer->GetNumberOfChildren(); i++) {
		auto child = layer->GetChild(i); 
		if (child->IsInteger() and !ordinalSeen) {
			m_ORDINAL = child->GetInteger();
			ordinalSeen = true;
		} else if (!canonicalNameSeen) {
			if (child->IsString()) {
				m_CANONICAL_NAME = child->GetString();
			} else if (child->IsSymbol())  {
				m_CANONICAL_NAME = child->GetSymbol();
			} else {
				std::stringstream ss;
				ss << "layer: " << child->AsString() << " missing CANONICAL_NAME";
				throw LayerUtils::LayerException(ss.str());
			}
			canonicalNameSeen = true;
		} else if (child->IsSymbol() and !typeSeen) {
			m_TYPE = LayerUtils::mapType(child->GetSymbol());
			typeSeen = true;
		}
	}

#ifdef DEBUG
		std::cerr << "[+] Layer created with: ordinalSeen " << ordinalSeen
		<< " canonicalNameSeen "  << canonicalNameSeen << " typeSeen " << typeSeen
		<< " userNameSeen " << userNameSeen << std::endl;
#endif
}

void Layer::addPrimitive(std::unique_ptr<Primitive> primitive) {
	m_primitives.push_back(std::move(primitive));
}

void Layer::listPrimitives(void) const {
	for (auto& a : m_primitives) {
		std::cout << "[*] " << __func__ << ": " << a->getPrimitiveName() << std::endl;
	}
}

void Layer::drawAll(SDL_Renderer *renderer, Transform& t) const {
	for (const auto& a : m_primitives) {
#ifdef DEBUG
	std::cerr << "[*] " << __func__ << ": drawing primitive of type " << a->getPrimitiveName() << " inside layer " << m_CANONICAL_NAME << std::endl;
#endif
		a->draw(renderer, t);
	}
}

BoundingBox Layer::getBoundingBox() const {
	double inf = std::numeric_limits<double>::infinity();
	double ninf = -std::numeric_limits<double>::infinity();
	BoundingBox result = {inf, inf, ninf, ninf};
	for (const auto& prim : m_primitives) {
		result.expandToInclude(prim->getBoundingBox());
	}
	return result;
}
