#pragma once

#include "RepoReservas.h"
#include "RepoVuelos.h"
#include "Lista.h"
#include <string>

class ServicioReservas {
private:
    RepoReservas repoReservas;
    RepoVuelos  repoVuelos;
public:
    ServicioReservas();

    Lista<Reserva> listarReservasUsuario(const string& email) const;

    bool crearReserva(const Reserva& r);

    bool cancelarReserva(const string& codigo);
};