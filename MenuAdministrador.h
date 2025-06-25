#pragma once
#include "MenuBase.h"
#include "MenuOpcionTexto.h"
#include "ServicioVuelos.h"
#include "ServicioReservas.h"
#include "ServicioCheckIn.h"
#include "RepoUsuarios.h"
#include <iostream>

using namespace std;

class MenuAdministrador : public MenuBase {
private:
    ServicioVuelos svcVuelos;
    ServicioReservas svcReservas;
	ServicioCheckIn svcCheckIn;

    void opcionRegistrarVuelo();
    void opcionVerTodosVuelos();
    void opcionModificarVuelo();
    void opcionEliminarVuelo();
    void opcionHistorialReservas();
    void opcionVerUsuarios();
    void opcionVuelosMasCaros();
    void opcionVuelosProximos();

	void opcionProcesarCheckIn();
	void opcionVerPendientesCheckIn();
	void opcionVerHistorialCheckIn();

    void opcionCerrarSesion();

public:
    MenuAdministrador(Sesion& s) : MenuBase(s) {
        // Gestión de vuelos
        agregarOpcion(new MenuOpcionTexto("Registrar nuevo vuelo", [this] { opcionRegistrarVuelo(); }));
        agregarOpcion(new MenuOpcionTexto("Ver todos los vuelos", [this] { opcionVerTodosVuelos(); }));
        agregarOpcion(new MenuOpcionTexto("Modificar vuelo", [this] { opcionModificarVuelo(); }));
        agregarOpcion(new MenuOpcionTexto("Eliminar vuelo", [this] { opcionEliminarVuelo(); }));

        // Información adicional / reportes
        agregarOpcion(new MenuOpcionTexto("Historial de reservas", [this] { opcionHistorialReservas(); }));
        agregarOpcion(new MenuOpcionTexto("Ver usuarios registrados", [this] { opcionVerUsuarios(); })); // 🟡 Discutible
        agregarOpcion(new MenuOpcionTexto("Vuelos mas caros (precio desc.)", [this] { opcionVuelosMasCaros(); })); // 🟡 Discutible
        agregarOpcion(new MenuOpcionTexto("Vuelos proximos por fecha", [this] { opcionVuelosProximos(); }));

        agregarOpcion(new MenuOpcionTexto("Procesar próximo check-in", [this] { opcionProcesarCheckIn();      }));
        agregarOpcion(new MenuOpcionTexto("Ver cola de check-in", [this] { opcionVerPendientesCheckIn(); }));
        agregarOpcion(new MenuOpcionTexto("Historial de check-in", [this] { opcionVerHistorialCheckIn();  }));

        // Cierre de sesión
        agregarOpcion(new MenuOpcionTexto("Cerrar sesion", [this] { opcionCerrarSesion(); }));
    }

    void mostrar() const override {
        cout << "\n--- MENU DE ADMINISTRADOR AIR PACIFIC ---\n";
        MenuBase::mostrar();
    }
};
