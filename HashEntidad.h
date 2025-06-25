#pragma once
template<typename K, typename T>
class HashEntidad {
private:
    K clave;
    T valor;
    bool eliminado;

public:
    HashEntidad(K c, T v) : clave(c), valor(v), eliminado(false) {}

    K& getClave() { return clave; }
    const K& getClave() const { return clave; }

    T& getValor() { return valor; }
    const T& getValor() const { return valor; }

    bool estaEliminado() const { return eliminado; }
    void marcarEliminado() { eliminado = true; }
};
