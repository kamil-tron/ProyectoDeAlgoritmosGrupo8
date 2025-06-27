#pragma once
#include "MenuBase.h"
#include "MenuOpcionTexto.h"
#include "ServicioVuelos.h"
#include "ServicioReservas.h"
#include "ServicioCheckIn.h"
#include "ServicioAeropuertos.h"
#include "RepoUsuarios.h"
#include <iostream>

using namespace std;

class MenuAdministrador : public MenuBase {
private:
    ServicioVuelos svcVuelos;
    ServicioReservas svcReservas;
	ServicioCheckIn svcCheckIn;
	ServicioAeropuertos svcAeropuertos;

    void opcionRegistrarVuelo();
    void opcionModificarVuelo();
    void opcionEliminarVuelo();
    void opcionHistorialReservas();
    void opcionVerUsuarios();
    void opcionVuelosMasCaros();
    void opcionVerTodosVuelos();

	void opcionProcesarCheckIn();
	void opcionVerPendientesCheckIn();
	void opcionVerHistorialCheckIn();

    void opcionRegistrarAeropuerto();
    void opcionVerAeropuertos();

    void opcionCerrarSesion();

public:
    MenuAdministrador(Sesion& s) : MenuBase(s) {
        agregarOpcion(new MenuOpcionTexto(
            "Registrar aeropuerto", [this] { opcionRegistrarAeropuerto(); }));
        agregarOpcion(new MenuOpcionTexto(
            "Ver aeropuertos", [this] { opcionVerAeropuertos(); }));


        // Gestión de vuelos
        agregarOpcion(new MenuOpcionTexto("Registrar nuevo vuelo", [this] { opcionRegistrarVuelo(); }));
        agregarOpcion(new MenuOpcionTexto("Ver todos los vuelos", [this] { opcionVerTodosVuelos(); })); // ahora ordenados por fecha
        agregarOpcion(new MenuOpcionTexto("Modificar vuelo", [this] { opcionModificarVuelo(); }));
        agregarOpcion(new MenuOpcionTexto("Eliminar vuelo", [this] { opcionEliminarVuelo(); }));

        // Información adicional / reportes
        agregarOpcion(new MenuOpcionTexto("Historial de reservas", [this] { opcionHistorialReservas(); }));
        agregarOpcion(new MenuOpcionTexto("Ver usuarios registrados", [this] { opcionVerUsuarios(); }));
        agregarOpcion(new MenuOpcionTexto("Vuelos mas caros (precio desc.)", [this] { opcionVuelosMasCaros(); }));

        // Check-in
        agregarOpcion(new MenuOpcionTexto("Procesar próximo check-in", [this] { opcionProcesarCheckIn(); }));
        agregarOpcion(new MenuOpcionTexto("Ver cola de check-in", [this] { opcionVerPendientesCheckIn(); }));
        agregarOpcion(new MenuOpcionTexto("Historial de check-in", [this] { opcionVerHistorialCheckIn(); }));

        // Cierre de sesión
        agregarOpcion(new MenuOpcionTexto("Cerrar sesion", [this] { opcionCerrarSesion(); }));
    }


    void mostrar() const override {
        cout << "\n--- MENU DE ADMINISTRADOR AIR PACIFIC ---\n";
        MenuBase::mostrar();
    }
};
