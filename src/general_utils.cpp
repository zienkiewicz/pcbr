#include "general_utils.h"
#include "assertm.h"

void GeneralUtils::assertSection(const SEXPR::SEXPR_LIST *general) {
        assertm("general section must exist in the sexpr", general != nullptr);
        assertm("general section must be a list", general->IsList());
}
