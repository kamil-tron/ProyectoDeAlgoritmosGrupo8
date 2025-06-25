#pragma once
#include "Lista.h"
#include "Vuelo.h"
#include "Reserva.h"

struct PrecioReserva {
    double  precio;
    Reserva res;
};

void mergeSortPorPrecioDesc(Lista<Vuelo>& lista);      // precio ↓