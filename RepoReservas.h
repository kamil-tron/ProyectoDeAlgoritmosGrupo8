#pragma once

#include "FileManager.h"
#include "Reserva.h"
#include "Lista.h"
#include <string>

using namespace std;

class RepoReservas : public FileManager<Reserva> {
public:
    RepoReservas() : FileManager("reservas.txt") {}

    bool crearReserva(const Reserva& r) {
        auto items = cargarTodos();
        for (int i = 0; i < items.longitud(); ++i)
            if (items.obtenerPos(i).getCodigo() == r.getCodigo())
                return false;
        items.agregaFinal(r);
        guardar(items);
        return true;
    }

    bool buscarPorCodigo(const string& codigo, Reserva& out) const {
        auto items = cargarTodos();
        for (int i = 0; i < items.longitud(); ++i) {
            const Reserva& tmp = items.obtenerPos(i);
            if (tmp.getCodigo() == codigo) {
                out = tmp;
                return true;
            }
        }
        return false;
    }

    Lista<Reserva> listarReservasUsuario(const string& email) const {
        Lista<Reserva> result;
        auto items = cargarTodos();
        for (int i = 0; i < items.longitud(); ++i) {
            const Reserva& tmp = items.obtenerPos(i);
            if (tmp.getUserEmail() == email)
                result.agregaFinal(tmp);
        }
        return result;
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

    bool cancelarReserva(const string& codigo) {
        Reserva r;
        if (!buscarPorCodigo(codigo, r))
            return false;
        if (r.getEstado() == EstadoReserva::CANCELADA)
            return false;
        r.cancelar();
        return actualizar(r);
    }
};
