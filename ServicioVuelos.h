#pragma once
#include "RepoVuelos.h"
#include "RepoAsientos.h"
#include "RepoAeropuertos.h"
#include "Lista.h"
#include "HashTable.h"
#include "FuncionesHash.h"
#include "Vuelo.h"
#include "ArbolAVL.h"

using namespace std;

class ServicioVuelos {
private:
	RepoVuelos repoVuelos;
	RepoAsientos repoAsientos;
	RepoAeropuertos repoAeropuertos;

	HashTable<int, Vuelo>* idx;
	ArbolAVL<Vuelo>* vuelosPorFecha;

	void cargarIndice();

public:
	ServicioVuelos();
	~ServicioVuelos();

	Lista<Vuelo> listarVuelos() const;
	Lista<Vuelo> listarVuelosPorFecha() const;

	bool buscarVuelo(int id, Vuelo& v) const;
	bool crearVuelo(const Vuelo& v);
	bool modificarVuelo(const Vuelo& v);
	bool eliminarVuelo(int id);
	Lista<Aeropuerto> listarAeropuertos() const;
};