#pragma once
#include "Lista.h"
#include "Vuelo.h"
#include "Reserva.h"

struct PrecioReserva {
    double precio;
    Reserva res;
};

void selectionSortPorPrecioDesc(Lista<Vuelo>& lista);
void insertionSortPorPrecioAsc(Lista<PrecioReserva>& lista);
int claveFecha(const std::string& f);
void shellSortPorFechaAsc(Lista<Vuelo>& lista);
