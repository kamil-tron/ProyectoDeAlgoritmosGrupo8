// ServicioReservas.h
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

    // Devuelve todas las reservas activas de un usuario
    Lista<Reserva> listarReservasUsuario(const string& email) const;

    // Lista los asientos libres de un vuelo
    Lista<Asiento> listarAsientosDisponibles(int vueloId) const;

    // Crea una reserva: valida y marca cada asiento como ocupado
    bool crearReserva(const Reserva& r);

    // Cancela una reserva: libera cada asiento y marca la reserva
    bool cancelarReserva(const string& codigo);
};
