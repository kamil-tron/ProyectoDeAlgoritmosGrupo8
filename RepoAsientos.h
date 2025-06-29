#pragma once
#include <fstream>
#include <vector>
#include <string>
#include "Asiento.h"
#include "Lista.h"

struct CacheVuelo {
    Lista<Asiento> asientos;
    int libres = 0;
};

class RepoAsientos {
    mutable vector<CacheVuelo> cache;
    mutable bool cargado = false;
    const string file = "asientos.txt";

    void cargarTodo() const {
        if (cargado) return;

        int maxId = 0;
        {
            ifstream f(file); string ln;
            while (getline(f, ln)) {
                int id = stoi(ln.substr(0, ln.find(',')));
                if (id > maxId) maxId = id;
            }
        }
        cache.resize(maxId + 1);

        ifstream f(file); string ln;
        while (getline(f, ln)) {
            Asiento a = Asiento::fromString(ln);
            int id = a.getVueloId();
            auto& c = cache[id];
            c.asientos.agregaFinal(a);
            if (!a.isOcupado()) ++c.libres;
        }
        cargado = true;
    }

    void sobrescribirArchivo() const {
        ofstream out(file, ios::trunc);
        for (size_t id = 1; id < cache.size(); ++id) {
            const auto& c = cache[id];
            for (int i = 0; i < c.asientos.longitud(); ++i)
                out << c.asientos.obtenerPos(i).serialize() << '\n';
        }
    }

public:
    bool buscar(int id, int fila, char letra, Asiento& out) const {
        cargarTodo();
        const auto& l = cache[id].asientos;
        for (int i = 0; i < l.longitud(); ++i) {
            const Asiento& a = l.obtenerPos(i);
            if (a.getFila() == fila && a.getLetra() == letra) {
                out = a; return true;
            }
        }
        return false;
    }

    const Lista<Asiento>& listarPorVuelo(int id) const {
        cargarTodo();
        return cache[id].asientos;
    }

    int contarLibres(int id) const {
        cargarTodo();
        return cache[id].libres;
    }

    void agregar(const Asiento& a) {
        cargarTodo();
        int id = a.getVueloId();
        if (id >= (int)cache.size()) cache.resize(id + 1);

        auto& c = cache[id];
        c.asientos.agregaFinal(a);
        if (!a.isOcupado()) ++c.libres;

        ofstream out(file, ios::app);
        out << a.serialize() << '\n';
    }

    bool actualizar(const Asiento& nuevo) {
        cargarTodo();
        int id = nuevo.getVueloId();
        if (id >= (int)cache.size()) return false;

        auto& c = cache[id];
        for (int i = 0; i < c.asientos.longitud(); ++i) {
            Asiento& a = c.asientos.obtenerRef(i);
            if (a == nuevo) {
                if (a.isOcupado() != nuevo.isOcupado())
                    c.libres += nuevo.isOcupado() ? -1 : 1;
                a = nuevo;
                sobrescribirArchivo();
                return true;
            }
        }
        return false;
    }

    bool eliminar(int id, int fila, char letra) {
        cargarTodo();
        auto& c = cache[id];
        for (int i = 0; i < c.asientos.longitud(); ++i) {
            const Asiento& a = c.asientos.obtenerPos(i);
            if (a.getFila() == fila && a.getLetra() == letra) {
                if (!a.isOcupado()) --c.libres;
                c.asientos.eliminaPos(i);
                sobrescribirArchivo();
                return true;
            }
        }
        return false;
    }

    void eliminarPorVuelo(int id) {
        cargarTodo();
        if (id >= (int)cache.size()) return;
        cache[id].asientos.limpiar();
        cache[id].libres = 0;
        sobrescribirArchivo();
    }
};
