#include <wx/string.h>

wxString From_UTF8(char const* s) {
    return wxString::FromUTF8(s);
}
