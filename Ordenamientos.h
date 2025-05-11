#pragma once
#include "Lista.h"
#include "Vuelo.h"
#include "Reserva.h"

struct PrecioReserva {
    double  precio;
    Reserva res;
};

/* ---------- prototipos ---------- */
void insertionSortPorPrecioAsc(Lista<Vuelo>& lista);
void selectionSortPorPrecioDesc(Lista<Vuelo>& lista);      // precio ↓
void insertionSortPorPrecioAsc(Lista<PrecioReserva>& lista); // precio ↑
int  claveFecha(const std::string& f);                     // dd/mm/yyyy → yyyymmdd
void shellSortPorFechaAsc(Lista<Vuelo>& lista);            // fecha ↑
