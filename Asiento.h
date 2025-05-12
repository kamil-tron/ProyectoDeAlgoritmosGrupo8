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
    bool   vip_{ false };

public:
    Asiento() = default;
    Asiento(int vueloId, int fila, char letra, bool ocupado = false, bool vip = false)
        : vueloId_(vueloId)
        , fila_(fila)
        , letra_(letra)
        , ocupado_(ocupado)
        , vip_(vip)
    {
    }
    bool operator==(const Asiento& other) const {
        return vueloId_ == other.vueloId_
            && fila_ == other.fila_
            && letra_ == other.letra_;
    }
    bool isVip() const { return vip_; }
    void setVip(bool v) { vip_ = v; }

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
            << (ocupado_ ? '1' : '0') << ','
            << (vip_ ? '1' : '0');    // <-- incluimos vip
        return oss.str();
    }
    static Asiento fromString(const string& s) {
        istringstream iss(s);
        Asiento a;
        char comma, flagO, flagVip;
        iss >> a.vueloId_ >> comma
            >> a.fila_ >> comma
            >> a.letra_ >> comma
            >> flagO >> comma
            >> flagVip;                // <-- leemos vip
        a.ocupado_ = (flagO == '1');
        a.vip_ = (flagVip == '1');
        return a;
    }
};
