#include "ServicioPagos.h"

ServicioPagos::ServicioPagos() = default;

bool ServicioPagos::procesarPago(const Pago& p) {
    Reserva r;
    if (!repoReservas.buscarPorCodigo(to_string(p.getReservaId()), r)) return false;
    repoPagos.agregar(p);
    return true;
}

bool ServicioPagos::obtenerPagoPorReserva(int reservaId, Pago& p) const {
    return repoPagos.buscarPorReservaId(reservaId, p);
}