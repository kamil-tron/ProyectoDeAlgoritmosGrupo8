#pragma once
#include "Lista.h"
#include "Vuelo.h"

struct RutaPosible {
    Lista<int>   indicesAeropuertos;
    Lista<Vuelo> vuelos;
    double       distanciaTotal{ 0 };
    double       costoTotal{ 0 };
};
