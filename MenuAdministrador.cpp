#include "MenuAdministrador.h"
#include "Pila.h"
#include <iostream>
#include <iomanip>
#include "Cola.h"
using namespace std;

void MenuAdministrador::opcionRegistrarVuelo() {
    string origen, destino, fecha;
    double precio;
    int capacidad = 0;

    cout << "Origen: ";  getline(cin, origen);
    cout << "Destino: "; getline(cin, destino);
    cout << "Fecha (dd/mm/aaaa): "; getline(cin, fecha);
    cout << "Precio: "; cin >> precio;

    while (capacidad > 120 || capacidad < 1) {
        cout << "Capacidad de asientos: "; cin >> capacidad;
        cin.ignore(10000, '\n');
        if (capacidad > 120) cout << "Capacidad excedida\n";
        if (capacidad < 1)   cout << "Capacidad insuficiente\n";
    }

    int id = svcVuelos.listarVuelos().longitud() + 1;
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
        int disponibles = svcReservas
            .listarAsientosDisponibles(v.getId())
            .longitud();
        cout << v.getId() << " | "
            << v.getOrigen() << " | "
            << v.getDestino() << " | "
            << v.getFecha() << " | "
            << v.getPrecio() << " | "
            << disponibles << "\n";
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
    cout << "Nuevo precio: ";   cin >> nuevoPrecio;
    cout << "Nueva capacidad: "; cin >> nuevaCap;
    cin.ignore(10000, '\n');

    v.setPrecio(nuevoPrecio);
    v.setCapacidad(nuevaCap);

    if (svcVuelos.modificarVuelo(v))
        cout << "Vuelo actualizado.\n";
    else
        cout << "Error al actualizar el vuelo.\n";
}

void MenuAdministrador::opcionEliminarVuelo() {
    int id;
    cout << "Ingrese ID del vuelo a eliminar: "; cin >> id;
    cin.ignore(10000, '\n');

    cout << "Seguro que desea eliminar este vuelo? (s/n): ";
    char resp; cin >> resp;
    cin.ignore(10000, '\n');

    if (resp == 's' || resp == 'S') {
        if (svcVuelos.eliminarVuelo(id))
            cout << "Vuelo eliminado correctamente.\n";
        else
            cout << "Error al eliminar vuelo.\n";
    }
}

void MenuAdministrador::opcionEliminarUlitimoVuelo() {
    auto lista = svcVuelos.listarVuelos();
    if (lista.esVacia()) {
        cout << "No hay vuelos registrados.\n";
        return;
    }

    Pila<Vuelo> pila;
    for (int i = 0; i < lista.longitud(); ++i)
        pila.apilar(lista.obtenerPos(i));

    Vuelo ultimo = pila.cima();

    cout << "Seguro que desea eliminar el ultimo vuelo agregado (ID: "
        << ultimo.getId() << ")? (s/n): ";
    char resp; cin >> resp;
    cin.ignore(10000, '\n');

    if (resp == 's' || resp == 'S') {
        if (svcVuelos.eliminarVuelo(ultimo.getId()))
            cout << "Ultimo vuelo eliminado exitosamente.\n";
        else
            cout << "Error al eliminar el vuelo.\n";
    }
}

void MenuAdministrador::opcionHistorialReservas() {
    auto lista = svcReservas.listarReservasUsuario("");
    cout << "Codigo | Usuario | Vuelo ID | Fecha | Asientos\n";
    for (int i = 0; i < lista.longitud(); ++i) {
        auto r = lista.obtenerPos(i);
        cout << r.getCodigo() << " | "
            << r.getUserEmail() << " | "
            << r.getVueloId() << " | "
            << r.getFecha() << " | "
            << r.getAsientos().toPrint(";") << "\n";
    }
}

void MenuAdministrador::opcionVerUsuarios() {
    RepoUsuarios repo;
    auto lista = repo.cargarTodos();
    cout << "Usuarios registrados:\n";
    for (int i = 0; i < lista.longitud(); ++i) {
        auto u = lista.obtenerPos(i);
        cout << (i + 1) << ". "
            << u.getNombre() << ' ' << u.getApellido()
            << " - " << u.getCorreo() << "\n";
    }
}

void MenuAdministrador::opcionVuelosMasCaros() {
    Lista<Vuelo> ordenados = svcVuelos.listarVuelos();

    for (int i = 0; i < ordenados.longitud(); ++i) {
        for (int j = 0; j < ordenados.longitud() - 1; ++j) {
            Vuelo a = ordenados.obtenerPos(j);
            Vuelo b = ordenados.obtenerPos(j + 1);
            if (a.getPrecio() > b.getPrecio()) {
                ordenados.modificarPos(b, j);
                ordenados.modificarPos(a, j + 1);
            }
        }
    }

    Pila<Vuelo> pila;
    for (int i = 0; i < ordenados.longitud(); ++i)
        pila.apilar(ordenados.obtenerPos(i));

    cout << "\nVUELOS MAS CAROS (precio descendente)\n"
        << "ID | Origen | Destino | Fecha | Precio | Asientos disp.\n";

    while (!pila.esVacia()) {
        Vuelo v = pila.cima();
        pila.desapilar();
        cout << v.getId() << " | "
            << v.getOrigen() << " | "
            << v.getDestino() << " | "
            << v.getFecha() << " | S/ "
            << fixed << setprecision(2) << v.getPrecio() << " | "
            << v.getAsientosDisponibles() << '\n';
    }
}

static int claveFecha(const std::string& f) {
    int d = stoi(f.substr(0, 2));
    int m = stoi(f.substr(3, 2));
    int y = stoi(f.substr(6, 4));
    return y * 10000 + m * 100 + d;
}

void MenuAdministrador::opcionVuelosProximos() {
    Lista<Vuelo> vuelos = svcVuelos.listarVuelos();

    for (int i = 0; i < vuelos.longitud(); ++i) {
        for (int j = 0; j < vuelos.longitud() - 1; ++j) {
            Vuelo a = vuelos.obtenerPos(j);
            Vuelo b = vuelos.obtenerPos(j + 1);
            if (claveFecha(a.getFecha()) > claveFecha(b.getFecha())) {
                vuelos.modificarPos(b, j);
                vuelos.modificarPos(a, j + 1);
            }
        }
    }

    Cola<Vuelo> cola;
    for (int i = 0; i < vuelos.longitud(); ++i)
        cola.encolar(vuelos.obtenerPos(i));

    cout << "\nVUELOS PROXIMOS (orden cronologico)\n"
        << "ID | Origen | Destino | Fecha | Precio | Asientos disp.\n";

    while (!cola.estaVacia()) {
        Vuelo v = cola.frente();
        cola.desencolar();
        cout << v.getId() << " | "
            << v.getOrigen() << " | "
            << v.getDestino() << " | "
            << v.getFecha() << " | S/ "
            << fixed << setprecision(2)
            << v.getPrecio() << " | "
            << v.getAsientosDisponibles() << '\n';
    }
}

void MenuAdministrador::opcionCerrarSesion() {
    cout << "Cerrando sesion...\n";
}
