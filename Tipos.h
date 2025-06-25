#pragma once
#include "Vuelo.h"
#include <string>

using namespace std;

struct EntradaEspera { string correo; int idVuelo; int orden; };
struct EntradaCheckIn { string correo; int idVuelo; int orden; };

enum class ResultadoReserva { CONFIRMADA, LISTA_ESPERA, DUPLICADA, FALLO };

enum class TipoOperacion { ALTA, BAJA, MODIFICACION };
struct Operacion {
    TipoOperacion tipo;
    Vuelo antes;
    Vuelo despues;
};
