#include "MenuAdministrador.h"
#include "Pila.h"
#include "Cola.h"
#include "Ordenamientos.h"
#include <iostream>
#include <iomanip>
#include <functional>
#include <conio.h>
using namespace std;

void MenuAdministrador::opcionRegistrarVuelo() {
    int y=13;
    string origen, destino, fecha;
    double precio;
    int capacidad = 0;
cursor(70,12);
    cout << "Origen: "; getline(cin, origen); cursor(70, y);y++;
    cout << "Destino: "; getline(cin, destino); cursor(70, y); y++;
    cout << "Fecha (dd/mm/aaaa): "; getline(cin, fecha); cursor(70, y); y++;
    cout << "Precio: "; cin >> precio; cursor(70, y); y++;

    while (capacidad < 1 || capacidad > 120) {
        cout << "Capacidad de asientos: "; cin >> capacidad; cursor(70, y); y++;
        cin.ignore(10000, '\n');
        if (capacidad > 120) cout << "Capacidad excedida"; cursor(70, y); y++;
        if (capacidad < 1)  cout << "Capacidad insuficiente"; cursor(70, y); y++;
    }

    int id = svcVuelos.listarVuelos().longitud() + 1;
    Vuelo v(id, origen, destino, fecha, precio, capacidad);

    cout << (svcVuelos.crearVuelo(v) ? "Vuelo registrado exitosamente con ID: " : "Error al registrar el vuelo. ID: ") << id << "\n";
    _getch();
}

void MenuAdministrador::opcionModificarVuelo() {
int Y=13;
    cursor(70, Y); Y++;
    int id; cout << "Ingrese ID del vuelo a modificar: "; cin >> id; cin.ignore(10000, '\n');
    Vuelo v;
    cursor(70, Y); Y++;
    if (!svcVuelos.buscarVuelo(id, v)) {
        cursor(70, Y); Y++;
        cout << "Vuelo no encontrado.\n";
        return;
    }
   
    cout << "Nuevo precio: "; double nuevoPrecio; cin >> nuevoPrecio;   cursor(70, Y); Y++;
    cout << "Nueva capacidad: "; int nuevaCap; cin >> nuevaCap; cin.ignore(10000, '\n');   cursor(70, Y); Y++;
    v.setPrecio(nuevoPrecio);
    v.setCapacidad(nuevaCap);
    cout << (svcVuelos.modificarVuelo(v) ? "Vuelo actualizado.\n" : "Error al actualizar el vuelo.\n");
    _getch();

}

void MenuAdministrador::opcionEliminarVuelo() {
    int Y = 13;
    cursor(70, Y); Y++;
    int id; cout << "Ingrese ID del vuelo a eliminar: "; cin >> id; cin.ignore(10000, '\n');  cursor(70, Y); Y++;
    cout << "Seguro que desea eliminar este vuelo? (s/n): "; char resp; cin >> resp; cin.ignore(10000, '\n');  cursor(70, Y); Y++;
    if (resp == 's' || resp == 'S')
        cout << (svcVuelos.eliminarVuelo(id) ? "Vuelo eliminado correctamente.\n" : "Error al eliminar vuelo.\n");
    _getch();

}

void MenuAdministrador::opcionHistorialReservas() {
int Y =13;
    cursor(70, Y); Y++;
    auto lista = svcReservas.listarTodasReservas();
    cout << "Codigo | Usuario | Vuelo ID | Fecha | Asientos\n";
    function<void(int)> imprimir = [&](int i) {
        if (i >= lista.longitud()) return;
        const auto& r = lista.obtenerPos(i);
        cursor(70, Y); Y++;
        cout << r.getCodigo() << " | " << r.getUserEmail() << " | " << r.getVueloId() << " | " << r.getFecha() << " | " << r.getAsientos().toPrint(";") << "\n";
        imprimir(i + 1);
        };
    imprimir(0);
_getch();
}

void MenuAdministrador::opcionVerUsuarios() {
    int Y=13;
    RepoUsuarios repo;
    auto lista = repo.cargarTodos();
    cursor(70, Y); Y++;
    cout << "Usuarios registrados:\n";
    function<void(int)> imprimir = [&](int i) {
        if (i >= lista.longitud()) return;
        const auto& u = lista.obtenerPos(i);
        cursor(70, Y); Y++;
        cout << (i + 1) << ". " << u.getNombre() << ' ' << u.getApellido() << " - " << u.getCorreo() << "\n";
        imprimir(i + 1);
        };
    imprimir(0);
_getch();
}

void MenuAdministrador::opcionVuelosMasCaros() {
    Lista<Vuelo> ordenados = svcVuelos.listarVuelos();
    mergeSortPorPrecioDesc(ordenados);

    if (ordenados.esVacia()) {
        cout << "No hay vuelos registrados.";
        _getch();
        return;
    }

    const int pageSize = 30;
    int total = ordenados.longitud();
    int page = 0;
    char tecla;

    do {
        HANDLE con = GetStdHandle(STD_OUTPUT_HANDLE);
        WORD BG_GRAY = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
        system("cls");
        menuAdmin();
        SetConsoleTextAttribute(con, BG_GRAY);
        cursor(6, 12);
        cout << "MENU DE ADMINISTRADOR - AIR PACIFIC" << endl; cursor(8, 13);
        MenuBase::mostrar();
        int Y = 4;
        cursor(70, Y++); cout << "VUELOS MAS CAROS (precio descendente)";
        cursor(70, Y++); cout << "ID | Origen | Destino | Fecha       | Precio   | Asientos disp.";
        cursor(70, Y++); cout << "---------------------------------------------------------------";

        int start = page * pageSize;
        int end = min(start + pageSize, total);
        for (int i = start; i < end; ++i) {
            const auto& v = ordenados.obtenerPos(i);
            int disp = svcReservas
                .listarAsientosDisponibles(v.getId())
                .longitud();
            cursor(70, Y++);
            cout
                << setw(2) << v.getId() << " | "
                << setw(6) << v.getOrigen() << " | "
                << setw(7) << v.getDestino() << " | "
                << setw(10) << v.getFecha() << " | S/"
                << fixed << setprecision(2)
                << setw(7) << v.getPrecio() << " | "
                << setw(5) << disp << "\n";
        }

        cursor(70, Y += 1);
        cout << "Mostrando " << (start + 1) << "--" << end << " DE " << total;
        cursor(70, Y += 1);
        cout << "[L] Siguiente  [J] Anterior  [Q] Salir";

        tecla = toupper(_getch());
        if (tecla == 'L' && page < (total - 1) / pageSize) ++page;
        else if (tecla == 'J' && page > 0) --page;

    } while (tecla != 'Q');
}

void MenuAdministrador::opcionVerTodosVuelos() {
    Lista<Vuelo> vuelos = svcVuelos.listarVuelosPorFecha();
    if (vuelos.esVacia()) {
        cout << "No hay vuelos próximos registrados.";
        _getch();
        return;
    }

    const int pageSize = 30;
    int total = vuelos.longitud();
    int page = 0;
    char tecla;

    do {
        HANDLE con = GetStdHandle(STD_OUTPUT_HANDLE);
        WORD BG_GRAY = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
        system("cls");
        menuAdmin();
        SetConsoleTextAttribute(con, BG_GRAY);
        cursor(6, 12);
        cout << "MENU DE ADMINISTRADOR - AIR PACIFIC" << endl; cursor(8, 13);
        MenuBase::mostrar();
        int Y = 4;
        cursor(70, Y++); cout << "VUELOS PROXIMOS (orden cronologico - AVL)";
        cursor(70, Y++); cout << "ID | Origen | Destino | Fecha       | Precio   | Asientos disp.";
        cursor(70, Y++); cout << "---------------------------------------------------------------";

        int start = page * pageSize;
        int end = min(start + pageSize, total);
        for (int i = start; i < end; ++i) {
            const auto& v = vuelos.obtenerPos(i);
            int disp = svcReservas
                .listarAsientosDisponibles(v.getId())
                .longitud();
            cursor(70, Y++);
            cout
                << setw(2) << v.getId() << " | "
                << setw(6) << v.getOrigen() << " | "
                << setw(7) << v.getDestino() << " | "
                << setw(10) << v.getFecha() << " | S/"
                << fixed << setprecision(2)
                << setw(7) << v.getPrecio() << " | "
                << setw(5) << disp << "\n";
        }

        cursor(70, Y += 1);
        cout << "Mostrando " << (start + 1) << "--" << end << " DE " << total;
        cursor(70, Y += 1);
        cout << "[L] Siguiente  [J] Anterior  [Q] Salir";

        tecla = toupper(_getch());
        if (tecla == 'L' && page < (total - 1) / pageSize) ++page;
        else if (tecla == 'J' && page > 0) --page;

    } while (tecla != 'Q');
}


void MenuAdministrador::opcionProcesarCheckIn() {
int Y=13;

    CheckIn c;
    if (!svcCheckIn.procesarSiguiente(c)) {
        cursor(70, Y); Y++;
        cout << "No hay pasajeros en la cola de check-in.\n";
        return;
    }
    cursor(70, Y); Y++;
    cout << "Check-in procesado: "
        << c.getEmail() << "  (reserva " << c.getReservaCod() << ")\n";
_getch();
}

void MenuAdministrador::opcionVerPendientesCheckIn() {
    int Y = 13;
    const Cola<CheckIn>& cola = svcCheckIn.pendientes();

    if (cola.size() == 0) {
        cursor(70, Y++);
        cout << "La cola de check-in esta vacia.";
        _getch();
        return;
    }

    cursor(70, Y++);
    cout << "Pasajeros esperando check-in:";

    for (int i = 0; i < cola.size(); ++i) {
        const CheckIn& c = cola.at(i);
        cursor(70, Y++);
        cout << (i + 1) << ") "
            << c.getEmail()
            << " (reserva " << c.getReservaCod() << ")";
    }

    _getch();
}



void MenuAdministrador::opcionVerHistorialCheckIn() {
    int Y = 13;
    const Pila<CheckIn>& pila = svcCheckIn.ultimos();
    int n = pila.size();

    if (n == 0) {
        cursor(70, Y++);
        cout << "No hay historial de check-in en esta sesion.";
        _getch();
        return;
    }

    cursor(70, Y++);
    cout << "Ultimos check-in procesados (mas reciente primero):";

    for (int i = 0; i < n; ++i) {
        const CheckIn& c = pila.at(i);
        cursor(70, Y++);
        cout << "* " << c.getEmail()
            << "  (reserva " << c.getReservaCod() << ")";
    }

    _getch();
}


void MenuAdministrador::opcionRegistrarAeropuerto() {
    int Y = 13;
    string cod; int x, y;
    cursor(70, 12);
    cout << "Código IATA: ";   cin >> cod; cursor(70, Y);Y++;
    cout << "Coordenada X : "; cin >> x; cursor(70, Y); Y++;
    cout << "Coordenada Y : "; cin >> y; cursor(70, Y); Y++;

    if (svcAeropuertos.existe(cod)) {
        cout << "Ya existe un aeropuerto con ese código.\n"; cursor(70, Y); Y++;
    }
    else if (svcAeropuertos.crear(Aeropuerto(cod, x, y))) {
        cout << "Aeropuerto registrado.\n"; cursor(70, Y); Y++;
    }
    else {
        cout << "No se pudo registrar el aeropuerto.\n";
    }
    cin.ignore(); cin.get();
}
    
void MenuAdministrador::generarUsuarios() {
    int Y = 13;
    int cantidad = 0;
    cursor(70, Y++);
    cout << "Cuantos usuarios desea generar ";
    cin >> cantidad;
    if (cantidad <= 0) return;

    RepoUsuarios repo;
    auto existentes = repo.cargarTodos();
    int inicio = existentes.longitud();  

    for (int i = 1; i <= cantidad; ++i) {
        int idx = inicio + i;  
 
        string dni = to_string(idx);


        string nom = "nombre" + to_string(idx);
        string ape = "apellido" + to_string(idx);
        string mail = "correo" + to_string(idx);
        string pass = "contrasena" + to_string(idx);

        Usuario u(dni, nom, ape, mail, pass);
        repo.agregar(u);
    }
    cursor(70, Y++);
    cout << "Se agregaron " << cantidad
        << " usuarios (del " << (inicio + 1)
        << " al " << (inicio + cantidad)
        << ") en usuarios.txt\n";
_getch();   
}

void MenuAdministrador::opcionVerAeropuertos() {
    int Y = 13;
    cursor(70, Y++);
    auto lista = svcAeropuertos.listar();

    if (lista.esVacia()) {
        cursor(70, Y++);
        cout << "No hay aeropuertos registrados\n";
    }
    else {
        timSortAeropuertos(lista);

        cursor(70, Y++);
        cout << "COD   X    Y\n";
        for (int i = 0; i < lista.longitud(); ++i) {
            const Aeropuerto& a = lista.obtenerPos(i);
            cursor(70, Y++);
            cout << a.getCodigo()
                << "   " << a.getX()
                << "    " << a.getY() << "\n";
        }
    }

    cin.ignore();
    cin.get();
}

void MenuAdministrador::opcionCerrarSesion() {
    cout << "Cerrando sesion...\n";
}