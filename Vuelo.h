#pragma once
#include <string>
#include <sstream>
#include "Asiento.h"
#include "Lista.h"
#include "RepoAsientos.h"
#include "StringUtils.h"
#include <iomanip>
#include <cstdio>

using namespace std;

class Vuelo {
private:
    int id_{};
    string origen, destino, fecha;
    double precio_{};
    int capacidad_{};

public:
    Vuelo() = default;

    Vuelo(int id, string origen, string destino, string fecha,
        double precio, int capacidad)
        : id_(id)
        , origen(move(origen))
        , destino(move(destino))
        , fecha(move(fecha))
        , precio_(precio)
        , capacidad_(capacidad)
    {}

    Vuelo(int id)
        : id_(id)
        , precio_(0)
        , capacidad_(0)
    {}

    bool operator==(const Vuelo& o) const { return id_ == o.id_; }

    bool operator<(const Vuelo& otro) const
    {
        int f1 = getFechaSerial();          // YYYYMMDD como entero
        int f2 = otro.getFechaSerial();

        if (f1 != f2)                       // 1. ordenar por fecha
            return f1 < f2;

        return id_ < otro.id_;              // 2. desempatar por id
    }

    bool operator>(const Vuelo& otro) const
    {
        int f1 = getFechaSerial();
        int f2 = otro.getFechaSerial();

        if (f1 != f2)
            return f1 > f2;

        return id_ > otro.id_;
    }

    int getId() const { return id_; }
    const string& getOrigen() const { return origen; }
    const string& getDestino() const { return destino; }
    const string& getFecha() const { return fecha; }

    int getFechaSerial() const
    {
        // 1. El campo no debería estar vacío
        if (fecha.empty())
            throw std::invalid_argument("fecha vacía en vuelo " +
                std::to_string(id_));

        // 2. Limpio espacios y CR/LF por si llegaron desde el CSV
        std::string f = fecha;
        trim(f);                           // tu helper de StringUtils.h

        // 3. Extraigo día / mes / año ― versión “segura” con sscanf_s
        int d, m, y;
        if (sscanf_s(f.c_str(), "%d/%d/%d", &d, &m, &y) != 3)
            throw std::invalid_argument("fecha malformada: " + f);

        // 4. Validación numérica básica (opcional pero útil)
        if (d < 1 || d > 31 || m < 1 || m > 12 || y < 1900)
            throw std::out_of_range("fecha fuera de rango: " + f);

        // 5. Devuelvo YYYYMMDD como entero (por ejemplo 20250811)
        return y * 10000 + m * 100 + d;
    }

    double getPrecio() const { return precio_; }
    int getCapacidad() const { return capacidad_; }

    void setPrecio(double p) { precio_ = p; }
    void setCapacidad(int c) { capacidad_ = c; }

    string serialize() const {
        ostringstream oss;
        oss << id_ << ','
            << origen << ','
            << destino << ','
            << fecha << ','
            << precio_ << ','
            << capacidad_;
        return oss.str();
    }

    static Vuelo fromString(const string& s) {
        istringstream iss(s);
        Vuelo v;
        char comma;

        iss >> v.id_ >> comma;

        getline(iss, v.origen, ',');  trim(v.origen);
        getline(iss, v.destino, ',');  trim(v.destino);
        getline(iss, v.fecha, ',');  trim(v.fecha);

        iss >> v.precio_ >> comma >> v.capacidad_;
        return v;
    }

    Lista<Asiento> listarAsientos() const {
        RepoAsientos repo;
        return repo.listarPorVuelo(id_);
    }

    int contarAsientosDisponibles() const {
        auto lista = listarAsientos();
        int libres = 0;
        for (int i = 0; i < lista.longitud(); ++i) {
            if (!lista.obtenerPos(i).isOcupado())
                ++libres;
        }
        return libres;
    }

    int getAsientosDisponibles() const { return contarAsientosDisponibles(); }

    void setAsientosDisponibles(int nuevosLibres) {
        RepoAsientos repo;
        auto asientos = repo.listarPorVuelo(id_);
        int libresActuales = contarAsientosDisponibles();
        int diferencia = nuevosLibres - libresActuales;

        if (diferencia < 0) {
            diferencia = -diferencia;
            for (int i = 0; i < asientos.longitud() && diferencia; ++i) {
                if (!asientos.obtenerPos(i).isOcupado()) {
                    Asiento a = asientos.obtenerPos(i);
                    a.setOcupado(true);
                    repo.actualizar(a);
                    --diferencia;
                }
            }
        }
        else if (diferencia > 0) {
            for (int i = 0; i < asientos.longitud() && diferencia; ++i) {
                if (asientos.obtenerPos(i).isOcupado()) {
                    Asiento a = asientos.obtenerPos(i);
                    a.setOcupado(false);
                    repo.actualizar(a);
                    --diferencia;
                }
            }
        }
    }

    string getFechaOrdenable() const {
        return fecha.substr(6, 4) + '/' + fecha.substr(3, 2) + '/' + fecha.substr(0, 2);
    }
};
