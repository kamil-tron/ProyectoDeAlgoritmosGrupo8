#pragma once
#include "FileManager.h"
#include "Aeropuerto.h"

using namespace std;

class RepoAeropuertos : public FileManager<Aeropuerto> {
public:
    RepoAeropuertos() : FileManager("aeropuertos.txt") {}

    bool existe(const string& cod) const {
        Aeropuerto tmp;
        return buscar(cod, tmp);
    }

    bool buscar(const string& cod, Aeropuerto& out) const {
        auto lista = cargarTodos();
        for (int i = 0; i < lista.longitud(); ++i)
            if (lista.obtenerPos(i).getCodigo() == cod) {
                out = lista.obtenerPos(i);
                return true;
            }
        return false;
    }

    bool agregar(const Aeropuerto& a) {
        if (existe(a.getCodigo())) return false;
        auto l = cargarTodos();
        l.agregaFinal(a);
        guardar(l);
        return true;
    }

    bool actualizar(const Aeropuerto& a) {
        auto l = cargarTodos();
        for (int i = 0; i < l.longitud(); ++i)
            if (l.obtenerPos(i).getCodigo() == a.getCodigo()) {
                l.modificarPos(a, i);
                guardar(l);
                return true;
            }
        return false;
    }
};
