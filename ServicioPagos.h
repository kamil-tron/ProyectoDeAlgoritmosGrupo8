#pragma once
#include "RepoPagos.h"
#include "RepoReservas.h"
#include "Pago.h"
#include "Reserva.h"
#include "Lista.h"
#include <string>

using namespace std;

class ServicioPagos {
private:
    RepoPagos repoPagos;
    RepoReservas repoReservas;

public:
    ServicioPagos();

    /* existente */
    bool procesarPago(const Pago& pago);
    bool obtenerPagoPorReserva(const string& reservaCodigo, Pago& p) const;
    Lista<Pago> listarPagosUsuario(const string& correo) const;

    /* NUEVO */
    bool procesarPagoReserva(const Reserva& reserva,
        double monto,
        const string& metodo,
        Pago& outPago);
};