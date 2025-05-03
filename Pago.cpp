#include "Pago.h"

string Pago::serialize() const {
    ostringstream oss;
    oss << reservaId << ',' << monto << ',' << estado << ',' << fecha;
    return oss.str();
}

Pago Pago::fromString(const string& s) {
    istringstream iss(s);
    Pago p;
    string token;
    getline(iss, token, ','); p.reservaId = stoi(token);
    getline(iss, token, ','); p.monto = stod(token);
    getline(iss, p.estado, ',');
    getline(iss, p.fecha, ',');
    return p;
}