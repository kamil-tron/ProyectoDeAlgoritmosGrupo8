#pragma once
#include "RepoAsientos.h"
#include "RepoVuelos.h"
#include "HashTable.h"
#include "FuncionesHash.h"
#include "Asiento.h"
#include "Lista.h"
#include <string>

class ServicioAsientos {
private:
    RepoAsientos repoAsientos;
    RepoVuelos repoVuelos;
    HashTable<string, Asiento>* idx;
    static const double VIP_SURCHARGE;

    string makeKey(int vueloId, int fila, char letra) const {
        return to_string(vueloId) + "|" + to_string(fila) + letra;
    }

    void cargarIndice();

public:
    ServicioAsientos();
    ~ServicioAsientos();

    bool listarDisponibles(int vueloId, Lista<Asiento>& out) const;
    bool precioAsiento(int vueloId, const string& codigo, double& precio) const;
    double totalSeleccion(int vueloId, const Lista<string>& codigos) const;
    bool ocupar(int vueloId, const Lista<string>& codigos);
    bool liberar(int vueloId, const Lista<string>& codigos);
};
