#include "sexpr.h"
#include <vector>
#include <string>

const SEXPR::SEXPR_LIST *find_sub_sexpr(const SEXPR::SEXPR_LIST *sexpr, const std::string& key) {
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

void find_all_sub_sexprs(const SEXPR::SEXPR_LIST* sexpr, const std::string& key, std::vector<const SEXPR::SEXPR_LIST*>& results) {
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
            results.push_back(list);
        }

        find_all_sub_sexprs(list, key, results);
    }
}

std::vector<const SEXPR::SEXPR_LIST*> find_all_sub_sexprs(const SEXPR::SEXPR_LIST* sexpr, const std::string& key) {
    std::vector<const SEXPR::SEXPR_LIST*> results;
    find_all_sub_sexprs(sexpr, key, results);
    return results;
}
