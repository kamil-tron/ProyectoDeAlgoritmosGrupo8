#include "ServicioPagos.h"

ServicioPagos::ServicioPagos() = default;

bool ServicioPagos::procesarPago(const Pago& pago) {
    Reserva r;
    if (!repoReservas.buscarPorCodigo(pago.getReservaCodigo(), r))
        return false;
    if (r.getEstado() != EstadoReserva::PENDIENTE)
        return false;

    Pago existente;
    if (repoPagos.buscarPorReservaCodigo(pago.getReservaCodigo(), existente))
        return false;

    if (!repoPagos.agregar(pago))
        return false;

    if (pago.getEstado() == "COMPLETADO")
        r.confirmar();
    else
        r.cancelar();

    repoReservas.actualizar(r);
    return pago.getEstado() == "COMPLETADO";
}

bool ServicioPagos::obtenerPagoPorReserva(const std::string& reservaCodigo, Pago& p) const {
    return repoPagos.buscarPorReservaCodigo(reservaCodigo, p);
}

Lista<Pago> ServicioPagos::listarPagosUsuario(const std::string& correo) const {
    auto todos = repoPagos.cargarTodos();
    Lista<Pago> result;
    for (int i = 0; i < todos.longitud(); ++i) {
        const Pago& pg = todos.obtenerPos(i);
        Reserva res;
        if (repoReservas.buscarPorCodigo(pg.getReservaCodigo(), res) &&
            res.getUserEmail() == correo) {
            result.agregaFinal(pg);
        }
    }
    return result;
}