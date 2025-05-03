#pragma once
#include "MenuBase.h"
#include "MenuOpcionTexto.h"
#include "ServicioVuelos.h"
#include "ServicioReservas.h"
#include "ServicioPagos.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <limits>

class MenuUsuario : public MenuBase {
private:
    ServicioVuelos   svcVuelos;
    ServicioReservas svcReservas;
    ServicioPagos    svcPagos;
    void opcionBuscarVuelo();
    void opcionReservarVuelo();
    void opcionReservarVuelo(int vueloId, const string& fecha);
    void opcionVerReservas();
    void opcionCancelarReserva();
    void opcionVerPerfil();
    void opcionCerrarSesion();
public:
    explicit MenuUsuario(Sesion& s)
        : MenuBase(s) {
        agregarOpcion(new MenuOpcionTexto("Buscar vuelos", [this] { opcionBuscarVuelo(); }));
        agregarOpcion(new MenuOpcionTexto("Reservar vuelo", [this] { opcionReservarVuelo(); }));
        agregarOpcion(new MenuOpcionTexto("Ver mis reservas", [this] { opcionVerReservas(); }));
        agregarOpcion(new MenuOpcionTexto("Cancelar reserva", [this] { opcionCancelarReserva(); }));
        agregarOpcion(new MenuOpcionTexto("Mi perfil", [this] { opcionVerPerfil(); }));
        agregarOpcion(new MenuOpcionTexto("Cerrar sesion", [this] { opcionCerrarSesion(); }));
    }
    void mostrar() const override {
        cout << "-------------------------------\n"
            << "    MENU DE USUARIO - JETSMART\n"
            << "-------------------------------\n";
        MenuBase::mostrar();
    }
};
