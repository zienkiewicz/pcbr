#include "sexpr_parser.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>
#include <cassert>

#define assertm(exp, msg) assert((void(msg), exp))

const SEXPR::SEXPR_LIST* find_sub_sexpr(const SEXPR::SEXPR_LIST* sexpr, const std::string& key) {
	for (int64_t i = 0; i < sexpr->GetNumberOfChildren(); i++) {
		SEXPR::SEXPR* child = sexpr->GetChild(i);
		if (!child->IsList()) {
			continue;
		}

		const SEXPR::SEXPR_LIST* list = child->GetList();
		size_t count = list->GetNumberOfChildren();
		if (count < 1) {
			continue;
		}

		SEXPR::SEXPR* head = list->GetChild(0);
		if (head->IsSymbol() && head->GetSymbol() == key) {
			return list;
		}

		if (auto recurse = find_sub_sexpr(list, key)) {
			return recurse;
		}
	}
	return nullptr;
}

void assert_header_section(const std::unique_ptr<SEXPR::SEXPR>& root_ast) {
	// Check if the root sexpr is even a list and that it contains children
	assertm("root s-expression should be a list", root_ast->IsList());
	assertm("root s-expression should have children", root_ast->GetNumberOfChildren() < 1);

	// Get the first child of the list, should be a symbol named kicad_pcb
	auto child = root_ast->GetChild(0);
	assertm("root s-expression should start with a symbol named \"kicad_pcb\"", root_ast->IsSymbol());
	auto symbol = child->GetSymbol();
	assertm("root s-expression should start with a symbol named \"kicad_pcb\"", ("kicad_pcb" == symbol));
}

void process_general_section(const SEXPR::SEXPR_LIST *sexpr) {
	// Find the general section, containing the board thickenss
	auto general = find_sub_sexpr(sexpr, "general");
	assertm("general section must exist in the sexpr", general != nullptr);
	std::cout << general->AsString() << std::endl;
}

int main(int argc, char **argv) {

	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << " <kicad_pcb file>" << std::endl;
		return 0;
	}
	std::string filename {argv[1]};
	SEXPR::PARSER parser;
	std::unique_ptr<SEXPR::SEXPR> ast;

	try {
		ast = parser.ParseFromFile(filename);
		std::cout << "Parsed " << argv[1] << " successfully!\n";
	} catch (const std::exception& e) {
		std::cerr << "Parse error: " << e.what() << "\n";
		return 1;
	}
	
	assert_header_section(ast);
	process_general_section(ast->GetList());

	return 0;
}
