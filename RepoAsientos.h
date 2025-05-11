#pragma once

#include "FileManager.h"
#include "Asiento.h"
#include "Lista.h"

class RepoAsientos : public FileManager<Asiento> {
public:
    RepoAsientos() : FileManager("asientos.txt") {}

    bool buscar(int vueloId, int fila, char letra, Asiento& out) const {
        auto all = cargarTodos();
        for (int i = 0; i < all.longitud(); ++i) {
            Asiento a = all.obtenerPos(i);
            if (a.getVueloId() == vueloId
                && a.getFila() == fila
                && a.getLetra() == letra) {
                out = a;
                return true;
            }
        }
        return false;
    }

    Lista<Asiento> listarPorVuelo(int vueloId) const {
        auto all = cargarTodos();
        Lista<Asiento> result;
        for (int i = 0; i < all.longitud(); ++i) {
            Asiento a = all.obtenerPos(i);
            if (a.getVueloId() == vueloId) {
                result.agregaFinal(a);
            }
        }
        return result;
    }

    void agregar(const Asiento& a) {
        auto all = cargarTodos();
        all.agregaFinal(a);
        guardar(all);
    }

    bool actualizar(const Asiento& a) {
        auto all = cargarTodos();
        for (int i = 0; i < all.longitud(); ++i) {
            Asiento tmp = all.obtenerPos(i);
            if (tmp == a) {
                all.modificarPos(a, i);
                guardar(all);
                return true;
            }
        }
        return false;
    }

    bool eliminar(int vueloId, int fila, char letra) {
        auto all = cargarTodos();
        for (int i = 0; i < all.longitud(); ++i) {
            Asiento a = all.obtenerPos(i);
            if (a.getVueloId() == vueloId
                && a.getFila() == fila
                && a.getLetra() == letra) {
                all.eliminaPos(i);
                guardar(all);
                return true;
            }
        }
        return false;
    }

    void eliminarPorVuelo(int vueloId) {
        auto all = cargarTodos();
        Lista<Asiento> filtered;
        for (int i = 0; i < all.longitud(); ++i) {
            Asiento a = all.obtenerPos(i);
            if (a.getVueloId() != vueloId) {
                filtered.agregaFinal(a);
            }
        }
        guardar(filtered);
    }
};
