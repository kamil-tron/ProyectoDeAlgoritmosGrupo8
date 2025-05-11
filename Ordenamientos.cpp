#include "Ordenamientos.h"
#include <string>
void insertionSortPorPrecioAsc(Lista<Vuelo>& lista) {
    int n = lista.longitud();
    for (int i = 1; i < n; ++i) {
        Vuelo aux = lista.obtenerPos(i);
        int j = i - 1;
        // mientras queden elementos y su precio sea mayor que aux.precio
        while (j >= 0 && lista.obtenerPos(j).getPrecio() > aux.getPrecio()) {
            lista.modificarPos(lista.obtenerPos(j), j + 1);
            --j;
        }
        lista.modificarPos(aux, j + 1);
    }
}
/* ───────── Selection Sort (precio ↓) ───────── */
void selectionSortPorPrecioDesc(Lista<Vuelo>& lista) {
    int n = lista.longitud();
    for (int i = 0; i < n - 1; ++i) {
        int pMayor = i;
        for (int j = i + 1; j < n; ++j) {
            if (lista.obtenerPos(j).getPrecio() > lista.obtenerPos(pMayor).getPrecio())
                pMayor = j;
        }
        if (pMayor != i) {
            Vuelo tmp = lista.obtenerPos(i);
            lista.modificarPos(lista.obtenerPos(pMayor), i);
            lista.modificarPos(tmp, pMayor);
        }
    }
}

/* ───────── Insertion Sort (precio ↑) ───────── */
void insertionSortPorPrecioAsc(Lista<PrecioReserva>& lista) {
    int n = lista.longitud();
    for (int i = 1; i < n; ++i) {
        PrecioReserva aux = lista.obtenerPos(i);
        int j = i - 1;
        while (j >= 0 && lista.obtenerPos(j).precio > aux.precio) {
            lista.modificarPos(lista.obtenerPos(j), j + 1);
            --j;
        }
        lista.modificarPos(aux, j + 1);
    }
}

/* ─────────── Utilidad de fecha ─────────── */
int claveFecha(const std::string& f) {
    try {
        int d = std::stoi(f.substr(0, 2));
        int m = std::stoi(f.substr(3, 2));
        int y = std::stoi(f.substr(6, 4));
        return y * 10000 + m * 100 + d;          // yyyymmdd
    }
    catch (...) {
        return 0;                                // formato inválido
    }
}

/* ────────── Shell Sort (fecha ↑) ────────── */
void shellSortPorFechaAsc(Lista<Vuelo>& lista) {
    int n = lista.longitud();
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; ++i) {
            Vuelo temp = lista.obtenerPos(i);
            int j = i;
            while (j >= gap &&
                claveFecha(lista.obtenerPos(j - gap).getFecha()) >
                claveFecha(temp.getFecha())) {
                lista.modificarPos(lista.obtenerPos(j - gap), j);
                j -= gap;
            }
            lista.modificarPos(temp, j);
        }
    }
}
