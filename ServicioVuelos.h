#pragma once

#include "RepoVuelos.h"
#include "Lista.h"

class ServicioVuelos {
private:
	RepoVuelos repoVuelos;
public:
	ServicioVuelos() = default;

	Lista<Vuelo> listarVuelos() const {
		return repoVuelos.cargarTodos();
	}

	bool crearVuelo(const Vuelo& v) {
		repoVuelos.agregar(v);
		return true;
	}

	bool modificarVuelo(const Vuelo& v) {
		return repoVuelos.actualizar(v);
	}

	bool eliminarVuelo(int id) {
		return repoVuelos.eliminar(id);
	}

	bool buscarVuelo(int id, Vuelo& v) const {
		return repoVuelos.buscarPorId(id, v);
	}
};