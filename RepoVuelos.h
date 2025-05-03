#pragma once

#include "FileManager.h"
#include "Vuelo.h"
#include "Lista.h"


class RepoVuelos : public FileManager<Vuelo> {
public:
    RepoVuelos() : FileManager("vuelos.txt") {}

    bool buscarPorId(int id, Vuelo& v) const {
        auto items = cargarTodos();
        for (int i = 0; i < items.longitud(); ++i) {
            Vuelo tmp = items.obtenerPos(i);
            if (tmp.getId() == id) {
                v = tmp;
                return true;
            }
        }
        return false;
    }

    void agregar(const Vuelo& v) {
        auto items = cargarTodos();
        items.agregaFinal(v);
        guardar(items);
    }

    bool actualizar(const Vuelo& v) {
        auto items = cargarTodos();
        for (int i = 0; i < items.longitud(); ++i) {
            if (items.obtenerPos(i).getId() == v.getId()) {
                items.modificarPos(v, i);
                guardar(items);
                return true;
            }
        }
        return false;
    }

    bool eliminar(int id) {
        auto items = cargarTodos();
        for (int i = 0; i < items.longitud(); ++i) {
            if (items.obtenerPos(i).getId() == id) {
                items.eliminaPos(i);
                guardar(items);
                return true;
            }
        }
        return false;
    }
};