#pragma once
#include "RepoReservas.h"
#include "RepoVuelos.h"
#include "ServicioAsientos.h"
#include "HashTable.h"
#include "FuncionesHash.h"
#include "Reserva.h"
#include "Usuario.h"
#include "Lista.h"
#include <string>

class ServicioReservas {
private:
    RepoReservas repoReservas;
    RepoVuelos repoVuelos;
    ServicioAsientos svcAsientos;
    HashTable<string, Reserva>* idx;

    void cargarIndice();
    string generarCodigo() const;

public:
    ServicioReservas();
    ~ServicioReservas();

    Lista<Reserva> listarReservasUsuario(const string& email) const;
    Lista<Asiento> listarAsientosDisponibles(int vueloId) const;

    bool crearReserva(const Reserva& r);
    bool cancelarReserva(const string& codigo);

    bool validarAsientosDisponibles(int vueloId, const Lista<string>& codigos) const;
    double calcularTotal(int vueloId, const Lista<string>& codigos) const;
    bool crearReservaConAsientos(const Usuario& usuario,
        int vueloId,
        const Lista<string>& codigos,
        Reserva& outReserva);

    Reserva* obtener(const string& codigo);
    const Reserva* obtener(const string& codigo) const;
};
