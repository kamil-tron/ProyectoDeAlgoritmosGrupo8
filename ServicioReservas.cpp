// ServicioReservas.cpp
#include "ServicioReservas.h"
#include "Asiento.h"
#include "Reserva.h"
#include <string>

Lista<Reserva> ServicioReservas::listarReservasUsuario(const string& email) const {
    // Ahora devolvemos sólo las reservas CONFIRMADAS o PENDIENTES (filtramos CANCELADAS)
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
    // 1) Verificar que el vuelo exista
    Vuelo v;
    if (!repoVuelos.buscarPorId(r.getVueloId(), v))
        return false;

    // 2) Validar disponibilidad de asientos
    for (int i = 0; i < r.getAsientos().longitud(); ++i) {
        string code = r.getAsientos().obtenerPos(i);
        int fila = stoi(code.substr(0, code.size() - 1));
        char letra = code.back();
        Asiento a;
        if (!repoAsientos.buscar(r.getVueloId(), fila, letra, a) ||
            a.isOcupado())
        {
            return false;
        }
    }

    // 3) Marcar asientos como ocupados
    for (int i = 0; i < r.getAsientos().longitud(); ++i) {
        string code = r.getAsientos().obtenerPos(i);
        int fila = stoi(code.substr(0, code.size() - 1));
        char letra = code.back();
        Asiento a;
        repoAsientos.buscar(r.getVueloId(), fila, letra, a);
        a.setOcupado(true);
        repoAsientos.actualizar(a);
    }

    // 4) Crear la reserva en estado PENDIENTE
    //    (Repositorio usa crearReserva que evita duplicados)
    return repoReservas.crearReserva(r);
}

bool ServicioReservas::cancelarReserva(const string& codigo) {
    // 1) Obtener reserva y validar estado
    Reserva r;
    if (!repoReservas.buscarPorCodigo(codigo, r) ||
        r.getEstado() == EstadoReserva::CANCELADA)
    {
        return false;
    }

    // 2) Liberar los asientos
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

    // 3) Marcar reserva como CANCELADA y persistir
    r.cancelar();
    return repoReservas.actualizar(r);
}
