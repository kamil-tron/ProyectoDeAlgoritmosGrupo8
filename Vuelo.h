#pragma once

#include <string>
#include <sstream>
using namespace std;

class Vuelo {
private:
    int id_{};
    string origen, destino, fecha;
    double precio{};
    int capacidad{};
    int asientosDisponibles{};

public:
    Vuelo() = default;

    Vuelo(int id, string origen, string destino, string fecha,
        double precio, int capacidad)
        : id_(id)
        , origen(move(origen))
        , destino(move(destino))
        , fecha(move(fecha))
        , precio(precio)
        , capacidad(capacidad)
        , asientosDisponibles(capacidad)
    {}

    Vuelo(int id)
        : id_(id)
        , precio(0)
        , capacidad(0)
        , asientosDisponibles(0)
    {}

    bool operator==(const Vuelo& other) const {
        return id_ == other.id_;
    }

    int getId() const { return id_; }
    const string& getOrigen() const { return origen; }
    const string& getDestino() const { return destino; }
    const string& getFecha() const { return fecha; }
    double getPrecio() const { return precio; }
    int getCapacidad() const { return capacidad; }
    int getAsientosDisponibles() const { return asientosDisponibles; }

    void setPrecio(double p) { precio = p; }
    void setCapacidad(int c) { capacidad = c; }
    void setAsientosDisponibles(int n) { asientosDisponibles = n; }

    string serialize() const {
        ostringstream oss;
        oss << id_ << ',' << origen << ',' << destino << ','
            << fecha << ',' << precio << ',' << capacidad << ','
            << asientosDisponibles;
        return oss.str();
    }

    static Vuelo fromString(const string& s) {
        istringstream iss(s);
        Vuelo v;
        char comma;
        iss >> v.id_ >> comma;
        getline(iss, v.origen, ',');
        getline(iss, v.destino, ',');
        getline(iss, v.fecha, ',');
        iss >> v.precio >> comma >> v.capacidad >> comma >> v.asientosDisponibles;
        return v;
    }
};