#include "sexpr.h"
#include "general.h"

General::General(const SEXPR::SEXPR_LIST* general) {
    for (int64_t i = 0; i < general->GetNumberOfChildren(); ++i) {
        SEXPR::SEXPR* child = general->GetChild(i);
        if (!child->IsList()) continue;

        const SEXPR::SEXPR_LIST* list = child->GetList();
        if (list->GetNumberOfChildren() < 2) continue;

        SEXPR::SEXPR* key = list->GetChild(0);
        SEXPR::SEXPR* value = list->GetChild(1);
        if (!key->IsSymbol()) continue;

        const std::string& name = key->GetSymbol();

        if (name == "links" && value->IsInteger()) m_links = value->GetInteger();
        else if (name == "no_connects" && value->IsInteger()) m_noConnects = value->GetInteger();
        else if (name == "thickness" && value->IsInteger()) m_thickness = value->GetInteger();
        else if (name == "drawings" && value->IsInteger()) m_drawings = value->GetInteger();
        else if (name == "tracks" && value->IsInteger()) m_tracks = value->GetInteger();
        else if (name == "zones" && value->IsInteger()) m_zones = value->GetInteger();
        else if (name == "modules" && value->IsInteger()) m_footprints = value->GetInteger(); // modules = footprints
        else if (name == "nets" && value->IsInteger()) m_nets = value->GetInteger();
    }
}

