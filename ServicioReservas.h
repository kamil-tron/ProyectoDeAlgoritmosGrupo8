#pragma once
#include "RepoReservas.h"
#include "RepoVuelos.h"
#include "ServicioAsientos.h"
#include "Reserva.h"
#include "Usuario.h"
#include "Lista.h"
#include <string>

using namespace std;

class ServicioReservas {
private:
    RepoReservas repoReservas;
    RepoVuelos repoVuelos;           // aún se consulta para fecha del vuelo
    ServicioAsientos svcAsientos;    // toda la lógica de asientos

public:
    ServicioReservas() {}

    /* consultas */
    Lista<Reserva> listarReservasUsuario(const string& email) const;
    Lista<Asiento> listarAsientosDisponibles(int vueloId) const;

    /* API legado (aún usada internamente) */
    bool crearReserva(const Reserva& r);
    bool cancelarReserva(const string& codigo);

    /* helpers de alto nivel */
    bool validarAsientosDisponibles(int vueloId, const Lista<string>& codigos) const;
    double calcularTotal(int vueloId, const Lista<string>& codigos) const;
    bool crearReservaConAsientos(const Usuario& usuario,
        int vueloId,
        const Lista<string>& codigos,
        Reserva& outReserva);

private:
    string generarCodigo() const;    // util interno
};
