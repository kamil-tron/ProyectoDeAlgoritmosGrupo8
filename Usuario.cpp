#include "Usuario.h"
#include <sstream>

void trim(string& str) {
    while (!str.empty()) {
        char c = str.back();
        if (c == ' ' || c == '\n' || c == '\r' || c == '\t')
            str.pop_back();
        else
            break;
    }
}

string Usuario::serialize() const {
    ostringstream ss;
    ss << dni << ','
        << nombre << ','
        << apellido << ','
        << correo << ','
        << contrasena;
    return ss.str();
}

Usuario Usuario::fromString(const string& s) {
    istringstream ss(s);
    string dni, nom, ape, mail, pass;

    getline(ss, dni, ',');
    getline(ss, nom, ',');
    getline(ss, ape, ',');
    getline(ss, mail, ',');
    getline(ss, pass);
    trim(pass);

    return Usuario(dni, nom, ape, mail, pass);
}
