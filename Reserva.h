#pragma once

#include <string>
#include <sstream>
#include "Lista.h"

using namespace std;

enum class EstadoReserva {
    PENDIENTE,
    CONFIRMADA,
    CANCELADA
};

class Reserva {
private:
    string codigo;
    string userEmail;
    int vueloId{};
    string fecha;
    Lista<string> asientos;
    EstadoReserva estado{ EstadoReserva::PENDIENTE };
    bool checkedIn{ false };

public:
    Reserva() = default;

    Reserva(const string& codigo, const string& userEmail, int vueloId, const string& fecha, const Lista<string>& asientos)
        : codigo(codigo), userEmail(userEmail), vueloId(vueloId), fecha(fecha), asientos(asientos), estado(EstadoReserva::PENDIENTE) {
    }

    const string& getCodigo() const { return codigo; }
    const string& getUserEmail() const { return userEmail; }
    int getVueloId() const { return vueloId; }
    const string& getFecha() const { return fecha; }
    const Lista<string>& getAsientos() const { return asientos; }
    EstadoReserva getEstado() const { return estado; }
    bool isCheckedIn() const { return checkedIn; }

    void marcarCheckIn() { checkedIn = true; }
    void cancelarCheckIn() { checkedIn = false; }

    void confirmar() { estado = EstadoReserva::CONFIRMADA; }
    void cancelar() { estado = EstadoReserva::CANCELADA; }

    void agregarAsiento(const string& asiento) {
        asientos.agregaFinal(asiento);
    }

    string serialize() const {
        ostringstream oss;
        char flag;
        switch (estado) {
        case EstadoReserva::PENDIENTE: flag = 'P'; break;
        case EstadoReserva::CONFIRMADA: flag = 'C'; break;
        case EstadoReserva::CANCELADA: flag = 'X'; break;
        }
        oss << codigo << ','
            << userEmail << ','
            << vueloId << ','
            << fecha << ','
            << flag << ','
            << asientos.toPrint(";") << ','
            << (checkedIn ? '1' : '0');
        return oss.str();
    }

    static Reserva fromString(const string& s) {
        istringstream iss(s);
        Reserva r;
        char comma, flag;
        string seatsStr, checkStr;

        getline(iss, r.codigo, ',');
        getline(iss, r.userEmail, ',');
        iss >> r.vueloId >> comma;
        getline(iss, r.fecha, ',');
        iss >> flag >> comma;

        switch (flag) {
        case 'P': r.estado = EstadoReserva::PENDIENTE; break;
        case 'C': r.estado = EstadoReserva::CONFIRMADA; break;
        case 'X': r.estado = EstadoReserva::CANCELADA; break;
        default: r.estado = EstadoReserva::PENDIENTE; break;
        }

        getline(iss, seatsStr, ',');
        r.asientos = Lista<string>();
        istringstream ss(seatsStr);
        string token;
        while (getline(ss, token, ';')) {
            if (!token.empty()) r.asientos.agregaFinal(token);
        }

        getline(iss, checkStr);
        r.checkedIn = (!checkStr.empty() && checkStr[0] == '1');

        return r;
    }
};
