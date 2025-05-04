#pragma once
#include <exception>
#include "layer.h"

namespace LayerUtils {
	Layer::TYPE mapType(const std::string& type);
	
	class LayerException : public std::exception {
	private:
		std::string msg;
	public:
		LayerException(const std::string& m) : msg{m} {}
		const char* what() const noexcept override;
	};
}
