#include "MenuUsuario.h"
#include "Reserva.h"
#include "Pago.h"
#include <sstream>
#include <ctime>
#include <Windows.h>
#include <iomanip>
using namespace std;

// 1. Buscar vuelos
void MenuUsuario::opcionBuscarVuelo() {
	string origen, destino, fecha;
	cout << "\nBUSCAR VUELOS\n"
		<< "Origen: "; getline(cin, origen);
	cout << "Destino: "; getline(cin, destino);
	cout << "Fecha (dd/mm/aaaa): "; getline(cin, fecha);
	cout << "Ordenando resultados por precio...\n";

	auto todos = svcVuelos.listarVuelos();
	Lista<Vuelo> filtrados;

	for (int i = 0; i < todos.longitud(); ++i) {
		auto v = todos.obtenerPos(i);
		if (v.getOrigen() == origen && v.getDestino() == destino && v.getFecha() == fecha) {
			filtrados.agregaFinal(v);
		}
	}

	if (filtrados.esVacia()) {
		cout << "No hay vuelos para esa ruta/fecha.\n";
		return;
	}

	for (int i = 0; i < filtrados.longitud(); ++i) {
		for (int j = 0; j < filtrados.longitud() - 1; ++j) {
			Vuelo a = filtrados.obtenerPos(j);
			Vuelo b = filtrados.obtenerPos(j + 1);
			if (a.getPrecio() > b.getPrecio()) {
				filtrados.modificarPos(b, j);
				filtrados.modificarPos(a, j + 1);
			}
		}
	}

	cout << "Resultados encontrados:\n"
		<< "ID   | Origen | Destino | Fecha       | Precio\n"
		<< "-----|--------|---------|-------------|--------\n";

	for (int i = 0; i < filtrados.longitud(); ++i) {
		auto v = filtrados.obtenerPos(i);
		cout << setw(4) << v.getId() << " | "
			<< setw(6) << v.getOrigen() << " | "
			<< setw(7) << v.getDestino() << " | "
			<< setw(11) << v.getFecha() << " | S/ "
			<< fixed << setprecision(2) << v.getPrecio() << "\n";
	}

	cout << "\nSeleccione ID del vuelo: ";
	int id; cin >> id;
	cin.ignore(10000, '\n');
	opcionReservarVuelo(id, fecha);
}

void MenuUsuario::opcionReservarVuelo() {
	cout << "\nRESERVAR VUELO\n";
	int id;
	cout << "Ingrese ID de vuelo a reservar: "; cin >> id;
	cin.ignore(10000, '\n');
	string fecha;
	cout << "Fecha (dd/mm/aaaa): "; getline(cin, fecha);
	opcionReservarVuelo(id, fecha);
}
void cuentaRegresiva(int n) {
	if (n < 1) return;

	std::cout << n << std::endl;
	Sleep(1000);

	cuentaRegresiva(n - 1);
}

// 2b. Reservar vuelo (con ID y fecha)
void MenuUsuario::opcionReservarVuelo(int id, const string& fecha) {
	int cantidad;
	cout << "Numero de asientos a reservar: ";
	cin >> cantidad;
	cin.ignore(10000, '\n');

	Vuelo vuelo;
	if (!svcVuelos.buscarVuelo(id, vuelo)) {
		cout << "Vuelo no encontrado.\n";
		return;
	}

	if (cantidad > vuelo.getAsientosDisponibles()) {
		cout << "No hay suficientes asientos disponibles.\n";
		return;
	}

	cout << "Metodo de pago:\n"
		<< "1. Tarjeta de credito\n"
		<< "2. Tarjeta de debito\n"
		<< "3. Yape/Plin\n"
		<< "Opcion: ";
	int metodo; cin >> metodo;
	cin.ignore(10000, '\n');

	if (metodo == 1 || metodo == 2) {
		string tarjeta, cvv;
		cout << "Nro tarjeta: "; getline(cin, tarjeta);
		cout << "CVV: "; getline(cin, cvv);
	}
	else {
		cout << "Se abrira la app Yape/Plin...\n";
	}

	cout << "Procesando pago...\n";
	cuentaRegresiva(5);
	ostringstream code;
	code << "R-" << time(nullptr);
	string codigo = code.str();

	Reserva r(codigo, sesion.getUsuarioActual().getCorreo(), id, fecha, cantidad);
	svcReservas.crearReserva(r);

	vuelo.setAsientosDisponibles(vuelo.getAsientosDisponibles() - cantidad);
	svcVuelos.modificarVuelo(vuelo);

	double total = vuelo.getPrecio() * cantidad;
	Pago p(r.getVueloId(), total, "COMPLETADO", fecha);
	svcPagos.procesarPago(p);

	cout << "Reserva exitosa!\n"
		<< "Codigo de reserva: " << codigo << "\n";
}

// 3. Ver mis reservas
void MenuUsuario::opcionVerReservas() {
	auto lista = svcReservas.listarReservasUsuario(sesion.getUsuarioActual().getCorreo());
	cout << "\nMIS RESERVAS\n"
		<< "Codigo   | Vuelo ID | Fecha       | Asientos | Precio Total\n"
		<< "-------- | -------- | ----------- | -------- | ------------\n";

	for (int i = 0; i < lista.longitud(); ++i) {
		auto r = lista.obtenerPos(i);
		Vuelo v;
		svcVuelos.buscarVuelo(r.getVueloId(), v);
		double total = v.getPrecio() * r.getCantidadAsientos();
		cout << setw(8) << r.getCodigo() << " | "
			<< setw(8) << r.getVueloId() << " | "
			<< setw(11) << r.getFecha() << " | "
			<< setw(8) << r.getCantidadAsientos() << " | S/ "
			<< fixed << setprecision(2) << total << "\n";
	}
}

// 4. Cancelar reserva
void MenuUsuario::opcionCancelarReserva() {
	cout << "\nCANCELAR RESERVA\n"
		<< "Ingrese codigo de reserva a cancelar: ";
	string codigo;
	getline(cin, codigo);

	cout << "Seguro que desea cancelar? (s/n): ";
	char resp; cin >> resp;
	cin.ignore(10000, '\n');

	if (resp == 's' || resp == 'S') {
		if (svcReservas.cancelarReserva(codigo)) {
			cout << "Reserva cancelada correctamente.\n";
		}
		else {
			cout << "No se encontro la reserva o ya estaba cancelada.\n";
		}
	}
}

void MenuUsuario::opcionVerPerfil() {
	cout << "\nMI PERFIL\n";
	sesion.getUsuarioActual().mostrarPerfil();

	auto lista = svcReservas.listarReservasUsuario(sesion.getUsuarioActual().getCorreo());
	int activas = 0;
	for (int i = 0; i < lista.longitud(); ++i) {
		if (!lista.obtenerPos(i).isCancelada()) ++activas;
	}

	cout << "Reservas activas: " << activas << "\n"
		<< "Desea actualizar datos? (1=Si, 2=No): ";
	int opc; cin >> opc;
	cin.ignore(10000, '\n');

	if (opc == 1) {
		cout << "Funcionalidad de actualizacion pendiente.\n";
	}
}

void MenuUsuario::opcionCerrarSesion() {
	cout << "Cerrando sesion...\n";
}