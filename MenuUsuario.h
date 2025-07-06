#pragma once
#include "MenuBase.h"
#include "ArbolB.h"
#include "MenuOpcionTexto.h"
#include "ServicioVuelos.h"
#include "ServicioReservas.h"
#include "ServicioPagos.h"
#include "ServicioCheckIn.h"
#include "ServicioRutas.h"
#include "MatrizColor.h"
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
    void imprimirReservasRecursivo(const Lista<Reserva>& reservas, int index,int Y);
    int contarConfirmadasRec(const Lista<Reserva>& lista, int i);
    void opcionBuscarYReservar();
    void opcionBuscarRutaYReservar();
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
   HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        // bitmask de fondo gris claro:
        WORD BG_GRAY = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
        
        system("cls");
        menuUsuario();
        SetConsoleTextAttribute(hConsole, BG_GRAY);
        cursor(6, 12);
        cout << "MENU DE USUARIO - AIR PACIFIC" << endl; cursor(8, 13);
        MenuBase::mostrar();
    }
};
