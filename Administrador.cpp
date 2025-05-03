#include "Administrador.h"
#include <sstream>

string Administrador::serialize() const {
    ostringstream oss;
    oss << correo << ',' << contrasena;
    return oss.str();
}

Administrador Administrador::fromString(const string& s) {
    istringstream iss(s);
    Administrador a;
    getline(iss, a.correo, ',');
    getline(iss, a.contrasena, ',');
    return a;
}