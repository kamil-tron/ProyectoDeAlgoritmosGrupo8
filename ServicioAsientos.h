#pragma once
#include "RepoAsientos.h"
#include "RepoVuelos.h"
#include "Asiento.h"
#include "Lista.h"
#include <string>

using namespace std;

class ServicioAsientos {
private:
    RepoAsientos repoAsientos;
    RepoVuelos repoVuelos;
    static const double VIP_SURCHARGE;          // +20 % vip

public:
    ServicioAsientos() {}

    bool listarDisponibles(int vueloId, Lista<Asiento>& out) const;

    bool precioAsiento(int vueloId, const string& codigo, double& precio) const;

    double totalSeleccion(int vueloId, const Lista<string>& codigos) const;

    bool ocupar(int vueloId, const Lista<string>& codigos);      // bloquea
    bool liberar(int vueloId, const Lista<string>& codigos);     // desbloquea
};
