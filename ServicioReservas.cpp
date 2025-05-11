#include "ServicioReservas.h"
#include "Asiento.h"
#include "Reserva.h"
#include <string>

Lista<Reserva> ServicioReservas::listarReservasUsuario(const string& email) const {
    auto todas = repoReservas.cargarTodos();
    Lista<Reserva> result;
    for (int i = 0; i < todas.longitud(); ++i) {
        const Reserva& r = todas.obtenerPos(i);
        if (r.getUserEmail() == email &&
            r.getEstado() != EstadoReserva::CANCELADA) {
            result.agregaFinal(r);
        }
    }
    return result;
}

Lista<Asiento> ServicioReservas::listarAsientosDisponibles(int vueloId) const {
    auto todos = repoAsientos.listarPorVuelo(vueloId);
    Lista<Asiento> libres;
    for (int i = 0; i < todos.longitud(); ++i) {
        const Asiento& a = todos.obtenerPos(i);
        if (!a.isOcupado()) {
            libres.agregaFinal(a);
        }
    }
    return libres;
}

bool ServicioReservas::crearReserva(const Reserva& r) {
    Vuelo v;
    if (!repoVuelos.buscarPorId(r.getVueloId(), v))
        return false;

    for (int i = 0; i < r.getAsientos().longitud(); ++i) {
        string code = r.getAsientos().obtenerPos(i);
        int fila = stoi(code.substr(0, code.size() - 1));
        char letra = code.back();
        Asiento a;
        if (!repoAsientos.buscar(r.getVueloId(), fila, letra, a) ||
            a.isOcupado()) {
            return false;
        }
    }

    for (int i = 0; i < r.getAsientos().longitud(); ++i) {
        string code = r.getAsientos().obtenerPos(i);
        int fila = stoi(code.substr(0, code.size() - 1));
        char letra = code.back();
        Asiento a;
        repoAsientos.buscar(r.getVueloId(), fila, letra, a);
        a.setOcupado(true);
        repoAsientos.actualizar(a);
    }

    return repoReservas.crearReserva(r);
}

bool ServicioReservas::cancelarReserva(const string& codigo) {
    Reserva r;
    if (!repoReservas.buscarPorCodigo(codigo, r) ||
        r.getEstado() == EstadoReserva::CANCELADA) {
        return false;
    }

    for (int i = 0; i < r.getAsientos().longitud(); ++i) {
        string code = r.getAsientos().obtenerPos(i);
        int fila = stoi(code.substr(0, code.size() - 1));
        char letra = code.back();
        Asiento a;
        if (repoAsientos.buscar(r.getVueloId(), fila, letra, a)) {
            a.setOcupado(false);
            repoAsientos.actualizar(a);
        }
    }

    r.cancelar();
    return repoReservas.actualizar(r);
}
