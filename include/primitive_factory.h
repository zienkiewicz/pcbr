#pragma once
#include "sexpr.h"
#include "primitive.h"
#include <string>
#include <map>
#include <memory>
#include <functional>

class PrimitiveFactory {
public:
	using Creator = std::function<std::unique_ptr<Primitive>(SEXPR::SEXPR_LIST*)>;

	static PrimitiveFactory& instance() {
		static PrimitiveFactory factory;
		return factory;
	}

	void registerPrimitive(const std::string& name, Creator creator);

	std::unique_ptr<Primitive> create(const std::string& name, SEXPR::SEXPR_LIST *sexpr);
private:
	std::map<std::string, Creator> creators;
};
