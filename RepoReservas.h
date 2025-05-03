#pragma once
#include "FileManager.h"
#include "Reserva.h"
#include "Lista.h"
#include <string>

using namespace std;

class RepoReservas : public FileManager<Reserva> {
public:
    RepoReservas() : FileManager("reservas.txt") {}

    bool buscarPorCodigo(const string& codigo, Reserva& r) const {
        auto items = cargarTodos();
        for (int i = 0; i < items.longitud(); ++i) {
            Reserva tmp = items.obtenerPos(i);
            if (tmp.getCodigo() == codigo) {
                r = tmp;
                return true;
            }
        }
        return false;
    }

    void agregar(const Reserva& r) {
        auto items = cargarTodos();
        items.agregaFinal(r);
        guardar(items);
    }

    bool actualizar(const Reserva& r) {
        auto items = cargarTodos();
        for (int i = 0; i < items.longitud(); ++i) {
            if (items.obtenerPos(i).getCodigo() == r.getCodigo()) {
                items.modificarPos(r, i);
                guardar(items);
                return true;
            }
        }
        return false;
    }
};