#pragma once

#include "Lista.h"
#include "Vuelo.h"
#include "Reserva.h"

struct PrecioReserva {
	double precio;
	Reserva res;
};

inline void merge(Lista<Vuelo>& lista, int l, int m, int r) {
	int n1 = m - l + 1;
	int n2 = r - m;
	Vuelo* L = new Vuelo[n1];
	Vuelo* R = new Vuelo[n2];
	for (int i = 0; i < n1; ++i) L[i] = lista.obtenerPos(l + i);
	for (int j = 0; j < n2; ++j) R[j] = lista.obtenerPos(m + 1 + j);
	int i = 0, j = 0, k = l;
	while (i < n1 && j < n2) {
		if (L[i].getPrecio() >= R[j].getPrecio())
			lista.modificarPos(L[i++], k++);
		else
			lista.modificarPos(R[j++], k++);
	}
	while (i < n1) lista.modificarPos(L[i++], k++);
	while (j < n2) lista.modificarPos(R[j++], k++);
	delete[] L;
	delete[] R;
}

inline void mergeSortAux(Lista<Vuelo>& lista, int l, int r) {
	if (l < r) {
		int m = l + (r - l) / 2;
		mergeSortAux(lista, l, m);
		mergeSortAux(lista, m + 1, r);
		merge(lista, l, m, r);
	}
}

inline void mergeSortPorPrecioDesc(Lista<Vuelo>& lista) {
	mergeSortAux(lista, 0, lista.longitud() - 1);
}

inline int particion(Lista<PrecioReserva>& lista, int p, int r) {
	PrecioReserva pivote = lista.obtenerPos(r);
	int i = p - 1;
	for (int j = p; j < r; ++j) {
		if (lista.obtenerPos(j).precio <= pivote.precio) {
			++i;
			PrecioReserva temp = lista.obtenerPos(i);
			lista.modificarPos(lista.obtenerPos(j), i);
			lista.modificarPos(temp, j);
		}
	}
	PrecioReserva temp = lista.obtenerPos(i + 1);
	lista.modificarPos(lista.obtenerPos(r), i + 1);
	lista.modificarPos(temp, r);
	return i + 1;
}

inline void quickSortAux(Lista<PrecioReserva>& lista, int p, int r) {
	if (p < r) {
		int q = particion(lista, p, r);
		quickSortAux(lista, p, q - 1);
		quickSortAux(lista, q + 1, r);
	}
}

inline void quickSortPorPrecioAsc(Lista<PrecioReserva>& lista) {
	quickSortAux(lista, 0, lista.longitud() - 1);
}

// ejemplo para un ordenamiento con quicksort
/*
* Debes definir lo primero en el .h
void quickSortPorPrecioAsc(Lista<PrecioReserva>& lista) {
	quickSortAux(lista, 0, lista.longitud() - 1);
}
*/
