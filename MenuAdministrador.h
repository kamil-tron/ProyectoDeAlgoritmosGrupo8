﻿#pragma once

#include "MenuBase.h"
#include "MenuOpcionTexto.h"
#include "ServicioVuelos.h"
#include "ServicioReservas.h"
#include "RepoUsuarios.h"
#include <iostream>

class MenuAdministrador : public MenuBase {
private:
    ServicioVuelos   svcVuelos;
    ServicioReservas svcReservas;

    void opcionRegistrarVuelo();
    void opcionVerTodosVuelos();
    void opcionModificarVuelo();
    void opcionEliminarVuelo();
    void opcionEliminarPrimerVuelo();
    void opcionEliminarUlitimoVuelo();
    void opcionHistorialReservas();
    void opcionVerUsuarios();
    void opcionVuelosMasCaros();
    void opcionVuelosProximos();
    void opcionCerrarSesion();

public:
    MenuAdministrador(Sesion& s) : MenuBase(s) {
        agregarOpcion(new MenuOpcionTexto("Registrar nuevo vuelo", [this] { opcionRegistrarVuelo(); }));
        agregarOpcion(new MenuOpcionTexto("Ver todos los vuelos", [this] { opcionVerTodosVuelos(); }));
        agregarOpcion(new MenuOpcionTexto("Modificar vuelo", [this] { opcionModificarVuelo(); }));
        agregarOpcion(new MenuOpcionTexto("Eliminar vuelo", [this] { opcionEliminarVuelo(); }));
        agregarOpcion(new MenuOpcionTexto("Eliminar ultimo vuelo agregado", [this] { opcionEliminarUlitimoVuelo(); }));
        agregarOpcion(new MenuOpcionTexto("Eliminar primer vuelo agregado", [this] { opcionEliminarPrimerVuelo(); }));
        agregarOpcion(new MenuOpcionTexto("Historial de reservas", [this] { opcionHistorialReservas(); }));
        agregarOpcion(new MenuOpcionTexto("Ver usuarios registrados", [this] { opcionVerUsuarios(); }));
        agregarOpcion(new MenuOpcionTexto("Vuelos mas caros (precio desc.)", [this] { opcionVuelosMasCaros(); }));
        agregarOpcion(new MenuOpcionTexto("Vuelos proximos por fecha", [this] { opcionVuelosProximos(); }));
        agregarOpcion(new MenuOpcionTexto("Cerrar sesion", [this] { opcionCerrarSesion(); }));
    }

    void mostrar() const override {
        cout << "\n--- MENU DE ADMINISTRADOR AIR PACIFIC---\n";
        MenuBase::mostrar();
    }
};
