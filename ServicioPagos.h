#pragma once
#include "RepoPagos.h"
#include "RepoReservas.h"
#include "Pago.h"
#include "Reserva.h"
#include "Lista.h"
#include "HashTable.h"
#include "FuncionesHash.h"
#include <string>

class ServicioPagos {
private:
    RepoPagos repoPagos;
    RepoReservas repoReservas;
    HashTable<string, Pago>* idx;

    void cargarIndice();

public:
    ServicioPagos();
    ~ServicioPagos();

    bool procesarPago(const Pago& pago);
    bool obtenerPagoPorReserva(const string& reservaCodigo, Pago& p) const;
    Lista<Pago> listarPagosUsuario(const string& correo) const;
    bool procesarPagoReserva(const Reserva& reserva, double monto, const string& metodo, Pago& outPago);
};
