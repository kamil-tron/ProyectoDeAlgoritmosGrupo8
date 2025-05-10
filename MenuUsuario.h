#pragma once
#include "MenuBase.h"
#include "MenuOpcionTexto.h"
#include "ServicioVuelos.h"
#include "ServicioReservas.h"
#include "ServicioPagos.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include <ctime>

using namespace std;

class MenuUsuario : public MenuBase {
private:
    ServicioVuelos   svcVuelos;
    ServicioReservas svcReservas;
    ServicioPagos    svcPagos;

    /*------------------ helpers ------------------*/
    void reservarVuelo(int vueloId, const string& fecha);

    /*------------------ opciones de menú ------------------*/
    void opcionBuscarYReservar();
    void opcionVerReservas();
    void opcionVerPerfil();
    void opcionCerrarSesion();

public:
    explicit MenuUsuario(Sesion& s) : MenuBase(s) {
        agregarOpcion(new MenuOpcionTexto("Buscar vuelos / Nueva reserva",
            [this] { opcionBuscarYReservar(); }));
        agregarOpcion(new MenuOpcionTexto("Mis reservas",
            [this] { opcionVerReservas(); }));
        agregarOpcion(new MenuOpcionTexto("Mi perfil",
            [this] { opcionVerPerfil(); }));
        agregarOpcion(new MenuOpcionTexto("Cerrar sesion",
            [this] { opcionCerrarSesion(); }));
    }

    void mostrar() const override {
        cout << "-------------------------------\n"
            << "     MENU DE USUARIO - JETSMART\n"
            << "-------------------------------\n";
        MenuBase::mostrar();
    }
};