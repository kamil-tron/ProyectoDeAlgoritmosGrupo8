#pragma once
#include "FileManager.h"
#include "Pago.h"
#include "Lista.h"
#include <string>

using namespace std;

class RepoPagos : public FileManager<Pago> {
public:
	RepoPagos() : FileManager("pagos.txt") {}

	bool buscarPorReservaId(int reservaId, Pago& p) const {
		auto items = cargarTodos();
		for (int i = 0; i < items.longitud(); ++i) {
			Pago tmp = items.obtenerPos(i);
			if (tmp.getReservaId() == reservaId) {
				p = tmp;
				return true;
			}
		}
		return false;
	}

	void agregar(const Pago& p) {
		auto items = cargarTodos();
		items.agregaFinal(p);
		guardar(items);
	}
};