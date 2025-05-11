#pragma once

#include <string>
#include <sstream>
using namespace std;

class Asiento {
private:
    int    vueloId_{};
    int    fila_{};
    char   letra_{};
    bool   ocupado_{ false };

public:
    Asiento() = default;
    Asiento(int vueloId, int fila, char letra, bool ocupado = false)
        : vueloId_(vueloId)
        , fila_(fila)
        , letra_(letra)
        , ocupado_(ocupado)
    {}

    bool operator==(const Asiento& other) const {
        return vueloId_ == other.vueloId_
            && fila_ == other.fila_
            && letra_ == other.letra_;
    }

    int  getVueloId() const { return vueloId_; }
    int  getFila()     const { return fila_; }
    char getLetra()    const { return letra_; }
    bool isOcupado()   const { return ocupado_; }

    void setOcupado(bool o) { ocupado_ = o; }

    string getCodigo() const {
        return to_string(fila_) + letra_;   // p.ej. "12B"
    }

    string serialize() const {
        ostringstream oss;
        oss << vueloId_ << ','
            << fila_ << ','
            << letra_ << ','
            << (ocupado_ ? '1' : '0');
        return oss.str();
    }

    static Asiento fromString(const string& s) {
        istringstream iss(s);
        Asiento a;
        char comma, flag;
        iss >> a.vueloId_ >> comma
            >> a.fila_ >> comma
            >> a.letra_ >> comma
            >> flag;
        a.ocupado_ = (flag == '1');
        return a;
    }
};
