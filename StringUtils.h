#pragma once
#include <string>

inline void trim(std::string& s)
{
    const char* ws = " \t\n\r\f\v";          // espacios, tabs, CR/LF, etc.
    s.erase(0, s.find_first_not_of(ws));     // izquierda
    s.erase(s.find_last_not_of(ws) + 1);     // derecha
}
