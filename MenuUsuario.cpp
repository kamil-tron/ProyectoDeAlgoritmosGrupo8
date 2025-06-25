#include "MenuUsuario.h"
#include "Reserva.h"
#include "Pago.h"
#include <sstream>
#include <ctime>
#include <Windows.h>
#include <cctype>
#include <iostream>
#include <iomanip>
#include "Usuario.h"
#include "AuthService.h"
#include "Ordenamientos.h"

using namespace std;

void cuentaRegresiva(int n) {
    if (n < 1) return;
    cout << n << endl;
    Sleep(1000);
    cuentaRegresiva(n - 1);
}

void MenuUsuario::imprimirReservasRecursivo(const Lista<Reserva>& reservas, int index) {
    if (index >= reservas.longitud()) return;

    const auto& r = reservas.obtenerPos(index);
    Vuelo v;
    svcVuelos.buscarVuelo(r.getVueloId(), v);
    auto allSeats = v.listarAsientos();
    double total = 0.0;

    for (int k = 0; k < r.getAsientos().longitud(); ++k) {
        const string& codigo = r.getAsientos().obtenerPos(k);
        for (int m = 0; m < allSeats.longitud(); ++m) {
            const Asiento& a = allSeats.obtenerPos(m);
            if (a.getCodigo() == codigo) {
                double precio = v.getPrecio();
                if (a.isVip()) precio *= 1.20;
                total += precio;
                break;
            }
        }
    }

    cout << r.getCodigo() << " | "
        << r.getVueloId() << " | "
        << r.getFecha() << " | "
        << r.getAsientos().toPrint(" ") << " | "
        << (r.getEstado() == EstadoReserva::CONFIRMADA ? "CONF" :
            r.getEstado() == EstadoReserva::PENDIENTE ? "PEND" : "CANC")
        << " | S/ " << fixed << setprecision(2) << total << "\n";

    imprimirReservasRecursivo(reservas, index + 1);
}

int MenuUsuario::contarConfirmadasRec(const Lista<Reserva>& lista, int i) {
    if (i >= lista.longitud()) return 0;
    int suma = (lista.obtenerPos(i).getEstado() == EstadoReserva::CONFIRMADA) ? 1 : 0;
    return suma + contarConfirmadasRec(lista, i + 1);
}

void MenuUsuario::opcionBuscarYReservar() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    WORD defaultAttrs = csbi.wAttributes;

    auto todos = svcVuelos.listarVuelos();
    if (todos.esVacia()) {
        cout << "No hay vuelos disponibles.\n";
        return;
    }

    insertionSortPorPrecioAsc(todos);

    cout << "\nVUELOS DISPONIBLES\n";
    cout << "ID | Origen | Destino | Fecha       | Precio\n";

    for (int i = 0; i < todos.longitud(); ++i) {
        const Vuelo& v = todos.obtenerPos(i);
        cout << setw(2) << v.getId() << " | "
            << setw(6) << v.getOrigen() << " | "
            << setw(7) << v.getDestino() << " | "
            << setw(11) << v.getFecha() << " | S/ "
            << fixed << setprecision(2) << v.getPrecio() << ' ';
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
        cout << "+20%";
        SetConsoleTextAttribute(hConsole, defaultAttrs);
        cout << '\n';
    }

    cout << "\nDesea reservar alguno de estos vuelos? (1=Si, 2=No): ";
    int opc; cin >> opc; cin.ignore(10000, '\n');
    if (opc != 1) return;

    cout << "Seleccione ID del vuelo: ";
    int id; cin >> id; cin.ignore(10000, '\n');

    Vuelo v;
    if (!svcVuelos.buscarVuelo(id, v)) {
        cout << "Vuelo no encontrado.\n";
        return;
    }

    reservarVuelo(id);
}

void MenuUsuario::reservarVuelo(int id) {
    Vuelo vuelo;
    if (!svcVuelos.buscarVuelo(id, vuelo)) {
        cout << "Vuelo no encontrado.\n";
        return;
    }

    auto libres = svcReservas.listarAsientosDisponibles(id);
    if (libres.esVacia()) {
        cout << "No hay asientos disponibles.\n";
        return;
    }

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    WORD defaultAttrs = csbi.wAttributes;

    cout << "\nAsientos libres:\n";
    string filaAnt;

    for (int i = 0; i < libres.longitud(); ++i) {
        const Asiento& a = libres.obtenerPos(i);
        string cod = a.getCodigo();

        string fila;
        for (char c : cod) if (isdigit(c)) fila += c; else break;
        if (!filaAnt.empty() && fila != filaAnt) cout << '\n';

        if (a.isVip()) {
            SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN);
            cout << cod;
            SetConsoleTextAttribute(hConsole, defaultAttrs);
            cout << ' ';
        }
        else {
            cout << cod << ' ';
        }
        filaAnt = fila;
    }
    cout << "\n\n";

    cout << "Cantidad a reservar: ";
    int cant; cin >> cant; cin.ignore(10000, '\n');
    if (cant <= 0 || cant > libres.longitud()) {
        cout << "Cantidad invalida.\n";
        return;
    }

    Lista<string> seleccion;
    cout << "Ingrese los codigos de asiento separados por espacio:\n";
    for (int i = 0; i < cant; ++i) {
        string code; cin >> code;
        seleccion.agregaFinal(code);
    }
    cin.ignore(10000, '\n');

    Usuario& u = static_cast<Usuario&>(sesion.getUsuarioActual());
    Reserva nueva;
    if (!svcReservas.crearReservaConAsientos(u, id, seleccion, nueva)) {
        cout << "No fue posible completar la reserva.\n";
        return;
    }

    double totalPago = svcReservas.calcularTotal(id, seleccion);

    cout << "\nMetodo de pago:\n"
        << "1. Tarjeta de credito\n"
        << "2. Tarjeta de debito\n"
        << "3. Yape/Plin\n"
        << "Opcion: ";
    int metodoOp; cin >> metodoOp; cin.ignore(10000, '\n');
    string metodo = (metodoOp == 1 ? "TC" : metodoOp == 2 ? "TD" : "YP");

    cout << "Procesando pago...\n";
    cuentaRegresiva(3);

    Pago pagoConfirmado;
    bool exito = svcPagos.procesarPagoReserva(nueva, totalPago, metodo, pagoConfirmado);

    if (exito) {
        cout << "Reserva y pago confirmados!!!\n"
            << "Codigo reserva: " << nueva.getCodigo() << "\n";
    }
    else {
        cout << "El pago fallo o ya existe un pago para esta reserva.\n";
    }
}

void MenuUsuario::opcionVerReservas() {
    auto reservas = svcReservas.listarReservasUsuario(sesion.getUsuarioActual().getCorreo());
    if (reservas.esVacia()) {
        cout << "\nNo tienes reservas.\n";
        return;
    }

    cout << "\nMIS RESERVAS\n"
        << "Codigo | Vuelo | Fecha | Asientos | Estado | Total\n";

    imprimirReservasRecursivo(reservas, 0);

    cout << "\nDeseas cancelar alguna reserva? (1=Si, 2=No): ";
    int opc; cin >> opc; cin.ignore(10000, '\n');
    if (opc != 1) return;

    cout << "Codigo de la reserva a cancelar: ";
    string codigo;
    getline(cin, codigo);

    if (svcReservas.cancelarReserva(codigo))
        cout << "Reserva cancelada.\n";
    else
        cout << "No se encontro o ya estaba cancelada.\n";
}

void MenuUsuario::opcionVerPerfil() {
    cout << "\nMI PERFIL\n";
    sesion.getUsuarioActual().mostrarPerfil();

    auto lista = svcReservas.listarReservasUsuario(sesion.getUsuarioActual().getCorreo());
    int activas = contarConfirmadasRec(lista, 0);
    cout << "Reservas confirmadas: " << activas << '\n';

    cout << "\nDeseas actualizar tu perfil?\n";
    cout << "1. Si\n2. No\nOpcion: ";
    int opc; cin >> opc; cin.ignore(10000, '\n');
    if (opc != 1) return;

    string nuevoNombre, nuevoApellido, nuevaContrasena;
    cout << "Nuevo nombre: "; getline(cin, nuevoNombre);
    cout << "Nuevo apellido: "; getline(cin, nuevoApellido);
    cout << "Nueva contrasena: "; getline(cin, nuevaContrasena);

    Usuario& u = static_cast<Usuario&>(sesion.getUsuarioActual());
    u.setNombre(nuevoNombre);
    u.setApellido(nuevoApellido);
    u.setPassword(nuevaContrasena);

    AuthService auth;
    auth.getRepoUsuarios().actualizar(u);

    cout << "Perfil actualizado correctamente.\n";
}

void MenuUsuario::opcionCerrarSesion() {
    cout << "Cerrando sesion...\n";
}
