#pragma once
#include "sexpr.h"
#include <vector>

const SEXPR::SEXPR_LIST *find_sub_sexpr(const SEXPR::SEXPR_LIST *sexpr, const std::string& key);
void find_all_sub_sexprs(const SEXPR::SEXPR_LIST* sexpr, const std::string& key, std::vector<const SEXPR::SEXPR_LIST*>& results);
std::vector<const SEXPR::SEXPR_LIST*> find_all_sub_sexprs(const SEXPR::SEXPR_LIST* sexpr, const std::string& key);
