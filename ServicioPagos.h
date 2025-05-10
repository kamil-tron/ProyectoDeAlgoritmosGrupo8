#pragma once
#include "RepoPagos.h"
#include "RepoReservas.h"
#include "Pago.h"
#include "Reserva.h"
#include "Lista.h"
#include <string>

class ServicioPagos {
private:
    RepoPagos    repoPagos;
    RepoReservas repoReservas;

public:
    ServicioPagos();

    bool procesarPago(const Pago& pago);

    bool obtenerPagoPorReserva(const std::string& reservaCodigo, Pago& p) const;

    Lista<Pago> listarPagosUsuario(const std::string& correo) const;
};