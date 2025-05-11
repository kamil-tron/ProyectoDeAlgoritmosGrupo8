#pragma once
#include <string>
#include <sstream>
using namespace std;

class Pago {
private:
    string reservaCodigo;
    double monto;
    string metodo;
    string estado;
    string fechaHora;

public:
    Pago() = default;
    Pago(const string& reservaCodigo, double monto,
        const string& metodo, const string& estado, const string& fechaHora)
        : reservaCodigo(reservaCodigo),
        monto(monto),
        metodo(metodo),
        estado(estado),
        fechaHora(fechaHora)
    {}

    const string& getReservaCodigo() const { return reservaCodigo; }
    double getMonto() const { return monto; }
    const string& getMetodo() const { return metodo; }
    const string& getEstado() const { return estado; }
    const string& getFechaHora() const { return fechaHora; }

    string serialize() const {
        ostringstream oss;
        oss << reservaCodigo << ',' << monto << ',' << metodo
            << ',' << estado << ',' << fechaHora;
        return oss.str();
    }

    static Pago fromString(const string& s) {
        istringstream iss(s);
        Pago p;
        getline(iss, p.reservaCodigo, ',');
        string token;
        getline(iss, token, ','); p.monto = stod(token);
        getline(iss, p.metodo, ',');
        getline(iss, p.estado, ',');
        getline(iss, p.fechaHora, ',');
        return p;
    }
};
