#include "MenuAdministrador.h"
#include "Pila.h"
#include "Cola.h"
#include "Ordenamientos.h"
#include <iostream>
#include <iomanip>
#include <functional>

using namespace std;

void MenuAdministrador::opcionRegistrarVuelo() {
    string origen, destino, fecha;
    double precio;
    int capacidad = 0;

    cout << "Origen: "; getline(cin, origen);
    cout << "Destino: "; getline(cin, destino);
    cout << "Fecha (dd/mm/aaaa): "; getline(cin, fecha);
    cout << "Precio: "; cin >> precio;

    while (capacidad < 1 || capacidad > 120) {
        cout << "Capacidad de asientos: "; cin >> capacidad;
        cin.ignore(10000, '\n');
        if (capacidad > 120) cout << "Capacidad excedida\n";
        if (capacidad < 1) cout << "Capacidad insuficiente\n";
    }

    int id = svcVuelos.listarVuelos().longitud() + 1;
    Vuelo v(id, origen, destino, fecha, precio, capacidad);

    cout << (svcVuelos.crearVuelo(v) ? "Vuelo registrado exitosamente con ID: " : "Error al registrar el vuelo. ID: ") << id << "\n";
}

void MenuAdministrador::opcionVerTodosVuelos() {
    auto lista = svcVuelos.listarVuelos();
    cout << "ID | Origen | Destino | Fecha | Precio | Asientos disp.\n";
    function<void(int)> imprimir = [&](int i) {
        if (i >= lista.longitud()) return;
        const auto& v = lista.obtenerPos(i);
        int disp = svcReservas.listarAsientosDisponibles(v.getId()).longitud();
        cout << v.getId() << " | " << v.getOrigen() << " | " << v.getDestino() << " | " << v.getFecha() << " | " << fixed << setprecision(2) << v.getPrecio() << " | " << disp << "\n";
        imprimir(i + 1);
        };
    imprimir(0);
}

void MenuAdministrador::opcionModificarVuelo() {
    int id; cout << "Ingrese ID del vuelo a modificar: "; cin >> id; cin.ignore(10000, '\n');
    Vuelo v;
    if (!svcVuelos.buscarVuelo(id, v)) {
        cout << "Vuelo no encontrado.\n";
        return;
    }
    cout << "Nuevo precio: "; double nuevoPrecio; cin >> nuevoPrecio;
    cout << "Nueva capacidad: "; int nuevaCap; cin >> nuevaCap; cin.ignore(10000, '\n');
    v.setPrecio(nuevoPrecio);
    v.setCapacidad(nuevaCap);
    cout << (svcVuelos.modificarVuelo(v) ? "Vuelo actualizado.\n" : "Error al actualizar el vuelo.\n");
}

void MenuAdministrador::opcionEliminarVuelo() {
    int id; cout << "Ingrese ID del vuelo a eliminar: "; cin >> id; cin.ignore(10000, '\n');
    cout << "Seguro que desea eliminar este vuelo? (s/n): "; char resp; cin >> resp; cin.ignore(10000, '\n');
    if (resp == 's' || resp == 'S')
        cout << (svcVuelos.eliminarVuelo(id) ? "Vuelo eliminado correctamente.\n" : "Error al eliminar vuelo.\n");
}

void MenuAdministrador::opcionHistorialReservas() {
    auto lista = svcReservas.listarReservasUsuario("");
    cout << "Codigo | Usuario | Vuelo ID | Fecha | Asientos\n";
    function<void(int)> imprimir = [&](int i) {
        if (i >= lista.longitud()) return;
        const auto& r = lista.obtenerPos(i);
        cout << r.getCodigo() << " | " << r.getUserEmail() << " | " << r.getVueloId() << " | " << r.getFecha() << " | " << r.getAsientos().toPrint(";") << "\n";
        imprimir(i + 1);
        };
    imprimir(0);
}

void MenuAdministrador::opcionVerUsuarios() {
    RepoUsuarios repo;
    auto lista = repo.cargarTodos();
    cout << "Usuarios registrados:\n";
    function<void(int)> imprimir = [&](int i) {
        if (i >= lista.longitud()) return;
        const auto& u = lista.obtenerPos(i);
        cout << (i + 1) << ". " << u.getNombre() << ' ' << u.getApellido() << " - " << u.getCorreo() << "\n";
        imprimir(i + 1);
        };
    imprimir(0);
}

void MenuAdministrador::opcionVuelosMasCaros() {
    Lista<Vuelo> ordenados = svcVuelos.listarVuelos();
    selectionSortPorPrecioDesc(ordenados);
    cout << "\nVUELOS MAS CAROS (precio descendente)\nID | Origen | Destino | Fecha | Precio | Asientos disp.\n";
    function<void(int)> imprimir = [&](int i) {
        if (i >= ordenados.longitud()) return;
        const auto& v = ordenados.obtenerPos(i);
        int disp = svcReservas.listarAsientosDisponibles(v.getId()).longitud();
        cout << v.getId() << " | " << v.getOrigen() << " | " << v.getDestino() << " | " << v.getFecha() << " | " << fixed << setprecision(2) << v.getPrecio() << " | " << disp << "\n";
        imprimir(i + 1);
        };
    imprimir(0);
}

void MenuAdministrador::opcionVuelosProximos() {
    Lista<Vuelo> vuelos = svcVuelos.listarVuelos();
    shellSortPorFechaAsc(vuelos);
    cout << "\nVUELOS PROXIMOS (orden cronologico)\nID | Origen | Destino | Fecha | Precio | Asientos disp.\n";
    function<void(int)> imprimir = [&](int i) {
        if (i >= vuelos.longitud()) return;
        const auto& v = vuelos.obtenerPos(i);
        int disp = svcReservas.listarAsientosDisponibles(v.getId()).longitud();
        cout << v.getId() << " | " << v.getOrigen() << " | " << v.getDestino() << " | " << v.getFecha() << " | " << fixed << setprecision(2) << v.getPrecio() << " | " << disp << "\n";
        imprimir(i + 1);
        };
    imprimir(0);
}

void MenuAdministrador::opcionProcesarCheckIn() {
    CheckIn c;
    if (!svcCheckIn.procesarSiguiente(c)) {
        cout << "No hay pasajeros en la cola de check-in.\n";
        return;
    }
    cout << "Check-in procesado: "
        << c.getEmail() << "  (reserva " << c.getReservaCod() << ")\n";
}

void MenuAdministrador::opcionVerPendientesCheckIn() {
    Cola<CheckIn> cola = svcCheckIn.pendientes();
    if (cola.esVacia()) {
        cout << "La cola de check-in está vacía.\n";
        return;
    }
    cout << "Pasajeros esperando check-in:\n";
    int pos = 1;
    while (!cola.esVacia()) {
        CheckIn c = cola.frente();
        cola.desencolar();
        cout << pos++ << ") "
            << c.getEmail() << " (reserva " << c.getReservaCod() << ")\n";
    }
}

void MenuAdministrador::opcionVerHistorialCheckIn() {
    Pila<CheckIn> pila = svcCheckIn.ultimos();
    if (pila.esVacia()) {
        cout << "No hay historial de check-in en esta sesión.\n";
        return;
    }
    cout << "Últimos check-in procesados (más reciente primero):\n";
    while (!pila.esVacia()) {
        CheckIn c = pila.cima();
        pila.desapilar();
        cout << "• " << c.getEmail()
            << "  (reserva " << c.getReservaCod() << ")\n";
    }
}

void MenuAdministrador::opcionCerrarSesion() {
    cout << "Cerrando sesion...\n";
}
