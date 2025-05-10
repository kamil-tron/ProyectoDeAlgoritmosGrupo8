#pragma once

#include "FileManager.h"
#include "Reserva.h"
#include "Lista.h"
#include <string>

using namespace std;

class RepoReservas : public FileManager<Reserva> {
public:
    RepoReservas() : FileManager("reservas.txt") {}

    // 1) Crear una nueva reserva (evita duplicados por código)
    bool crearReserva(const Reserva& r) {
        auto items = cargarTodos();
        for (int i = 0; i < items.longitud(); ++i)
            if (items.obtenerPos(i).getCodigo() == r.getCodigo())
                return false;
        items.agregaFinal(r);
        guardar(items);
        return true;
    }

    // 2) Buscar por código
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

    // 3) Listar todas las reservas de un usuario (por email)
    //    Incluye pendientes, confirmadas y canceladas
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

    // 4) Actualizar una reserva existente (coincide por código)
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

    // 5) Cancela una reserva (si no lo estaba) y persiste el nuevo estado
    bool cancelarReserva(const string& codigo) {
        Reserva r;
        if (!buscarPorCodigo(codigo, r))
            return false;
        if (r.getEstado() == EstadoReserva::CANCELADA)
            return false;
        r.cancelar();              // pasa a EstadoReserva::CANCELADA
        return actualizar(r);
    }
};
