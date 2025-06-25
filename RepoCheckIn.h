#pragma once

#include "FileManager.h"
#include "CheckIn.h"
#include "Lista.h"
#include <string>

using namespace std;

class RepoCheckIn : public FileManager<CheckIn> {
public:
    RepoCheckIn() : FileManager("checkins.txt") {}

    bool registrarCheckIn(const CheckIn& c) {
        auto items = cargarTodos();
        for (int i = 0; i < items.longitud(); ++i)
            if (items.obtenerPos(i).getReservaCod() == c.getReservaCod())
                return false;
        items.agregaFinal(c);
        guardar(items);
        return true;
    }


    bool buscarPorId(int id, CheckIn& out) const {
        auto items = cargarTodos();
        for (int i = 0; i < items.longitud(); ++i) {
            const CheckIn& tmp = items.obtenerPos(i);
            if (tmp.getId() == id) {
                out = tmp;
                return true;
            }
        }
        return false;
    }

    bool buscarPorReserva(const string& codReserva, CheckIn& out) const {
        auto items = cargarTodos();
        for (int i = 0; i < items.longitud(); ++i) {
            const CheckIn& tmp = items.obtenerPos(i);
            if (tmp.getReservaCod() == codReserva) {
                out = tmp;
                return true;
            }
        }
        return false;
    }

    Lista<CheckIn> listarPorVuelo(int vueloId) const {
        Lista<CheckIn> result;
        auto items = cargarTodos();
        for (int i = 0; i < items.longitud(); ++i) {
            const CheckIn& tmp = items.obtenerPos(i);
            if (tmp.getVueloId() == vueloId)
                result.agregaFinal(tmp);
        }
        return result;
    }

    bool actualizar(const CheckIn& c) {
        auto items = cargarTodos();
        for (int i = 0; i < items.longitud(); ++i) {
            if (items.obtenerPos(i).getId() == c.getId()) {
                items.modificarPos(c, i);
                guardar(items);
                return true;
            }
        }
        return false;
    }
};
