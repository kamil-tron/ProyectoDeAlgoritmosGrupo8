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
#include "MatrizMapa.h"

using namespace std;

void cuentaRegresiva(int n) {
    if (n < 1) return;
    cout << n << endl;
    Sleep(1000);
    cuentaRegresiva(n - 1);
}

static string elegirMetodoPago() {
    cout << "\nMetodo de pago:\n"
        << "1. Tarjeta de credito\n"
        << "2. Tarjeta de debito\n"
        << "3. Yape / Plin\n"
        << "Opcion: ";
    int op; cin >> op; cin.ignore(10000, '\n');
    return (op == 1 ? "TC" : op == 2 ? "TD" : "YP");
}


 
static void imprimirMapaRuta(const RutaPosible& ruta, ServicioRutas& svcRutas)
{
    // 1. Respaldar matrizPeru sin memcpy
    int backup[50][101];
    for (int f = 0; f < 50; ++f)
        for (int c = 0; c < 101; ++c)
            backup[f][c] = matrizPeru[f][c];
    system("cls");
    // 2. Pintar la ruta y mostrar el mapa
    svcRutas.pintarRutaEnMatriz(ruta, /*valorLinea*/ 4, /*valorNodo*/ 9);
    cursor(0, 50);
    std::cout << "\n==== MAPA DE LA RUTA ====\n";
    imprimirMapaPeru(25,0);
    std::cout << "\nPresiona ENTER para continuar...";
    std::cin.ignore(10000, '\n');
    std::cin.get();

    // 3. Restaurar matrizPeru
    for (int f = 0; f < 50; ++f)
        for (int c = 0; c < 101; ++c)
            matrizPeru[f][c] = backup[f][c];
}


void MenuUsuario::imprimirReservasRecursivo(const Lista<Reserva>& reservas, int index, int Y) {

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

    cursor(70, Y); Y++;
    cout << r.getCodigo() << " | "
        << r.getVueloId() << " | "
        << r.getFecha() << " | "
        << r.getAsientos().toPrint(" ") << " | "
        << (r.getEstado() == EstadoReserva::CONFIRMADA ? "CONF" :
            r.getEstado() == EstadoReserva::PENDIENTE ? "PEND" : "CANC")
        << " | S/ " << fixed << setprecision(2) << total << "\n";

    imprimirReservasRecursivo(reservas, index + 1,Y);
}

int MenuUsuario::contarConfirmadasRec(const Lista<Reserva>& lista, int i) {
    if (i >= lista.longitud()) return 0;
    int suma = (lista.obtenerPos(i).getEstado() == EstadoReserva::CONFIRMADA) ? 1 : 0;
    return suma + contarConfirmadasRec(lista, i + 1);
}

void MenuUsuario::opcionBuscarYReservar()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    WORD defaultAttrs = csbi.wAttributes;

    cout << "\n--- BUSCAR VUELOS O RUTAS ---\n";

    auto aeropuertos = svcVuelos.listarAeropuertos();
    if (aeropuertos.esVacia()) {
        cout << "No hay aeropuertos registrados.\n";
        return;
    }
    cout << "\nAEROPUERTOS DISPONIBLES:\n";
    for (int i = 0; i < aeropuertos.longitud(); ++i) {
        const Aeropuerto& a = aeropuertos.obtenerPos(i);
        cout << "- " << a.getCodigo()
            << "  (X: " << a.getX() << ", Y: " << a.getY() << ")\n";
    }

    string origen, destino;
    cout << "\nIngrese el codigo IATA de origen  : ";  cin >> origen;
    cout << "Ingrese el codigo IATA de destino : ";  cin >> destino;

    cout << "\nOrdenar las rutas con escalas por:\n"
        << "  1. Costo total (mas baratas primero)\n"
        << "  2. Distancia total (mas cortas primero)\n"
        << "Opcion: ";
    int crit;  cin >> crit;  cin.ignore(10000, '\n');
    bool porCosto = (crit == 1);

    Lista<Vuelo> vuelosDirectos;
    auto todos = svcVuelos.listarVuelosPorFecha();
    for (int i = 0; i < todos.longitud(); ++i) {
        const Vuelo& v = todos.obtenerPos(i);
        if (v.getOrigen() == origen && v.getDestino() == destino)
            vuelosDirectos.agregaFinal(v);
    }

    if (vuelosDirectos.longitud() > 1)
        mergeSortPorPrecioAsc(vuelosDirectos);

    Lista<RutaPosible> rutas = svcRutas.mejoresKRutas(origen, destino, 10);

    mergeSortRutas(rutas, porCosto);

    if (vuelosDirectos.esVacia() && rutas.esVacia()) {
        cout << "\nNo se encontraron vuelos ni rutas entre los aeropuertos.\n";
        return;
    }

    cout << "\nOpciones disponibles:\n";

    struct OpcionRuta { bool esDirecto; int index; };
    Lista<OpcionRuta> opciones;
    int opcionActual = 1;

    if (!vuelosDirectos.esVacia()) {
        cout << "\nVUELOS DIRECTOS:\n";
        for (int i = 0; i < vuelosDirectos.longitud(); ++i) {
            const Vuelo& v = vuelosDirectos.obtenerPos(i);
            cout << "[" << opcionActual << "] "
                << v.getOrigen() << "->" << v.getDestino() << ' '
                << v.getFecha() << "  S/"
                << fixed << setprecision(2) << v.getPrecio() << '\n';
            opciones.agregaFinal({ true, v.getId() });
            ++opcionActual;
        }
    }

    if (!rutas.esVacia()) {
        cout << "\nRUTAS CON ESCALAS:\n";
        for (int i = 0; i < rutas.longitud(); ++i) {
            const RutaPosible& r = rutas.obtenerPos(i);
            const auto& inicio = r.vuelos.obtenerPos(0);
            const auto& fin = r.vuelos.obtenerFinal();
            cout << "[" << opcionActual << "] "
                << r.vuelos.longitud() << " tramos  "
                << inicio.getFecha() << "->" << fin.getFecha()
                << "  S/" << fixed << setprecision(2) << r.costoTotal << '\n';
            opciones.agregaFinal({ false, i });
            ++opcionActual;
        }
    }

    cout << "\nSeleccione una opcion para reservar (0=Cancelar): ";
    int sel;  cin >> sel;  cin.ignore(10000, '\n');
    if (sel < 1 || sel > opciones.longitud()) return;

    OpcionRuta elegida = opciones.obtenerPos(sel - 1);

    if (elegida.esDirecto) {
        Vuelo vSel;
        if (!svcVuelos.buscarVuelo(elegida.index, vSel)) {
            cout << "No se pudo recuperar la informacion del vuelo.\n";
            return;
        }
        RutaPosible rutaDir = svcRutas.rutaMasCorta(vSel.getOrigen(), vSel.getDestino());
        imprimirMapaRuta(rutaDir, svcRutas);
        reservarVuelo(elegida.index);
    }
    else {
        const RutaPosible& rutaSel = rutas.obtenerPos(elegida.index);
        imprimirMapaRuta(rutaSel, svcRutas);
        reservarRuta(rutaSel);
    }
}


void MenuUsuario::reservarVuelo(int vueloId) {
    Usuario& u = static_cast<Usuario&>(sesion.getUsuarioActual());
    string metodo = elegirMetodoPago();
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    WORD defaultAttrs = csbi.wAttributes;

    // Obtener asientos disponibles para el vuelo
    auto libres = svcReservas.listarAsientosDisponibles(vueloId);
    if (libres.esVacia()) {
        cout << "No hay asientos disponibles en el vuelo " << vueloId << ".\n";
        return;
    }

    // Mostrar asientos libres
    cout << "Asientos libres para vuelo " << vueloId << ":\n";
    string filaAnt;
    for (int i = 0; i < libres.longitud(); ++i) {
        const Asiento& a = libres.obtenerPos(i);
        string cod = a.getCodigo();

        string fila;
        for (char c : cod) if (isdigit(c)) fila += c; else break;
        if (!filaAnt.empty() && fila != filaAnt) cout << "\n";
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
    cout << "\nCantidad de asientos a reservar: ";
    int cantidad;
    cin >> cantidad;
    cin.ignore(10000, '\n');
    if (cantidad < 1 || cantidad > libres.longitud()) {
        cout << "Cantidad inválida.\n";
        return;
    }

    // Selección de códigos
    Lista<string> seleccionados;
    cout << "Ingrese los codigos de asiento separados por espacio:\n";
    for (int i = 0; i < cantidad; ++i) {
        string codigo;
        cin >> codigo;
        seleccionados.agregaFinal(codigo);
    }
    cin.ignore(10000, '\n');

    // Crear reserva y procesar pago
    Reserva reserva;
    if (!svcReservas.crearReservaConAsientos(u, vueloId, seleccionados, reserva)) {
        cout << "No se pudo crear la reserva.\n";
        return;
    }

    double total = svcReservas.calcularTotal(vueloId, seleccionados);
    Pago pago;
    if (svcPagos.procesarPagoReserva(reserva, total, metodo, pago)) {
        cout << "Reserva confirmada: " << reserva.getCodigo()
            << " (S/" << fixed << setprecision(2) << total << ")\n";
    }
    else {
        cout << "Pago fallido para la reserva.\n";
    }
}
void MenuUsuario::opcionBuscarRutaYReservar() {
    string origen, destino;
    cout << "\n--- BUSCAR RUTA CON ESCALAS ---\n";
    cout << "Codigo IATA origen  : "; cin >> origen;
    cout << "Codigo IATA destino : "; cin >> destino;

    cout << "\nCriterio (1=Mas barata, 2=Mas corta): ";
    int crit;
    cin >> crit;
    cin.ignore(10000, '\n');

    auto rutas = svcRutas.mejoresKRutas(origen, destino, 10);
    if (rutas.esVacia()) {
        cout << "No se encontraron rutas disponibles.\n";
        return;
    }

    cout << "\nRUTAS DISPONIBLES:\n";
    for (int i = 0; i < rutas.longitud(); ++i) {
        const auto& r = rutas.obtenerPos(i);
        const auto& inicio = r.vuelos.obtenerPos(0);
        const auto& fin = r.vuelos.obtenerFinal();
        cout << "[" << i + 1 << "] " << r.vuelos.longitud() << " tramos"
            << " " << inicio.getFecha() << "->" << fin.getFecha()
            << " S/" << fixed << setprecision(2) << r.costoTotal << '\n';
    }
    cout << "Seleccione ruta a reservar (1-" << rutas.longitud() << ", 0=Cancelar): ";
    int sel;
    cin >> sel;
    cin.ignore(10000, '\n');
    if (sel < 1 || sel > rutas.longitud()) return;

    reservarRuta(rutas.obtenerPos(sel - 1));
}
void MenuUsuario::reservarRuta(const RutaPosible& ruta) {
    Usuario& u = static_cast<Usuario&>(sesion.getUsuarioActual());
    string metodo = elegirMetodoPago();
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    WORD defaultAttrs = csbi.wAttributes;

    double totalRuta = 0.0;
    int exitosas = 0;

    for (int i = 0; i < ruta.vuelos.longitud(); ++i) {
        const Vuelo& v = ruta.vuelos.obtenerPos(i);
        cout << "\n--- Vuelo " << v.getId()
            << " (" << v.getOrigen() << "->" << v.getDestino() << ") ---\n";

        auto libres = svcReservas.listarAsientosDisponibles(v.getId());
        if (libres.esVacia()) {
            cout << "No hay asientos disponibles en este tramo.\n";
            continue;
        }

        cout << "Asientos libres:\n";
        string filaAnt;
        for (int j = 0; j < libres.longitud(); ++j) {
            const Asiento& a = libres.obtenerPos(j);
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
        cout << "\nCantidad a reservar: ";
        int cnt; cin >> cnt; cin.ignore(10000, '\n');
        if (cnt < 1 || cnt > libres.longitud()) { --i; continue; }

        Lista<string> sel;
        cout << "Ingrese codigos separados por espacio:\n";
        for (int k = 0; k < cnt; ++k) {
            string code; cin >> code; sel.agregaFinal(code);
        }
        cin.ignore(10000, '\n');

        Reserva nueva;
        if (!svcReservas.crearReservaConAsientos(u, v.getId(), sel, nueva)) {
            cout << "No se pudo reservar estos asientos.\n";
            continue;
        }

        double totalVuelo = svcReservas.calcularTotal(v.getId(), sel);
        Pago pago;
        if (svcPagos.procesarPagoReserva(nueva, totalVuelo, metodo, pago)) {
            exitosas++;
            totalRuta += totalVuelo;
            cout << "Reserva confirmada: " << nueva.getCodigo()
                << " (S/" << fixed << setprecision(2) << totalVuelo << ")\n";
        }
        else {
            cout << "Pago fallido para este tramo.\n";
        }
    }

    if (exitosas) {
        cout << "\nRuta reservada exitosamente.";
        cout << " Total pagado: S/" << fixed << setprecision(2) << totalRuta << "\n";
    }
    else {
        cout << "\nNo se concretaron reservas en la ruta.\n";
    }
}


void MenuUsuario::opcionVerReservas() {
    int Y = 12;

    auto reservas = svcReservas.listarReservasUsuario(sesion.getUsuarioActual().getCorreo());
    if (reservas.esVacia()) {
        cursor(70, Y); Y++;
        cout << "\nNo tienes reservas.\n";
        return;
    }
    cursor(70, Y); Y++;
    cout << "MIS RESERVAS";
    cursor(70, Y); 
    cout<< "Codigo | Vuelo | Fecha | Asientos | Estado | Total";

    imprimirReservasRecursivo  (reservas, 0,Y);
    int n = reservas.longitud();
    Y += n;
    cursor(70, Y); Y++;
    cout << "Deseas cancelar alguna reserva? (1=Si, 2=No): ";
    int opc; cin >> opc; cin.ignore(10000, '\n');
    if (opc != 1) return;
    cursor(70, Y); Y++;
    cout << "Codigo de la reserva a cancelar: ";
    string codigo;
    getline(cin, codigo);

    if (svcReservas.cancelarReserva(codigo)){
        cursor(70, Y); Y++;
        cout << "Reserva cancelada";
        _getch();
    }else
        cursor(70, Y); Y++;
        cout << "No se encontro o ya estaba cancelada.";
        _getch();
}

void MenuUsuario::opcionVerPerfil() {
    int Y = 12;
    cursor(70, Y); Y+=4;
    cout << "MI PERFIL";
    sesion.getUsuarioActual().mostrarPerfil();

    auto lista = svcReservas.listarReservasUsuario(sesion.getUsuarioActual().getCorreo());
    int activas = contarConfirmadasRec(lista, 0);
    cursor(70, Y); Y++;
    cout << "Reservas confirmadas: " << activas;
    cursor(70, Y); Y++;
    cout << "Deseas actualizar tu perfil?"; cursor(70, Y); Y++;
    cout << "1. Si"; cursor(70, Y); Y++;
    cout<<"2. No"; cursor(70, Y); Y++;
    cout<<"Opcion: "; 
    int opc; cin >> opc; cin.ignore(10000, '\n');
    if (opc != 1) return;

    string nuevoNombre, nuevoApellido, nuevaContrasena;
    cursor(70, Y); Y++;
    cout << "Nuevo nombre: "; getline(cin, nuevoNombre); 
    cursor(70, Y); Y++; cout << "Nuevo apellido: "; getline(cin, nuevoApellido);
    cursor(70, Y); Y++; cout << "Nueva contrasena: "; getline(cin, nuevaContrasena);

    Usuario& u = static_cast<Usuario&>(sesion.getUsuarioActual());
    u.setNombre(nuevoNombre);
    u.setApellido(nuevoApellido);
    u.setPassword(nuevaContrasena);

    AuthService auth;
    auth.getRepoUsuarios().actualizar(u);
    cursor(70, Y); Y++;
    cout << "Perfil actualizado correctamente.\n";
_getch();
}

void MenuUsuario::opcionHacerCheckIn() {
    int Y=13;
    auto reservas = svcReservas.listarReservasUsuario(sesion.getUsuarioActual().getCorreo());
    string codigo; 
cursor(70, Y); Y++;
    cout << "MIS RESERVAS";
    cursor(70, Y);
    cout << "Codigo | Vuelo | Fecha | Asientos | Estado | Total";

    imprimirReservasRecursivo(reservas, 0, Y);
    int n = reservas.longitud();
    Y += n;
    cursor(70, Y); Y++;
    cout << "Ingrese codigo de reserva confirmada: ";
    cin >> codigo;

    if (svcCheckIn.registrarEnCola(codigo)) {
        cursor(70, Y); Y++;
        cout << "Te uniste a la cola de check-in. ¡Gracias!";
    }
    else {
        cursor(70, Y); Y++;
        cout << "No se pudo registrar el check-in. ";
        cursor(70, Y); Y++;
            "Verifica que la reserva este confirmada y no hayas hecho check-in antes.";
    }
    _getch();
}

void MenuUsuario::opcionCerrarSesion() {
    cout << "Cerrando sesion...\n";
}
