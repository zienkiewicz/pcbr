#include "sexpr_parser.h"
#include "layer.h"
#include "layer_utils.h"
#include "assertm.h"
#include "sexpr_utils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>
#include <vector>

void assert_header_section(const std::unique_ptr<SEXPR::SEXPR>& root_ast) {
	// Check if the root sexpr is even a list and that it contains children
	assertm("root s-expression should be a list", root_ast->IsList());
	assertm("root s-expression should have children", root_ast->GetNumberOfChildren() < 1);

	// Get the first child of the list, should be a symbol named kicad_pcb
	auto child = root_ast->GetChild(0);
	assertm("root s-expression should start with a symbol named \"kicad_pcb\"", child->IsSymbol() && "kicad_pcb" == child->GetSymbol());
}

void process_general_section(const SEXPR::SEXPR_LIST *sexpr) {
	// Find the general section, containing the board thickenss
	auto general = find_sub_sexpr(sexpr, "general");
	assertm("general section must exist in the sexpr", general != nullptr);
	std::cout << general->AsString() << std::endl;
}

void layers_process_section(const SEXPR::SEXPR_LIST *sexpr) {
	auto layers = find_sub_sexpr(sexpr, "layers");
	std::vector<Layer> v;
#ifdef DEBUG
	std::cerr << "[+] process_layers_section" << std::endl << layers->AsString() << std::endl;
#endif
	for (int64_t i = 1, children = layers->GetNumberOfChildren(); i < children; i++) {
		auto child = layers->GetChild(i);
#ifdef DEBUG
		std::cerr << "[+] Child" << std::endl << child->AsString() << std::endl;
#endif
		assertm("child of layers must be a list", child->IsList());
		v.push_back(Layer(child->GetList()));
	}
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
	auto list = ast->GetList();
	process_general_section(list);

	LayerUtils::assertSection(list);
	layers_process_section(list);
	return 0;
}
