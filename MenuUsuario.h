#pragma once
#include "MenuBase.h"
#include "MenuOpcionTexto.h"
#include "ServicioVuelos.h"
#include "ServicioReservas.h"
#include "ServicioPagos.h"
#include "ServicioCheckIn.h"
#include "ServicioRutas.h"
#include <iostream>
#include <string>
using namespace std;

class MenuUsuario : public MenuBase {
private:
    ServicioVuelos svcVuelos;
    ServicioReservas svcReservas;
    ServicioPagos svcPagos;
	ServicioCheckIn svcCheckIn;
    ServicioRutas svcRutas;

    void reservarVuelo(int vueloId);
    void reservarRuta(const RutaPosible& ruta);
    void imprimirReservasRecursivo(const Lista<Reserva>& reservas, int index);
    int contarConfirmadasRec(const Lista<Reserva>& lista, int i);
    void opcionBuscarYReservar();
    void opcionVerReservas();
	void opcionHacerCheckIn();
    void opcionVerPerfil();
    void opcionCerrarSesion();

public:
    MenuUsuario(Sesion& s) : MenuBase(s) {
        agregarOpcion(new MenuOpcionTexto("Buscar vuelos / Nueva reserva", [this] { opcionBuscarYReservar(); }));
        agregarOpcion(new MenuOpcionTexto("Mis reservas", [this] { opcionVerReservas(); }));
		agregarOpcion(new MenuOpcionTexto("Hacer check-in", [this] { opcionHacerCheckIn(); }));
        agregarOpcion(new MenuOpcionTexto("Mi perfil", [this] { opcionVerPerfil(); }));
        agregarOpcion(new MenuOpcionTexto("Cerrar sesion", [this] { opcionCerrarSesion(); }));
    }

    void mostrar() const override {
        cout << "MENU DE USUARIO - AIR PACIFIC" << endl;
        MenuBase::mostrar();
    }
};
