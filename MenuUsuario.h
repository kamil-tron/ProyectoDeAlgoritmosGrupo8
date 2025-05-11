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
    ServicioVuelos svcVuelos;
    ServicioReservas svcReservas;
    ServicioPagos svcPagos;

    void reservarVuelo(int vueloId, const string& fecha);
    void imprimirReservasRecursivo(const Lista<Reserva>& reservas, int index);
    int contarConfirmadasRec(const Lista<Reserva>& lista, int i);
    double imprimirHistorialRec(const Lista<Reserva>& reservas, int i);

    void opcionBuscarYReservar();
    void opcionVerReservas();
    void opcionVerPerfil();
    void opcionReportes();
    void opcionCerrarSesion();

    void reporteHistorialReservas();
    void reporteGastoPorMetodo();
    void reporteReservasCanceladas(int dias);
    void reporteVuelosBaratos(int topK);
    void reporteFrecuenciaDestinos();

public:
    MenuUsuario(Sesion& s) : MenuBase(s) {
        agregarOpcion(new MenuOpcionTexto("Buscar vuelos / Nueva reserva", [this] { opcionBuscarYReservar(); }));
        agregarOpcion(new MenuOpcionTexto("Mis reservas", [this] { opcionVerReservas(); }));
        agregarOpcion(new MenuOpcionTexto("Mi perfil", [this] { opcionVerPerfil(); }));
        agregarOpcion(new MenuOpcionTexto("Reportes", [this] { opcionReportes(); }));
        agregarOpcion(new MenuOpcionTexto("Cerrar sesion", [this] { opcionCerrarSesion(); }));
    }

    void mostrar() const override {
        cout << "MENU DE USUARIO - AIR PACIFIC" << endl;
        MenuBase::mostrar();
    }
};
