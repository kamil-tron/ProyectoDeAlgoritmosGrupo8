#pragma once

#include "RepoReservas.h"
#include "RepoVuelos.h"
#include "RepoAsientos.h"
#include "Reserva.h"
#include "Lista.h"
#include "Asiento.h"

using namespace std;

class ServicioReservas {
private:
    RepoReservas repoReservas;
    RepoVuelos   repoVuelos;
    RepoAsientos repoAsientos;

public:
    ServicioReservas() = default;

    Lista<Reserva> listarReservasUsuario(const string& email) const;

    Lista<Asiento> listarAsientosDisponibles(int vueloId) const;

    bool crearReserva(const Reserva& r);

    bool cancelarReserva(const string& codigo);
};
