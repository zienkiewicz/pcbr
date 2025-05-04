#include "layer.h"
#include "layer_utils.h"
#include <exception>
#include <sstream>
#include <iostream>


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
	m_USER_NAME = "dummy";

#ifdef DEBUG
		std::cerr << "[+] Layer created with: ordinalSeen " << ordinalSeen
		<< " canonicalNameSeen "  << canonicalNameSeen << " typeSeen " << typeSeen
		<< " userNameSeen " << userNameSeen << std::endl;
#endif
}
