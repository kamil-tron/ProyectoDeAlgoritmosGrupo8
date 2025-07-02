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
#include <algorithm>

using namespace std;


static void imprimirMapaRuta(const RutaPosible& ruta, ServicioRutas& svcRutas)
{
    // 1. Respaldar matrizPeru sin memcpy
    int backup[50][101];
    for (int f = 0; f < 50; ++f)
        for (int c = 0; c < 101; ++c)
            backup[f][c] = matrizPeru[f][c];

    // 2. Pintar la ruta y mostrar el mapa
    svcRutas.pintarRutaEnMatriz(ruta, /*valorLinea*/ 4, /*valorNodo*/ 9);
    std::cout << "\n======== MAPA DE LA RUTA ========\n";
    imprimirMapaPeru();
    std::cout << "\nPresiona ENTER para continuar...";
    std::cin.ignore(10000, '\n');   // descartar resto de línea
    std::cin.get();                 // esperar ENTER

    // 3. Restaurar matrizPeru sin memcpy
    for (int f = 0; f < 50; ++f)
        for (int c = 0; c < 101; ++c)
            matrizPeru[f][c] = backup[f][c];
}

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

void MenuUsuario::opcionBuscarYReservar()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    WORD defaultAttrs = csbi.wAttributes;

    std::cout << "\n--- BUSCAR VUELOS O RUTAS ---\n";

    auto aeropuertos = svcVuelos.listarAeropuertos();
    if (aeropuertos.esVacia()) {
        std::cout << "No hay aeropuertos registrados.\n";
        return;
    }

    std::cout << "\nAEROPUERTOS DISPONIBLES:\n";
    for (int i = 0; i < aeropuertos.longitud(); ++i) {
        const Aeropuerto& a = aeropuertos.obtenerPos(i);
        std::cout << "- " << a.getCodigo()
            << "  (X: " << a.getX() << ", Y: " << a.getY() << ")\n";
    }

    std::string origen, destino;
    std::cout << "\nIngrese el codigo IATA de origen  : "; std::cin >> origen;
    std::cout << "Ingrese el codigo IATA de destino : "; std::cin >> destino;

    auto rutasDistancia = svcRutas.todasLasRutasMasCortas(origen, destino);
    auto rutasCosto = svcRutas.todasLasRutasMasBaratas(origen, destino);

    if (rutasDistancia.esVacia() && rutasCosto.esVacia()) {
        std::cout << "\nNo se encontraron rutas entre los aeropuertos.\n";
        return;
    }

    // ---- nueva estructura ----
    struct OpcionRuta {
        RutaPosible ruta;        // la ruta exacta mostrada
        CriterioPeso criterio;   // solo informativo
    };
    Lista<OpcionRuta> opciones;
    int opcionActual = 1;

    // ---- lambda para listar rutas y llenar 'opciones' ----
    auto mostrarRutas = [&](const Lista<RutaPosible>& rutas,
        CriterioPeso criterio,
        const std::string& titulo)
        {
            bool mostroDirectos = false;
            bool mostroEscalas = false;

            for (int i = 0; i < rutas.longitud(); ++i) {
                const RutaPosible& r = rutas.obtenerPos(i);
                int n = r.vuelos.longitud();
                if (n == 0) continue;          // ruta invalida

                if (n == 1) {                  // vuelo directo
                    if (!mostroDirectos) {
                        std::cout << "\n" << titulo << " - VUELOS DIRECTOS:\n";
                        mostroDirectos = true;
                    }
                    const Vuelo& v = r.vuelos.obtenerPos(0);
                    std::cout << "[" << opcionActual << "] "
                        << v.getOrigen() << "->" << v.getDestino()
                        << " " << v.getFecha()
                        << "  S/" << std::fixed << std::setprecision(2)
                        << v.getPrecio() << '\n';
                }
                else {                         // con escalas
                    if (!mostroEscalas) {
                        std::cout << "\n" << titulo << " - RUTAS CON ESCALAS:\n";
                        mostroEscalas = true;
                    }
                    const Vuelo& inicio = r.vuelos.obtenerPos(0);
                    const Vuelo& fin = r.vuelos.obtenerFinal();
                    std::cout << "[" << opcionActual << "] "
                        << n << " tramos  "
                        << inicio.getFecha() << " -> " << fin.getFecha()
                        << "  S/" << std::fixed << std::setprecision(2)
                        << r.costoTotal << '\n';
                }

                opciones.agregaFinal({ r, criterio });
                ++opcionActual;
            }
        };

    mostrarRutas(rutasDistancia, CriterioPeso::DISTANCIA, "SEGUN DISTANCIA");
    mostrarRutas(rutasCosto, CriterioPeso::COSTO, "SEGUN COSTO");

    std::cout << "\nSeleccione una opcion para reservar (0 = Cancelar): ";
    int sel; std::cin >> sel; std::cin.ignore(10000, '\n');
    if (sel < 1 || sel > opciones.longitud()) return;

    const RutaPosible& rutaElegida = opciones.obtenerPos(sel - 1).ruta;

    // Mostrar mapa exacto
    imprimirMapaRuta(rutaElegida, svcRutas);

    // Delegar a reservarVuelo() o reservarRuta()
    if (rutaElegida.vuelos.longitud() == 1) {
        const Vuelo& v = rutaElegida.vuelos.obtenerPos(0);
        reservarVuelo(v.getId());
    }
    else {
        reservarRuta(rutaElegida);
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

void MenuUsuario::opcionHacerCheckIn() {
    string codigo;
    cout << "Ingrese codigo de reserva confirmada: ";
    cin >> codigo;

    if (svcCheckIn.registrarEnCola(codigo)) {
        cout << "Te uniste a la cola de check-in. ¡Gracias!\n";
    }
    else {
        cout << "No se pudo registrar el check-in. "
            "Verifica que la reserva este confirmada y no hayas hecho check-in antes.\n";
    }
}

void MenuUsuario::opcionCerrarSesion() {
    cout << "Cerrando sesion...\n";
}
