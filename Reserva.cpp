#include "Reserva.h"

string Reserva::serialize() const {
    ostringstream oss;
    oss << codigo << ',' << userEmail << ',' << vueloId << ','
        << fecha << ',' << cantidadAsientos << ',' << (cancelada ? '1' : '0');
    return oss.str();
}

Reserva Reserva::fromString(const string& s) {
    istringstream iss(s);
    Reserva r;
    string token;

    getline(iss, r.codigo, ',');
    getline(iss, r.userEmail, ',');
    getline(iss, token, ',');   r.vueloId = stoi(token);
    getline(iss, r.fecha, ',');
    getline(iss, token, ',');   r.cantidadAsientos = stoi(token);
    getline(iss, token, ',');   r.cancelada = (token == "1");

    return r;
}