#include "layer.h"
#include "layer_utils.h"
#include "assertm.h"
#include <sstream>
#include <iostream>

const char* LayerUtils::LayerException::what() const noexcept {
        return msg.c_str();
}

// Run asserts on the given section ensuring it's structured properly
void LayerUtils::assertSection(const SEXPR::SEXPR_LIST *layers) {
#ifdef DEBUG
	std::cerr << "LayerUtils::assertSection: " << layers->AsString() << std::endl;
#endif
        auto children = layers->GetNumberOfChildren();
        assertm("layers section should consist of at least 1 layer", children > 1);
        for (int64_t i = 0; i < children; i++) {
                auto child = layers->GetChild(i);
                if (i == 0) {
                        assertm("layers section should begin with a symbol \"layers\"", (child->IsSymbol() && child->GetSymbol() == "layers"));
                } else {
                        assertm("each layer inside layers section should be a list", child->IsList());
                }
        }
}

Layer::TYPE LayerUtils::mapType(const std::string& type) {
        if ("jumper" == type) {
                return Layer::jumper;
        } else if ("mixed" == type) {
                return Layer::mixed;
        } else if ("power" == type) {
                return Layer::power;
        } else if ("signal" == type) {
                return Layer::signal;
        } else if ("user" == type) {
                return Layer::user;
        }  else {
                std::stringstream ss;
                ss << "wrong TYPE of " << type << " for layer";
                throw LayerUtils::LayerException(ss.str());
        }
}
