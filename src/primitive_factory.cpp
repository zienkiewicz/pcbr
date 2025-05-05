#include "primitive_factory.h"

void PrimitiveFactory::registerPrimitive(const std::string& name, Creator creator) {
	creators[name] = std::move(creator);
}

std::unique_ptr<Primitive> PrimitiveFactory::create(const std::string& name, SEXPR::SEXPR_LIST *sexpr) {
	auto it = creators.find(name);
	if (it != creators.end()) {
		return (it->second)(sexpr);
	}
	return nullptr;
}
