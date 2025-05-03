#pragma once

#include <string>
#include <sstream>
using namespace std;

class Reserva {
private:
    string codigo;
    string userEmail;
    int vueloId{};
    string fecha;
    int cantidadAsientos{};
    bool cancelada{};
public:
    Reserva() = default;

    Reserva(const string& codigo, const string& userEmail, int vueloId,
        const string& fecha, int cantidadAsientos)
        : codigo(codigo), userEmail(userEmail),
        vueloId(vueloId), fecha(fecha),
        cantidadAsientos(cantidadAsientos), cancelada(false) {}

    const string& getCodigo() const { return codigo; }
    const string& getUserEmail() const { return userEmail; }
    int getVueloId() const { return vueloId; }
    const string& getFecha() const { return fecha; }
    int getCantidadAsientos() const { return cantidadAsientos; }
    bool isCancelada() const { return cancelada; }
    void cancelar() { cancelada = true; }

    string serialize() const;
    static Reserva fromString(const string& s);
};
