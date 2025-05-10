// Reserva.h
#pragma once

#include <string>
#include <sstream>
#include "Lista.h"

using namespace std;

class Reserva {
private:
    string        codigo;
    string        userEmail;
    int           vueloId{};
    string        fecha;
    Lista<string> asientos;
    bool          cancelada{};

public:
    Reserva() = default;

    Reserva(const string& codigo,
        const string& userEmail,
        int vueloId,
        const string& fecha,
        const Lista<string>& asientos)
        : codigo(codigo)
        , userEmail(userEmail)
        , vueloId(vueloId)
        , fecha(fecha)
        , asientos(asientos)
        , cancelada(false)
    {}

    // Getters
    const string& getCodigo() const { return codigo; }
    const string& getUserEmail() const { return userEmail; }
    int                getVueloId() const { return vueloId; }
    const string& getFecha() const { return fecha; }
    const Lista<string>& getAsientos() const { return asientos; }
    bool               isCancelada() const { return cancelada; }

    // Marca la reserva como cancelada
    void cancelar() { cancelada = true; }

    // Agrega un asiento a la reserva
    void agregarAsiento(const string& asiento) {
        asientos.agregaFinal(asiento);
    }

    string serialize() const {
        ostringstream oss;
        oss << codigo << ','
            << userEmail << ','
            << vueloId << ','
            << fecha << ','
            << (cancelada ? '1' : '0') << ','
            << asientos.toPrint(";");
        return oss.str();
    }

    static Reserva fromString(const string& s) {
        istringstream iss(s);
        Reserva r;
        char comma, flag;
        string seatsStr;

        // Campos hasta cancelada
        getline(iss, r.codigo, ',');
        getline(iss, r.userEmail, ',');

        iss >> r.vueloId >> comma;
        getline(iss, r.fecha, ',');

        iss >> flag >> comma;
        r.cancelada = (flag == '1');

        // Resto: lista de asientos separada por ';'
        getline(iss, seatsStr);
        r.asientos = Lista<string>();
        istringstream ss(seatsStr);
        string token;
        while (getline(ss, token, ';')) {
            if (!token.empty())
                r.asientos.agregaFinal(token);
        }

        return r;
    }
};
