#include "ServicioReservas.h"
#include <sstream>
#include <ctime>

ServicioReservas::ServicioReservas()
	: idx(new HashTable<string, Reserva>(4000, hashString)) {
	cargarIndice();
}
ServicioReservas::~ServicioReservas() { delete idx; }
Lista<Reserva> ServicioReservas::listarTodasReservas() const {
	auto todas = repoReservas.cargarTodos();
	Lista<Reserva> result;
	for (int i = 0; i < todas.longitud(); ++i) {
		const Reserva& r = todas.obtenerPos(i);
		if (r.getEstado() != EstadoReserva::CANCELADA)
			result.agregaFinal(r);
	}
	return result;
}
void ServicioReservas::cargarIndice() {
	auto lista = repoReservas.cargarTodos();
	for (int i = 0; i < lista.longitud(); ++i)
		idx->insertar(lista.obtenerPos(i).getCodigo(), lista.obtenerPos(i));
}

Reserva* ServicioReservas::obtener(const string& codigo) {
	return idx->obtener(codigo);
}
const Reserva* ServicioReservas::obtener(const string& codigo) const {
	return idx->obtener(codigo);
}

Lista<Reserva> ServicioReservas::listarReservasUsuario(const string& email) const {
	auto todas = repoReservas.cargarTodos();
	Lista<Reserva> result;
	for (int i = 0; i < todas.longitud(); ++i) {
		const Reserva& r = todas.obtenerPos(i);
		if (r.getUserEmail() == email && r.getEstado() != EstadoReserva::CANCELADA)
			result.agregaFinal(r);
	}
	return result;
}

Lista<Asiento> ServicioReservas::listarAsientosDisponibles(int vueloId) const {
	Lista<Asiento> libres;
	svcAsientos.listarDisponibles(vueloId, libres);
	return libres;
}

bool ServicioReservas::crearReserva(const Reserva& r) {
	Vuelo v;
	if (!repoVuelos.buscarPorId(r.getVueloId(), v)) return false;
	if (!validarAsientosDisponibles(r.getVueloId(), r.getAsientos())) return false;
	if (!svcAsientos.ocupar(r.getVueloId(), r.getAsientos())) return false;

	repoReservas.crearReserva(r);
	idx->insertar(r.getCodigo(), r);
	return true;
}

bool ServicioReservas::cancelarReserva(const string& codigo) {
	Reserva* r = obtener(codigo);
	if (!r || r->getEstado() == EstadoReserva::CANCELADA) return false;

	svcAsientos.liberar(r->getVueloId(), r->getAsientos());
	r->cancelar();
	repoReservas.actualizar(*r);
	return true;
}

string ServicioReservas::generarCodigo() const {
	ostringstream oss;
	oss << "R-" << time(nullptr);
	return oss.str();
}

bool ServicioReservas::validarAsientosDisponibles(int vueloId,
	const Lista<string>& codigos) const {
	double dummy;
	for (int i = 0; i < codigos.longitud(); ++i)
		if (!svcAsientos.precioAsiento(vueloId, codigos.obtenerPos(i), dummy))
			return false;
	return true;
}

double ServicioReservas::calcularTotal(int vueloId,
	const Lista<string>& codigos) const {
	return svcAsientos.totalSeleccion(vueloId, codigos);
}

bool ServicioReservas::crearReservaConAsientos(const Usuario& usuario,
	int vueloId,
	const Lista<string>& codigos,
	Reserva& outReserva) {
	if (!validarAsientosDisponibles(vueloId, codigos)) return false;

	Vuelo v;
	if (!repoVuelos.buscarPorId(vueloId, v)) return false;

	string codigo = generarCodigo();
	Reserva nueva(codigo, usuario.getCorreo(), vueloId, v.getFecha(), codigos);

	if (!crearReserva(nueva)) return false;
	outReserva = nueva;
	return true;
}
