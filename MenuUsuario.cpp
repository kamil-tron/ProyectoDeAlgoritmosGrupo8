﻿#include "MenuUsuario.h"
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
	int Y = 10;

	cursor(60, Y); Y++;
	cout << "Metodo de pago:"; cursor(60, Y); Y++;
	cout << "1. Tarjeta de credito"; cursor(60, Y); Y++;
	cout << "2. Tarjeta de debito"; cursor(60, Y); Y++;
	cout << "3. Yape / Plin"; cursor(60, Y); Y++;
	cout << "Opcion: ";

	int op; cin >> op; cin.ignore(10000, '\n');
	return (op == 1 ? "TC" : op == 2 ? "TD" : "YP");

}



static void imprimirMapaRuta(const RutaPosible& ruta, ServicioRutas& svcRutas)
{
	int backup[50][101];
	for (int f = 0; f < 50; ++f)
		for (int c = 0; c < 101; ++c)
			backup[f][c] = matrizPeru[f][c];
	system("cls");
	svcRutas.pintarRutaEnMatriz(ruta,4,9,5);
	cursor(0, 50);
	cout << "\n==== MAPA DE LA RUTA ====\n";
	imprimirMapaPeru(25, 0);
	cout << "\nPresiona ENTER para continuar...";
	cin.ignore(10000, '\n');
	cin.get();

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

	imprimirReservasRecursivo(reservas, index + 1, Y);
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

	int Y = 5;
	cursor(60, Y); Y++;
	cout << "--- BUSCAR VUELOS O RUTAS ---";

	auto aeropuertos = svcVuelos.listarAeropuertos();
	if (aeropuertos.esVacia()) {
		cursor(60, Y); Y++;
		cout << "No hay aeropuertos registrados.";
		return;
	}
	cursor(60, Y); Y++;
	cout << "AEROPUERTOS DISPONIBLES:";
	for (int i = 0; i < aeropuertos.longitud(); ++i) {
		const Aeropuerto& a = aeropuertos.obtenerPos(i);
		cursor(60, Y); Y++;
		cout << "- " << a.getCodigo() << "  (X: " << a.getX() << ", Y: " << a.getY() << ")";
	}


	string origen, destino; cursor(60, Y); Y++;
	cout << "Ingrese el codigo IATA de origen  : "; cin >> origen; cursor(60, Y); Y++;
	cout << "Ingrese el codigo IATA de destino : "; cin >> destino;


	cursor(60, Y); Y++;
	cout << "Ordenar las rutas con escalas por:"; cursor(60, Y); Y++;
	cout << "  1. Costo total (mas baratas primero)"; cursor(60, Y); Y++;
	cout << "  2. Distancia total (mas cortas primero)"; cursor(60, Y); Y++;
	cout << "Opcion: ";
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
		cursor(60, Y); Y++;
		cout << "No se encontraron vuelos ni rutas entre los aeropuertos.";
		return;
	}
	cursor(60, Y); Y++;
	cout << "Opciones disponibles:";

	struct OpcionRuta { bool esDirecto; int index; };
	Lista<OpcionRuta> opciones;
	int opcionActual = 1;

	if (!vuelosDirectos.esVacia()) {
		cursor(60, Y); Y++;
		cout << "VUELOS DIRECTOS:";
		for (int i = 0; i < vuelosDirectos.longitud(); ++i) {
			const Vuelo& v = vuelosDirectos.obtenerPos(i);
			cursor(60, Y); Y++;
			cout << "[" << opcionActual << "] "
				<< v.getOrigen() << "->" << v.getDestino() << ' '
				<< v.getFecha() << "  S/"
				<< fixed << setprecision(2) << v.getPrecio() << '\n';

			opciones.agregaFinal({ true, v.getId() });
			++opcionActual;
		}
	}

	if (!rutas.esVacia()) {
		cursor(60, Y); Y++;
		cout << "RUTAS CON ESCALAS:";
		for (int i = 0; i < rutas.longitud(); ++i) {
			const RutaPosible& r = rutas.obtenerPos(i);
			cursor(60, Y); Y++;

			cout << "[" << opcionActual << "] ";

			cout << r.vuelos.obtenerPos(0).getOrigen();
	
			for (int j = 0; j < r.vuelos.longitud(); ++j) {
				cout << "->" << r.vuelos.obtenerPos(j).getDestino();
			}
			cout << "  "
				<< r.vuelos.longitud() << " tramos  "
				<< r.vuelos.obtenerPos(0).getFecha() << "->"
				<< r.vuelos.obtenerFinal().getFecha()
				<< "  S/" << fixed << setprecision(2)
				<< r.costoTotal
				<< '\n';

			opciones.agregaFinal({ false, i });
			++opcionActual;
		}
	}
	cursor(60, Y); Y++;
	cout << "Seleccione una opcion para reservar (0=Cancelar): ";
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
	int Y = 15;
	HANDLE con = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD BG_GRAY = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
	Usuario& u = static_cast<Usuario&>(sesion.getUsuarioActual());
	system("cls");
	menuUsuario();
	SetConsoleTextAttribute(con, BG_GRAY);
	cursor(6, 12);
	cout << "MENU DE USUARIO - AIR PACIFIC" << endl; cursor(8, 13);
	MenuBase::mostrar();
	string metodo = elegirMetodoPago();
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	WORD defaultAttrs = csbi.wAttributes;

	auto libres = svcReservas.listarAsientosDisponibles(vueloId);
	if (libres.esVacia()) {
		cursor(60, Y); Y++;
		cout << "No hay asientos disponibles en el vuelo " << vueloId << ".\n";
		return;
	}

	cursor(60, Y); Y++;
	cout << "Asientos libres para vuelo " << libres.longitud() << ":\n";   cursor(60, Y); Y++;
	string filaAnt;
	for (int i = 0; i < libres.longitud(); ++i) {
		const Asiento& a = libres.obtenerPos(i);
		string cod = a.getCodigo();

		string fila;

		for (char c : cod) if (isdigit(c)) fila += c; else break;
		if (!filaAnt.empty() && fila != filaAnt) { cursor(60, Y); Y++; }
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
	cursor(60, Y); Y++;
	cout << "Cantidad de asientos a reservar: ";    cursor(60, Y); Y++;
	int cantidad;
	cin >> cantidad;
	cin.ignore(10000, '\n');
	if (cantidad < 1 || cantidad > libres.longitud()) {
		cout << "Cantidad inválida.\n";
		return;
	}

	Lista<string> seleccionados;
	cursor(60, Y); Y++;
	cout << "Ingrese los codigos de asiento separados por espacio: ";
	for (int i = 0; i < cantidad; ++i) {
		string codigo;
		cin >> codigo;
		seleccionados.agregaFinal(codigo);
	}
	cin.ignore(10000, '\n');

	Reserva reserva;
	if (!svcReservas.crearReservaConAsientos(u, vueloId, seleccionados, reserva)) {
		cursor(60, Y); Y++; cout << "No se pudo crear la reserva.";
		return;
	}

	double total = svcReservas.calcularTotal(vueloId, seleccionados);
	Pago pago;
	if (svcPagos.procesarPagoReserva(reserva, total, metodo, pago)) {
		cursor(60, Y); Y++;
		cout << "Reserva confirmada: " << reserva.getCodigo()
			<< " (S/" << fixed << setprecision(2) << total << ")\n";
	}
	else {
		cursor(60, Y); Y++;
		cout << "Pago fallido para la reserva.\n";
	}
	_getch();
}

void MenuUsuario::reservarRuta(const RutaPosible& ruta) {
	Usuario& u = static_cast<Usuario&>(sesion.getUsuarioActual());
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	WORD defaultAttrs = csbi.wAttributes;
	int Y = 14;
	HANDLE con = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD BG_GRAY = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
	system("cls");
	menuUsuario();
	SetConsoleTextAttribute(con, BG_GRAY);
	cursor(6, 12);
	cout << "MENU DE USUARIO - AIR PACIFIC" << endl; cursor(8, 13);
	MenuBase::mostrar();
	string metodo = elegirMetodoPago();
	double totalRuta = 0.0;
	int exitosas = 0;

	for (int i = 0; i < ruta.vuelos.longitud(); ++i) {
		const Vuelo& v = ruta.vuelos.obtenerPos(i);
		SetConsoleTextAttribute(con, BG_GRAY);
		cursor(60, Y); Y++;
		cout << "--- Vuelo " << v.getId()
			<< " (" << v.getOrigen() << "->" << v.getDestino() << ") ---\n";

		auto libres = svcReservas.listarAsientosDisponibles(v.getId());
		if (libres.esVacia()) {
			cursor(60, Y); Y++;
			cout << "No hay asientos disponibles en este tramo.\n";
			continue;
		}
		cursor(60, Y); Y++;
		cout << "Asientos libres para vuelo " << libres.longitud() << ":\n";   cursor(60, Y); Y++;
		string filaAnt;
		for (int i = 0; i < libres.longitud(); ++i) {
			const Asiento& a = libres.obtenerPos(i);
			string cod = a.getCodigo();
			string fila;
			for (char c : cod) if (isdigit(c)) fila += c; else break;
			if (!filaAnt.empty() && fila != filaAnt) { cursor(60, Y); Y++; }
			if (a.isVip()) {
				SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN);

				cout << cod;
				SetConsoleTextAttribute(hConsole, defaultAttrs);
				SetConsoleTextAttribute(con, BG_GRAY);
				cout << ' ';
			}
			else {
				cout << cod << ' ';
			}
			filaAnt = fila;
		}
		cursor(60, Y); Y++;
		cout << "Cantidad a reservar: ";
		int cnt; cin >> cnt; cin.ignore(10000, '\n');
		if (cnt < 1 || cnt > libres.longitud()) { --i; continue; }

		Lista<string> sel;
		cursor(60, Y); Y++;
		cout << "Ingrese codigos separados por espacio:";
		for (int k = 0; k < cnt; ++k) {
			string code; cin >> code; sel.agregaFinal(code);
		}
		cin.ignore(10000, '\n');

		Reserva nueva;
		if (!svcReservas.crearReservaConAsientos(u, v.getId(), sel, nueva)) {
			cursor(60, Y); Y++;
			cout << "No se pudo reservar estos asientos.";
			continue;
		}

		double totalVuelo = svcReservas.calcularTotal(v.getId(), sel);
		Pago pago;
		if (svcPagos.procesarPagoReserva(nueva, totalVuelo, metodo, pago)) {
			exitosas++;
			totalRuta += totalVuelo;
			cursor(60, Y); Y++;
			cout << "Reserva confirmada: " << nueva.getCodigo()
				<< " (S/" << fixed << setprecision(2) << totalVuelo << ")\n";
		}
		else {
			cursor(60, Y); Y++;
			cout << "Pago fallido para este tramo.\n";
		}
	}

	if (exitosas) {
		cursor(60, Y); Y++; cout << "Ruta reservada exitosamente.";
		cursor(60, Y); Y++; cout << " Total pagado: S/" << fixed << setprecision(2) << totalRuta;
	}
	else {
		cursor(60, Y); Y++; cout << "No se concretaron reservas en la ruta.";
	}
	_getch();
}


void MenuUsuario::opcionVerReservas() {
	int Y = 8;
	int elegir = 0;
	auto reservas = svcReservas.listarReservasUsuario(sesion.getUsuarioActual().getCorreo());
	if (reservas.esVacia()) {
		cursor(70, Y); Y++;
		cout << "No tienes reservas";
		return;
		_getch();
	}
	ArbolB<string> arbolReservas;
	for (int i = 0; i < reservas.longitud(); ++i) {
		arbolReservas.insertar(reservas.obtenerPos(i).getCodigo());
	}
		do {
			cursor(70, Y); Y++;
			cout << "COMO VER SUS RESERVAS";   cursor(70, Y); Y++;
			cout << "[1] precio ascendente";   cursor(70, Y); Y++;
			cout << "[2] precio desendente";   cursor(70, Y); Y++;
			cout << "elija una opcion: "; cin >> elegir;
			if (elegir > 2 || elegir < 1) {
				cursor(70, Y); Y++; cout << "opcion incorrecta";
			}
		} while (elegir > 2 || elegir < 1);

		if (elegir == 1) {
			quickSortReservasPorPrecioAsc(reservas, svcReservas);
		}
		else {
			heapSortDesc(reservas, svcReservas);
		}
		cursor(70, Y++);
		cout << "MIS RESERVAS";
		cursor(70, Y++);
		cout << "Codigo | Vuelo | Fecha | Asientos | Estado | Total";
		imprimirReservasRecursivo(reservas, 0, Y);

		Y += reservas.longitud();
		cursor(70, Y++);
		cout << "Deseas cancelar alguna reserva? (1=Si, 2=No): ";
		int opc; cin >> opc; cin.ignore(10000, '\n');
		if (opc != 1) return;

		cursor(70, Y++);
		cout << "Codigo de la reserva a cancelar: ";
		string codigo;
		getline(cin, codigo);

		// 3) Busca en O(log n) antes de cancelar
		if (!arbolReservas.buscar(codigo)) {
			cursor(70, Y++);
			cout << "Ese código no existe en tus reservas.\n";
		}
		else if (svcReservas.cancelarReserva(codigo)) {
			cursor(70, Y++);
			cout << "Reserva cancelada.\n";
		}
		else {
			cursor(70, Y++);
			cout << "No se pudo cancelar la reserva (quizá ya estaba cancelada).\n";
		}

		_getch();
	}

	void MenuUsuario::opcionVerPerfil() {
		int Y = 12;
		cursor(70, Y); Y += 4;
		cout << "MI PERFIL";
		sesion.getUsuarioActual().mostrarPerfil();

		auto lista = svcReservas.listarReservasUsuario(sesion.getUsuarioActual().getCorreo());
		int activas = contarConfirmadasRec(lista, 0);
		cursor(70, Y); Y++;
		cout << "Reservas confirmadas: " << activas;
		cursor(70, Y); Y++;
		cout << "Deseas actualizar tu perfil?"; cursor(70, Y); Y++;
		cout << "1. Si"; cursor(70, Y); Y++;
		cout << "2. No"; cursor(70, Y); Y++;
		cout << "Opcion: ";
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
		int Y = 13;
		auto reservas = svcReservas.listarReservasUsuario(sesion.getUsuarioActual().getCorreo());
		string codigo;
		cursor(70, Y); Y++;
		cout << "MIS RESERVAS";
		cursor(70, Y); Y++;
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