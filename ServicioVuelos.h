// ServicioVuelos.h
#pragma once

#include "RepoVuelos.h"
#include "RepoAsientos.h"
#include "Lista.h"
#include "Vuelo.h"
#include "Asiento.h"

using namespace std;

class ServicioVuelos {
private:
    RepoVuelos   repoVuelos;
    RepoAsientos repoAsientos;

public:
    ServicioVuelos() = default;

    // Lista todos los vuelos
    Lista<Vuelo> listarVuelos() const;

    // Crea un vuelo y genera automáticamente sus asientos
    bool crearVuelo(const Vuelo& v);

    // Modifica solo la cabecera del vuelo
    bool modificarVuelo(const Vuelo& v);

    // Elimina vuelo y todos sus asientos
    bool eliminarVuelo(int id);

    // Busca un vuelo por ID
    bool buscarVuelo(int id, Vuelo& v) const;
};
