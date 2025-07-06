#pragma once
#include "Cola.h"
#include "Pila.h"
#include "CheckIn.h"
#include "RepoCheckIn.h"
#include "RepoReservas.h"
#include "Reserva.h"
#include "HashTable.h"
#include "FuncionesHash.h"

using namespace std;

class ServicioCheckIn {
private:
    Cola<CheckIn> colaPend;
    Pila<CheckIn> hist;
    RepoCheckIn repoCheck;
    RepoReservas repoRes;
    HashTable<string, Reserva>* idxRes;

    void cargarIndice() {
        auto todas = repoRes.cargarTodos();
        for (int i = 0; i < todas.longitud(); ++i) {
            const Reserva& r = todas.obtenerPos(i);
            idxRes->insertar(r.getCodigo(), r);
            if (!r.isCheckedIn())
                colaPend.encolar(CheckIn(0, r.getVueloId(), r.getCodigo(), r.getUserEmail()));
            else
                hist.apilar(CheckIn(0, r.getVueloId(), r.getCodigo(), r.getUserEmail()));
        }
    }

public:
    ServicioCheckIn()
        : idxRes(new HashTable<string, Reserva>(4000, hashString)) {
        cargarIndice();
    }

    ~ServicioCheckIn() { delete idxRes; }

    bool registrarEnCola(const string& reservaCod) {
        cargarIndice();
        Reserva* r = idxRes->obtener(reservaCod);
        if (!r || r->getEstado() != EstadoReserva::CONFIRMADA || r->isCheckedIn())
            return false;

        CheckIn c(repoCheck.cargarTodos().longitud() + 1,
            r->getVueloId(),
            reservaCod,
            r->getUserEmail());
        colaPend.encolar(c);
        repoCheck.registrarCheckIn(c);
        return true;
    }

    bool procesarSiguiente(CheckIn& out) {
        if (colaPend.esVacia()) return false;

        out = colaPend.frente();
        colaPend.desencolar();
        hist.apilar(out);

        Reserva* r = idxRes->obtener(out.getReservaCod());
        if (r) {
            r->marcarCheckIn();
            repoRes.actualizar(*r);
        }
        return true;
    }

    // Devuelven referencias const: NO copian la cola ni la pila
    const Cola<CheckIn>& pendientes() const {
        return colaPend;
    }
    const Pila<CheckIn>& ultimos() const {
        return hist;
    }
};
