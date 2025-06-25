#pragma once
#include "Cola.h"
#include "Pila.h"
#include "CheckIn.h"
#include "RepoCheckIn.h"
#include "RepoReservas.h"
#include "Reserva.h"

using namespace std;

class ServicioCheckIn {
private:
    Cola<CheckIn> colaPend;
    Pila<CheckIn> hist;
    RepoCheckIn repoCheck;
    RepoReservas repoRes;

public:
    ServicioCheckIn() {
        Lista<CheckIn> todos = repoCheck.cargarTodos();
        for (int i = 0; i < todos.longitud(); ++i) {
            const CheckIn& ci = todos.obtenerPos(i);
            Reserva r;
            if (!repoRes.buscarPorCodigo(ci.getReservaCod(), r)) continue;
            if (!r.isCheckedIn())
                colaPend.encolar(ci);
            else
                hist.apilar(ci);
        }
    }

    bool registrarEnCola(const string& reservaCod) {
        Reserva r;
        if (!repoRes.buscarPorCodigo(reservaCod, r) ||
            r.getEstado() != EstadoReserva::CONFIRMADA ||
            r.isCheckedIn())
            return false;

        CheckIn c(repoCheck.cargarTodos().longitud() + 1,
            r.getVueloId(),
            reservaCod,
            r.getUserEmail());

        colaPend.encolar(c);
        return repoCheck.registrarCheckIn(c);
    }

    bool procesarSiguiente(CheckIn& out) {
        if (colaPend.esVacia()) return false;

        out = colaPend.frente();
        colaPend.desencolar();
        hist.apilar(out);

        Reserva r;
        if (repoRes.buscarPorCodigo(out.getReservaCod(), r)) {
            r.marcarCheckIn();
            repoRes.actualizar(r);
        }
        return true;
    }

    Cola<CheckIn> pendientes() const { return colaPend; }
    Pila<CheckIn> ultimos() const { return hist; }
};
