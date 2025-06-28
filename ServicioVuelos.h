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

// Temporal para recoger el recorrido in-order
static Lista<Vuelo>* listaTemporal = nullptr;

// Inserta un vuelo en la lista temporal
static void insertarEnListaTemporal(Vuelo v) {
	if (listaTemporal) listaTemporal->agregaFinal(v);
}

class ServicioVuelos {
private:
	RepoVuelos repoVuelos;
	RepoAsientos repoAsientos;
	RepoAeropuertos repoAeropuertos;

	HashTable<int, Vuelo>* idx;
	ArbolAVL<Vuelo>* vuelosPorFecha;

	void cargarIndice() {
		auto lista = repoVuelos.cargarTodos();
		for (int i = 0; i < lista.longitud(); ++i) {
			Vuelo v = lista.obtenerPos(i);
			idx->insertar(v.getId(), v);
			vuelosPorFecha->insertar(v);
		}
	}

public:
	ServicioVuelos()
		: idx(new HashTable<int, Vuelo>(2000, hashInt)),
		vuelosPorFecha(new ArbolAVL<Vuelo>(insertarEnListaTemporal))
	{
		cargarIndice();
	}

	~ServicioVuelos() {
		delete idx;
		delete vuelosPorFecha;
	}

	Lista<Vuelo> listarVuelos() const {
		return repoVuelos.cargarTodos();
	}

	Lista<Vuelo> listarVuelosPorFecha() const {
		Lista<Vuelo> ordenados;
		listaTemporal = &ordenados;
		vuelosPorFecha->inOrden();
		listaTemporal = nullptr;
		return ordenados;
	}

	bool buscarVuelo(int id, Vuelo& v) const {
		const Vuelo* p = idx->obtener(id);
		if (!p) return false;
		v = *p;
		return true;
	}

	bool crearVuelo(const Vuelo& v)
	{
		if (!repoAeropuertos.existe(v.getOrigen()) ||
			!repoAeropuertos.existe(v.getDestino()))
		{
			cout << "Error: aeropuerto no registrado ("
				<< v.getOrigen() << " o " << v.getDestino() << ").\n";
			return false;
		}

		repoVuelos.agregar(v);
		idx->insertar(v.getId(), v);
		vuelosPorFecha->insertar(v);

		int total = v.getCapacidad();
		int vipCut = total / 4;
		int count = 0;
		int fila = 1;

		while (count < total) {
			for (char letra = 'A'; letra <= 'F' && count < total; ++letra) {
				bool vip = (count < vipCut);
				repoAsientos.agregar(Asiento(v.getId(), fila, letra, false, vip));
				++count;
			}
			++fila;
		}
		return true;
	}

	bool modificarVuelo(const Vuelo& v)
	{
		if (!repoAeropuertos.existe(v.getOrigen()) ||
			!repoAeropuertos.existe(v.getDestino()))
		{
			cout << "Error: aeropuerto no registrado ("
				<< v.getOrigen() << " o " << v.getDestino() << ").\n";
			return false;
		}

		Vuelo viejo;
		if (!buscarVuelo(v.getId(), viejo)) {
			cout << "Error: vuelo no encontrado.\n";
			return false;
		}

		repoVuelos.actualizar(v);
		idx->eliminar(v.getId());
		idx->insertar(v.getId(), v);

		delete vuelosPorFecha;
		vuelosPorFecha = new ArbolAVL<Vuelo>(insertarEnListaTemporal);
		cargarIndice();

		return true;
	}

	bool eliminarVuelo(int id) {
		repoVuelos.eliminar(id);
		idx->eliminar(id);
		repoAsientos.eliminarPorVuelo(id);

		delete vuelosPorFecha;
		vuelosPorFecha = new ArbolAVL<Vuelo>(insertarEnListaTemporal);
		cargarIndice();

		return true;
	}

	Lista<Aeropuerto> listarAeropuertos() const {
		return repoAeropuertos.cargarTodos();
	}
};
