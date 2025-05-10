#pragma once
#include "FileManager.h"
#include "Pago.h"
#include "Lista.h"
#include <string>

using namespace std;

class RepoPagos : public FileManager<Pago> {
public:
    RepoPagos() : FileManager("pagos.txt") {}

    bool buscarPorReservaCodigo(const string& codigoReserva, Pago& p) const {
        auto items = cargarTodos();
        for (int i = 0; i < items.longitud(); ++i) {
            Pago tmp = items.obtenerPos(i);
            if (tmp.getReservaCodigo() == codigoReserva) {
                p = tmp;
                return true;
            }
        }
        return false;
    }

    bool agregar(const Pago& p) {
        auto items = cargarTodos();

        for (int i = 0; i < items.longitud(); ++i)
            if (items.obtenerPos(i).getReservaCodigo() == p.getReservaCodigo())
                return false;

        items.agregaFinal(p);
        guardar(items);
        return true;
    }
};
