#pragma once

#include "RepoPagos.h"
#include "RepoReservas.h"
#include "Pago.h"

class ServicioPagos {
private:
    RepoPagos    repoPagos;
    RepoReservas repoReservas;
public:
    ServicioPagos();

    bool procesarPago(const Pago& p);

    bool obtenerPagoPorReserva(int reservaId, Pago& p) const;
};