#include "ServicioReservas.h"

ServicioReservas::ServicioReservas() = default;

Lista<Reserva> ServicioReservas::listarReservasUsuario(const string& email) const {
    auto todas = repoReservas.cargarTodos();
    Lista<Reserva> result;
    for (int i = 0; i < todas.longitud(); ++i) {
        auto r = todas.obtenerPos(i);
        if (r.getUserEmail() == email && !r.isCancelada()) {
            result.agregaFinal(r);
        }
    }
    return result;
}

bool ServicioReservas::crearReserva(const Reserva& r) {
    Vuelo v;
    if (!repoVuelos.buscarPorId(r.getVueloId(), v)) return false;
    if (v.getAsientosDisponibles() < r.getCantidadAsientos()) return false;
    v.setAsientosDisponibles(v.getAsientosDisponibles() - r.getCantidadAsientos());
    repoVuelos.actualizar(v);
    repoReservas.agregar(r);
    return true;
}

bool ServicioReservas::cancelarReserva(const string& codigo) {
    Reserva r;
    if (!repoReservas.buscarPorCodigo(codigo, r) || r.isCancelada()) return false;
    r.cancelar();
    repoReservas.actualizar(r);
    Vuelo v;
    if (repoVuelos.buscarPorId(r.getVueloId(), v)) {
        v.setAsientosDisponibles(v.getAsientosDisponibles() + r.getCantidadAsientos());
        repoVuelos.actualizar(v);
    }
    return true;
}