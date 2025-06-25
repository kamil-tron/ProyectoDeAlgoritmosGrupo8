#include "ServicioPagos.h"

ServicioPagos::ServicioPagos()
    : idx(new HashTable<string, Pago>(4000, hashString)) {
    cargarIndice();
}

ServicioPagos::~ServicioPagos() {
    delete idx;
}

void ServicioPagos::cargarIndice() {
    auto lista = repoPagos.cargarTodos();
    for (int i = 0; i < lista.longitud(); ++i)
        idx->insertar(lista.obtenerPos(i).getReservaCodigo(), lista.obtenerPos(i));
}

bool ServicioPagos::procesarPago(const Pago& pago) {
    Reserva r;
    if (!repoReservas.buscarPorCodigo(pago.getReservaCodigo(), r) ||
        r.getEstado() != EstadoReserva::PENDIENTE)
        return false;

    if (idx->contiene(pago.getReservaCodigo()))
        return false;

    repoPagos.agregar(pago);
    idx->insertar(pago.getReservaCodigo(), pago);

    if (pago.getEstado() == "COMPLETADO")
        r.confirmar();
    else
        r.cancelar();

    repoReservas.actualizar(r);
    return pago.getEstado() == "COMPLETADO";
}

bool ServicioPagos::obtenerPagoPorReserva(const string& reservaCodigo, Pago& p) const {
    const Pago* q = idx->obtener(reservaCodigo);
    if (!q) return false;
    p = *q;
    return true;
}

Lista<Pago> ServicioPagos::listarPagosUsuario(const string& correo) const {
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

bool ServicioPagos::procesarPagoReserva(const Reserva& reserva, double monto, const string& metodo, Pago& outPago) {
    string fecha = reserva.getFecha();
    Pago p(reserva.getCodigo(), monto, metodo, "COMPLETADO", fecha);
    bool ok = procesarPago(p);
    if (ok) outPago = p;
    return ok;
}
