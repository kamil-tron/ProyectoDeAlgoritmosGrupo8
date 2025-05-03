#include "MenuAdministrador.h"
#include "Vuelo.h"
#include "Usuario.h"
#include <iostream>
using namespace std;

void MenuAdministrador::opcionRegistrarVuelo() {
    string origen, destino, fecha;
    double precio;
    int capacidad;
    cout << "Origen: "; getline(cin, origen);
    cout << "Destino: "; getline(cin, destino);
    cout << "Fecha (dd/mm/aaaa): "; getline(cin, fecha);
    cout << "Precio: "; cin >> precio;
    cout << "Capacidad de asientos: "; cin >> capacidad;
    cin.ignore(10000, '\n');

    int id = (int)(svcVuelos.listarVuelos().longitud()) + 1;
    Vuelo v(id, origen, destino, fecha, precio, capacidad);
    if (svcVuelos.crearVuelo(v))
        cout << "Vuelo registrado exitosamente con ID: " << id << "\n";
    else
        cout << "Error al registrar el vuelo.\n";
}

void MenuAdministrador::opcionVerTodosVuelos() {
    auto lista = svcVuelos.listarVuelos();
    cout << "ID | Origen | Destino | Fecha | Precio | Asientos disp.\n";
    for (int i = 0; i < lista.longitud(); ++i) {
        auto v = lista.obtenerPos(i);
        cout << v.getId() << " | "
            << v.getOrigen() << " | "
            << v.getDestino() << " | "
            << v.getFecha() << " | "
            << v.getPrecio() << " | "
            << v.getAsientosDisponibles() << "\n";
    }
}

void MenuAdministrador::opcionModificarVuelo() {
    int id;
    cout << "Ingrese ID del vuelo a modificar: "; cin >> id;
    cin.ignore(10000, '\n');

    Vuelo v;
    if (!svcVuelos.buscarVuelo(id, v)) {
        cout << "Vuelo no encontrado.\n";
        return;
    }

    double nuevoPrecio;
    int nuevaCap;
    cout << "Nuevo precio: "; cin >> nuevoPrecio;
    cout << "Nueva capacidad: "; cin >> nuevaCap;
    cin.ignore(10000, '\n'); 

    v.setPrecio(nuevoPrecio);
    v.setCapacidad(nuevaCap);
    v.setAsientosDisponibles(nuevaCap);

    if (svcVuelos.modificarVuelo(v))
        cout << "Vuelo actualizado.\n";
    else
        cout << "Error al actualizar el vuelo.\n";
}

void MenuAdministrador::opcionEliminarVuelo() {
    int id;
    cout << "Ingrese ID del vuelo a eliminar: "; cin >> id;
    cin.ignore(10000, '\n');

    cout << "¿Seguro que desea eliminar este vuelo? (s/n): ";
    char resp; cin >> resp;
    cin.ignore(10000, '\n');

    if (resp == 's' || resp == 'S') {
        if (svcVuelos.eliminarVuelo(id))
            cout << "Vuelo eliminado correctamente.\n";
        else
            cout << "Error al eliminar vuelo.\n";
    }
}

void MenuAdministrador::opcionHistorialReservas() {
    auto lista = svcReservas.listarReservasUsuario("");
    cout << "Código | Usuario | Vuelo ID | Fecha | Asientos\n";
    for (int i = 0; i < lista.longitud(); ++i) {
        auto r = lista.obtenerPos(i);
        cout << r.getCodigo() << " | "
            << r.getUserEmail() << " | "
            << r.getVueloId() << " | "
            << r.getFecha() << " | "
            << r.getCantidadAsientos() << "\n";
    }
}

void MenuAdministrador::opcionVerUsuarios() {
    RepoUsuarios repo;
    auto lista = repo.cargarTodos();
    cout << "Usuarios registrados:\n";
    for (int i = 0; i < lista.longitud(); ++i) {
        auto u = lista.obtenerPos(i);
        cout << (i + 1) << ". " << u.getNombre() << " " << u.getApellido() << " - " << u.getCorreo() << "\n";
    }
}

void MenuAdministrador::opcionCerrarSesion() {
    cout << "Cerrando sesion...\n";
}
