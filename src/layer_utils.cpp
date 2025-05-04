#include "layer.h"
#include "layer_utils.h"
#include <sstream>
#include <iostream>

const char* LayerUtils::LayerException::what() const noexcept {
        return msg.c_str();
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

