#include "sexpr_parser.h"
#include "layer.h"
#include "assertm.h"
#include "layer_utils.h"
#include "sexpr_utils.h"
#include "general_utils.h"
#include "general.h"
#include "renderer.h"
#include "line.h"
#include "primitive_factory.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <regex>
#include <map>

void assert_header_section(const std::unique_ptr<SEXPR::SEXPR>& root_ast);

void process_general_section(const SEXPR::SEXPR_LIST *sexpr) {
	// Find the general section, containing the board thickenss
	auto general = find_sub_sexpr(sexpr, "general");
	GeneralUtils::assertSection(general);

#ifdef DEBUG
	std::cout << "[+] Extracting general settings from: " << general->AsString() << std::endl;
#endif

	const SEXPR::SEXPR_LIST* g = static_cast<const SEXPR::SEXPR_LIST*>(general);
	General gen{g};
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


	PrimitiveFactory::instance().registerPrimitive("gr_line", [](const SEXPR::SEXPR_LIST *sexpr) {
		return std::make_unique<Line>(sexpr);
	});

	Renderer pcbr("kicad_pcb render", 800, 600, list);

	std::regex linesPattern{"gr_line"};
	auto grLines = find_all_sub_sexprs(list, linesPattern);

	for (auto& a : grLines) {
		pcbr.addPrimitive(a);
	}

#ifdef DEBUG
	pcbr.listLayersMembers();
#endif


	bool running = true;

	while (running) {
		running = pcbr.handleEvents();
		pcbr.clear();
		pcbr.drawAll();
		pcbr.present();
		SDL_Delay(42);
	}

	return 0;
}

void assert_header_section(const std::unique_ptr<SEXPR::SEXPR>& root_ast) {
	// Check if the root sexpr is even a list and that it contains children
	assertm("root s-expression should be a list", root_ast->IsList());
	// Get the first child of the list, should be a symbol named kicad_pcb
	auto child = root_ast->GetChild(0);
	assertm("root s-expression should start with a symbol named \"kicad_pcb\"", child->IsSymbol() && "kicad_pcb" == child->GetSymbol());
}
