#pragma once
#include <exception>
#include "layer.h"

namespace LayerUtils {
	void assertSection(const SEXPR::SEXPR_LIST *layers);

	Layer::TYPE mapType(const std::string& type);
	
	class LayerException : public std::exception {
	private:
		std::string msg;
	public:
		LayerException(const std::string& m) : msg{m} {}
		const char* what() const noexcept override;
	};
}
