#include "sexpr_parser.h"
#include <iostream>

int main() {
	std::string expr = "(print \"Hello, KiCad S-Expressions!\")";
	SEXPR::PARSER parser;
	try {
		auto ast = parser.Parse(expr);
		std::cout << "Parsed successfully!\n";
	} catch (const std::exception& e) {
		std::cerr << "Parse error: " << e.what() << "\n";
		return 1;
	}

	return 0;
}
