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

// RECURSIVA
void cuentaRegresiva(int n) {
	if (n < 1) return;
	cout << n << endl;
	Sleep(1000);
	cuentaRegresiva(n - 1);
}

// RECURSIVA
void MenuUsuario::imprimirReservasRecursivo(const Lista<Reserva>& reservas, int index) {
    if (index >= reservas.longitud()) {
        return;
    }

    const auto& r = reservas.obtenerPos(index);
    Vuelo v;
    svcVuelos.buscarVuelo(r.getVueloId(), v);
    double total = v.getPrecio() * r.getAsientos().longitud();
    cout << r.getCodigo() << " | "
        << r.getVueloId() << " | "
        << r.getFecha() << " | "
        << r.getAsientos().toPrint(" ") << " | "
        << (r.getEstado() == EstadoReserva::CONFIRMADA ? "CONF" :
            r.getEstado() == EstadoReserva::PENDIENTE ? "PEND" : "CANC")
        << " | S/ " << fixed << setprecision(2) << total << "\n";

    imprimirReservasRecursivo(reservas, index + 1);
}

// RECURSIVA
int MenuUsuario::contarConfirmadasRec(const Lista<Reserva>& lista, int i) {
    if (i >= lista.longitud()) return 0;
    int suma = (lista.obtenerPos(i).getEstado() == EstadoReserva::CONFIRMADA) ? 1 : 0;
    return suma + contarConfirmadasRec(lista, i + 1);
}

// RECURSIVA
double MenuUsuario::imprimirHistorialRec(const Lista<Reserva>& reservas, int i) {
    if (i >= reservas.longitud()) return 0;

    const Reserva& r = reservas.obtenerPos(i);
    Vuelo v; svcVuelos.buscarVuelo(r.getVueloId(), v);
    double total = v.getPrecio() * r.getAsientos().longitud();

    cout << r.getCodigo() << " | "
        << r.getFecha() << " | "
        << r.getVueloId() << " | "
        << r.getAsientos().toPrint(",") << " | "
        << (r.getEstado() == EstadoReserva::CONFIRMADA ? "CONF" :
            r.getEstado() == EstadoReserva::PENDIENTE ? "PEND" : "CANC")
        << " | " << fixed << setprecision(2) << total << '\n';

    return total + imprimirHistorialRec(reservas, i + 1);
}

void MenuUsuario::opcionBuscarYReservar() {
        auto todos = svcVuelos.listarVuelos();

        if (todos.esVacia()) {
            cout << "No hay vuelos disponibles.\n";
            return;
        }

        // Ordenar por precio (burbuja)
        insertionSortPorPrecioAsc(todos);

        // Mostrar vuelos disponibles
        cout << "\nVUELOS DISPONIBLES\n";
        cout << "ID | Origen | Destino | Fecha       | Precio\n";
        for (int i = 0; i < todos.longitud(); ++i) {
            const Vuelo& v = todos.obtenerPos(i);
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

        // Buscar el vuelo para obtener su fecha (ya que antes la recibías desde el input)
        Vuelo v;
        if (!svcVuelos.buscarVuelo(id, v)) {
            cout << "Vuelo no encontrado.\n";
            return;
        }

        reservarVuelo(id, v.getFecha());
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
    string filaAnt;
    for (int i = 0; i < libres.longitud(); ++i) {
        string cod = libres.obtenerPos(i).getCodigo();
        string fila;
        for (char c : cod) if (isdigit(c)) fila += c; else break;
        if (!filaAnt.empty() && fila != filaAnt) cout << '\n';
        cout << cod << ' ';
        filaAnt = fila;
    }
    cout << "\n";

    cout << "\nCantidad a reservar: ";
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

    ostringstream oss; oss << "R-" << time(nullptr);
    string codigoRes = oss.str();
    Reserva res(codigoRes,
        sesion.getUsuarioActual().getCorreo(),
        id, fecha, seleccion);
    if (!svcReservas.crearReserva(res)) {
        cout << "No fue posible completar la reserva (duplicada?).\n";
        return;
    }

    cout << "\nMetodo de pago:\n"
        << "1. Tarjeta de credito\n"
        << "2. Tarjeta de debito\n"
        << "3. Yape/Plin\n"
        << "Opcion: ";
    int metodoOp; cin >> metodoOp; cin.ignore(10000, '\n');
    string metodo = (metodoOp == 1 ? "TC" : metodoOp == 2 ? "TD" : "YP");

    double total = vuelo.getPrecio() * seleccion.longitud();
    string fh = fecha;

    // 3) Procesar pago y actualizar reserva
    cout << "Procesando pago...\n";
    cuentaRegresiva(3);
    // Se elimina la llamada a cuentaRegresiva
    Pago pago(codigoRes, total, metodo, "COMPLETADO", fh);
    bool exito = svcPagos.procesarPago(pago);

    if (exito) {
        cout << "¡Reserva y pago confirmados!\n"
            << "Codigo reserva: " << codigoRes << "\n";
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
    int opc;
    cin >> opc;
    cin.ignore(10000, '\n');
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

    auto lista = svcReservas.listarReservasUsuario(
        sesion.getUsuarioActual().getCorreo());
    int activas = contarConfirmadasRec(lista, 0);
    cout << "Reservas confirmadas: " << activas << '\n';

    cout << "\n¿Deseas actualizar tu perfil?\n";
    cout << "1. Si\n2. No\nOpcion: ";
    int opc; cin >> opc; cin.ignore(10000, '\n');

    if (opc != 1) return;

    string nuevoNombre, nuevoApellido, nuevaContrasena;
    cout << "Nuevo nombre: ";      getline(cin, nuevoNombre);
    cout << "Nuevo apellido: ";    getline(cin, nuevoApellido);
    cout << "Nueva contrasena: ";  getline(cin, nuevaContrasena);

    Usuario& u = static_cast<Usuario&>(sesion.getUsuarioActual());
    u.setNombre(nuevoNombre);
    u.setApellido(nuevoApellido);
    u.setPassword(nuevaContrasena);

    AuthService auth;
    auth.getRepoUsuarios().actualizar(u);

    cout << "Perfil actualizado correctamente.\n";
}


void MenuUsuario::opcionReportes() {
    int opc;
    cout << "\n--- REPORTES ---\n"
        << "1. Historial de reservas\n"
        << "2. Gasto por metodo de pago\n"
        << "3. Reservas canceladas\n"
        << "4. Vuelos mas economicos\n"
        << "5. Frecuencia de destinos\n"
        << "6. Volver\n"
        << "Opcion: ";
    cin >> opc; cin.ignore(10000, '\n');

    switch (opc) {
    case 1: reporteHistorialReservas();                 break;
    case 2: reporteGastoPorMetodo();                    break;
    case 3: {
        int d; cout << "¿ultimos cuantos dias?: ";
        cin >> d; cin.ignore(10000, '\n');
        reporteReservasCanceladas(d);
        break;
    }
    case 4: {
        int k; cout << "¿Top cuantos vuelos?: ";
        cin >> k; cin.ignore(10000, '\n');
        reporteVuelosBaratos(k);
        break;
    }
    case 5: reporteFrecuenciaDestinos();                break;
    default: break;  // volver
    }
}

void MenuUsuario::reporteHistorialReservas() {
    auto reservas = svcReservas.listarReservasUsuario(
        sesion.getUsuarioActual().getCorreo());

    if (reservas.esVacia()) {
        cout << "\nNo hay reservas registradas.\n";
        return;
    }
    cout << "\nHISTORIAL COMPLETO\n"
        << "Codigo | Fecha | Vuelo | Asientos | Estado | S/ Total\n";

    double totalGlobal = imprimirHistorialRec(reservas, 0);

    cout << "-------------------------------------------\n"
        << "Gasto acumulado: S/ " << fixed << setprecision(2) << totalGlobal << "\n";
}

struct MetodoSum { string metodo; double suma; };

void acumularMetodo(Lista<MetodoSum>& lst, const string& metodo, double monto) {
	for (int i = 0; i < lst.longitud(); ++i) {
		auto ms = lst.obtenerPos(i);
		if (ms.metodo == metodo) {
			ms.suma += monto;
			lst.modificarPos(ms, i);
			return;
		}
	}
	lst.agregaFinal({ metodo, monto });
}

void MenuUsuario::reporteGastoPorMetodo() {
	auto pagos = svcPagos.listarPagosUsuario(
		sesion.getUsuarioActual().getCorreo());

	if (pagos.esVacia()) {
		cout << "\nNo hay pagos registrados.\n";
		return;
	}

	Lista<MetodoSum> sumas;

    // LAMBDA
	auto acumular = [](Lista<MetodoSum>& lst, const string& metodo, double monto) {
		for (int i = 0; i < lst.longitud(); ++i) {
			auto ms = lst.obtenerPos(i);
			if (ms.metodo == metodo) {
				ms.suma += monto;
				lst.modificarPos(ms, i);
				return;
			}
		}
		lst.agregaFinal({ metodo, monto });
		};

	for (int i = 0; i < pagos.longitud(); ++i) {
		const Pago& p = pagos.obtenerPos(i);
		acumular(sumas, p.getMetodo(), p.getMonto());
	}

	auto obtenerEtiquetaMetodo = [](const string& metodo) {
		if (metodo == "TC") return "Tarjeta Credito";
		if (metodo == "TD") return "Tarjeta Debito";
		return "Yape/Plin";
		};

	cout << "\nGASTO POR MeTODO DE PAGO\n";
	double totalGeneral = 0;
	for (int i = 0; i < sumas.longitud(); ++i) {
		MetodoSum ms = sumas.obtenerPos(i);
		string etiqueta = obtenerEtiquetaMetodo(ms.metodo);
		cout << setw(16) << left << etiqueta << ": S/ " << fixed << setprecision(2) << ms.suma << '\n';
		totalGeneral += ms.suma;
	}

	cout << "Total general: S/ " << fixed << setprecision(2) << totalGeneral << "\n";
}

static time_t parseFecha(const string& ddmmyyyy) {
	tm t{}; t.tm_isdst = -1;
	sscanf_s(ddmmyyyy.c_str(), "%d/%d/%d", &t.tm_mday, &t.tm_mon, &t.tm_year);
	t.tm_mon -= 1;
	t.tm_year -= 1900;
	return mktime(&t);
}

void MenuUsuario::reporteReservasCanceladas(int dias) {
    auto reservas = svcReservas.listarReservasUsuario(
        sesion.getUsuarioActual().getCorreo());

    if (reservas.esVacia()) {
        cout << "\nNo hay reservas.\n";
        return;
    }

    // LAMBDA
    auto fechaADia = [](const string& fecha) -> int {
        int d, m, a;
        sscanf_s(fecha.c_str(), "%d/%d/%d", &d, &m, &a);
        return a * 365 + m * 30 + d;
        };

    int maxDia = 0;
    for (int i = 0; i < reservas.longitud(); ++i) {
        const Reserva& r = reservas.obtenerPos(i);
        int dia = fechaADia(r.getFecha());
        if (dia > maxDia) maxDia = dia;
    }

    // LAMBDA
    auto fueCanceladaRecientemente = [&](const Reserva& r) {
        if (r.getEstado() != EstadoReserva::CANCELADA) return false;
        int dia = fechaADia(r.getFecha());
        return (maxDia - dia) <= dias;
        };

    int cont = 0;
    cout << "\nRESERVAS CANCELADAS (ult " << dias << " dias)\n";
    for (int i = 0; i < reservas.longitud(); ++i) {
        const Reserva& r = reservas.obtenerPos(i);
        if (fueCanceladaRecientemente(r)) {
            cout << r.getCodigo() << " | " << r.getFecha() << " | Vuelo " << r.getVueloId() << '\n';
            ++cont;
        }
    }

    if (!cont) cout << "No se encontraron.\n";
}

void MenuUsuario::reporteVuelosBaratos(int k) {
    auto reservas = svcReservas.listarReservasUsuario(
        sesion.getUsuarioActual().getCorreo());

    Lista<PrecioReserva> precios;
    for (int i = 0; i < reservas.longitud(); ++i) {
        const Reserva& r = reservas.obtenerPos(i);
        if (r.getEstado() != EstadoReserva::CONFIRMADA) continue;
        Vuelo v;  svcVuelos.buscarVuelo(r.getVueloId(), v);
        precios.agregaFinal({ v.getPrecio(), r });
    }

    if (precios.esVacia()) {
        cout << "\nNo hay reservas confirmadas.\n";
        return;
    }

    insertionSortPorPrecioAsc(precios);                    // ↑ precio

    cout << "\nTOP " << k << " VUELOS MAS BARATOS\n";
    for (int i = 0; i < precios.longitud() && i < k; ++i) {
        const PrecioReserva& pr = precios.obtenerPos(i);
        Vuelo v;  svcVuelos.buscarVuelo(pr.res.getVueloId(), v);
        cout << i + 1 << ") " << v.getOrigen() << "-" << v.getDestino()
            << " (" << pr.res.getFecha() << ") - S/ "
            << fixed << setprecision(2) << pr.precio << '\n';
    }
}

struct DestinoCount { string destino; int cant; };

void incrementarDestino(Lista<DestinoCount>& l, const string& dest) {
	for (int i = 0; i < l.longitud(); ++i) {
		auto dc = l.obtenerPos(i);
		if (dc.destino == dest) {
			dc.cant++;
			l.modificarPos(dc, i);
			return;
		}
	}
	l.agregaFinal({ dest,1 });
}

void MenuUsuario::reporteFrecuenciaDestinos() {
	auto reservas = svcReservas.listarReservasUsuario(
		sesion.getUsuarioActual().getCorreo());

    // LAMBDA
	auto esConfirmada = [](const Reserva& r) {
		return r.getEstado() == EstadoReserva::CONFIRMADA;
		};

    //LAMBDA
	auto incrementarDestino = [](Lista<DestinoCount>& lista, const string& destino) {
		for (int i = 0; i < lista.longitud(); ++i) {
			auto dc = lista.obtenerPos(i);
			if (dc.destino == destino) {
				dc.cant++;
				lista.modificarPos(dc, i);
				return;
			}
		}
		lista.agregaFinal({ destino, 1 });
		};

	Lista<DestinoCount> freq;
	for (int i = 0; i < reservas.longitud(); ++i) {
		const Reserva& r = reservas.obtenerPos(i);
		if (!esConfirmada(r)) continue;

		Vuelo v;
		svcVuelos.buscarVuelo(r.getVueloId(), v);
		incrementarDestino(freq, v.getDestino());
	}

	if (freq.esVacia()) {
		cout << "\nAun no tienes destinos confirmados.\n";
		return;
	}

	cout << "\nFRECUENCIA DE DESTINOS\n";
	for (int i = 0; i < freq.longitud(); ++i) {
		auto dc = freq.obtenerPos(i);
		cout << setw(10) << left << dc.destino << " : " << dc.cant << " vuelo(s)\n";
	}
}

void MenuUsuario::opcionCerrarSesion() {
	cout << "Cerrando sesion...\n";
}
