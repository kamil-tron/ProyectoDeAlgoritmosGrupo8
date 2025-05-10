#include "MenuUsuario.h"
#include "Reserva.h"
#include "Pago.h"

void MenuUsuario::opcionBuscarYReservar() {
    string origen, destino, fecha;
    cout << "\nBUSCAR VUELOS\n"
        << "Origen: ";   getline(cin, origen);
    cout << "Destino: ";  getline(cin, destino);
    cout << "Fecha (dd/mm/aaaa): "; getline(cin, fecha);

    auto todos = svcVuelos.listarVuelos();
    Lista<Vuelo> filtrados;

    for (int i = 0; i < todos.longitud(); ++i) {
        const Vuelo& v = todos.obtenerPos(i);
        if (v.getOrigen() == origen && v.getDestino() == destino && v.getFecha() == fecha) {
            filtrados.agregaFinal(v);
        }
    }

    if (filtrados.esVacia()) {
        cout << "No hay vuelos para esa ruta/fecha.\n";
        return;
    }

    for (int i = 0; i < filtrados.longitud(); ++i)
        for (int j = 0; j < filtrados.longitud() - 1; ++j) {
            auto a = filtrados.obtenerPos(j);
            auto b = filtrados.obtenerPos(j + 1);
            if (a.getPrecio() > b.getPrecio()) {
                filtrados.modificarPos(b, j);
                filtrados.modificarPos(a, j + 1);
            }
        }

    cout << "\nID | Origen | Destino | Fecha       | Precio\n";
    for (int i = 0; i < filtrados.longitud(); ++i) {
        const Vuelo& v = filtrados.obtenerPos(i);
        cout << setw(2) << v.getId() << " | "
            << setw(6) << v.getOrigen() << " | "
            << setw(7) << v.getDestino() << " | "
            << setw(11) << v.getFecha() << " | S/ "
            << fixed << setprecision(2) << v.getPrecio() << '\n';
    }

    cout << "\nDesea reservar alguno de estos vuelos? (1=Si, 2=No): ";
    int opc; cin >> opc;
    cin.ignore(10000, '\n');
    if (opc != 1) return;

    cout << "Seleccione ID del vuelo: ";
    int id; cin >> id;
    cin.ignore(10000, '\n');
    reservarVuelo(id, fecha);
}

void MenuUsuario::reservarVuelo(int id, const string& fecha) {
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

    cout << "\nAsientos libres:\n";
    string filaAnt = "";
    for (int i = 0; i < libres.longitud(); ++i) {
        string cod = libres.obtenerPos(i).getCodigo();
        string fila = "";
        for (char c : cod)
            if (isdigit(c)) fila += c; else break;
        if (fila != filaAnt && i != 0) cout << '\n';
        cout << cod << ' ';
        filaAnt = fila;
    }
    cout << '\n';

    cout << "\n\nCantidad a reservar: ";
    int cant; cin >> cant;
    cin.ignore(10000, '\n');

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

    cout << "Procesando pago...\n";

    ostringstream oss; oss << "R-" << time(nullptr);
    string codigo = oss.str();

    Reserva res(codigo,
        sesion.getUsuarioActual().getCorreo(),
        id, fecha, seleccion);

    if (!svcReservas.crearReserva(res)) {
        cout << "No fue posible completar la reserva (verifique asientos).\n";
        return;
    }

    cout << "Reserva exitosa. Codigo: " << codigo << '\n';
}

void MenuUsuario::opcionVerReservas() {
    auto reservas = svcReservas.listarReservasUsuario(
        sesion.getUsuarioActual().getCorreo());

    if (reservas.esVacia()) {
        cout << "\nNo tienes reservas activas.\n";
        return;
    }

    cout << "\nMIS RESERVAS\n"
        << "Codigo | Vuelo | Fecha | Asientos | Total\n";

    for (int i = 0; i < reservas.longitud(); ++i) {
        const auto& r = reservas.obtenerPos(i);
        Vuelo v; svcVuelos.buscarVuelo(r.getVueloId(), v);
        double total = v.getPrecio() * r.getAsientos().longitud();
        cout << r.getCodigo() << " | "
            << r.getVueloId() << " | "
            << r.getFecha() << " | "
            << r.getAsientos().toPrint(" ") << " | S/ "
            << fixed << setprecision(2) << total << '\n';
    }

    cout << "\nDeseas cancelar alguna reserva? (1=Si, 2=No): ";
    int opc; cin >> opc;
    cin.ignore(10000, '\n');
    if (opc != 1) return;

    cout << "Codigo de la reserva a cancelar: ";
    string codigo; getline(cin, codigo);
    if (svcReservas.cancelarReserva(codigo))
        cout << "Reserva cancelada.\n";
    else
        cout << "No se encontro o ya estaba cancelada.\n";
}

void MenuUsuario::opcionVerPerfil() {
    cout << "\nMI PERFIL\n";
    sesion.getUsuarioActual().mostrarPerfil();

    auto lista = svcReservas.listarReservasUsuario(
        sesion.getUsuarioActual().getCorreo());
    int activas = 0;
    for (int i = 0; i < lista.longitud(); ++i)
        if (!lista.obtenerPos(i).isCancelada()) ++activas;

    cout << "Reservas activas: " << activas << "\n";

    cout << "Desea actualizar datos? (1=Si, 2=No): ";
    int opc; cin >> opc;
    cin.ignore(10000, '\n');
    if (opc == 1) cout << "Funcionalidad de actualizacion pendiente.\n";
}

void MenuUsuario::opcionCerrarSesion() {
    cout << "Cerrando sesion...\n";
}
