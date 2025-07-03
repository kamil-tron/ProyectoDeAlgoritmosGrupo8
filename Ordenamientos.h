#pragma once

#include "Lista.h"
#include "Vuelo.h"
#include "Reserva.h"
#include "RutaPosible.h"

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



inline void mergeIds(Lista<int>& lista,
	int l, int m, int r,
	ServicioVuelos& sv)
{
	int n1 = m - l + 1;
	int n2 = r - m;

	int* L = new int[n1];
	int* R = new int[n2];

	for (int i = 0; i < n1; ++i) L[i] = lista.obtenerPos(l + i);
	for (int j = 0; j < n2; ++j) R[j] = lista.obtenerPos(m + 1 + j);

	int i = 0, j = 0, k = l;
	while (i < n1 && j < n2) {
		Vuelo vL, vR;
		sv.buscarVuelo(L[i], vL);
		sv.buscarVuelo(R[j], vR);

		if (vL.getFechaSerial() <= vR.getFechaSerial())
			lista.modificarPos(L[i++], k++);
		else
			lista.modificarPos(R[j++], k++);
	}
	while (i < n1) lista.modificarPos(L[i++], k++);
	while (j < n2) lista.modificarPos(R[j++], k++);

	delete[] L; delete[] R;
}

inline void mergeSortIdsAux(Lista<int>& lista,
	int l, int r,
	ServicioVuelos& sv)
{
	if (l < r) {
		int m = l + (r - l) / 2;
		mergeSortIdsAux(lista, l, m, sv);
		mergeSortIdsAux(lista, m + 1, r, sv);
		mergeIds(lista, l, m, r, sv);
	}
}

// función pública que llamarás desde ServicioRutas
inline void mergeSortIdsPorFechaAsc(Lista<int>& lista,
	ServicioVuelos& sv)
{
	if (lista.longitud() > 1)
		mergeSortIdsAux(lista, 0, lista.longitud() - 1, sv);
}

inline void mergeRutas(Lista<RutaPosible>& L,
	size_t l, size_t m, size_t r,
	bool porCosto)          // true = costo, false = distancia
{
	size_t n1 = m - l + 1, n2 = r - m;
	RutaPosible* A = new RutaPosible[n1];
	RutaPosible* B = new RutaPosible[n2];

	for (size_t i = 0; i < n1; ++i) A[i] = L.obtenerPos(l + i);
	for (size_t j = 0; j < n2; ++j) B[j] = L.obtenerPos(m + 1 + j);

	size_t i = 0, j = 0, k = l;
	while (i < n1 && j < n2) {
		bool menor = porCosto
			? (A[i].costoTotal <= B[j].costoTotal)
			: (A[i].distanciaTotal <= B[j].distanciaTotal);
		L.modificarPos(menor ? A[i++] : B[j++], k++);
	}
	while (i < n1) L.modificarPos(A[i++], k++);
	while (j < n2) L.modificarPos(B[j++], k++);

	delete[] A; delete[] B;
}

inline void mergeSortRutasAux(Lista<RutaPosible>& L,
	size_t l, size_t r, bool porCosto)
{
	if (l < r) {
		size_t m = l + (r - l) / 2;
		mergeSortRutasAux(L, l, m, porCosto);
		mergeSortRutasAux(L, m + 1, r, porCosto);
		mergeRutas(L, l, m, r, porCosto);
	}
}

inline void mergeSortRutas(Lista<RutaPosible>& L, bool porCosto)
{
	if (L.longitud() > 1)
		mergeSortRutasAux(L, 0, L.longitud() - 1, porCosto);
}

inline void mergeAsc(Lista<Vuelo>& L, int l, int m, int r)
{
	int n1 = m - l + 1, n2 = r - m;
	Vuelo* A = new Vuelo[n1], * B = new Vuelo[n2];

	for (int i = 0; i < n1; ++i) A[i] = L.obtenerPos(l + i);
	for (int j = 0; j < n2; ++j) B[j] = L.obtenerPos(m + 1 + j);

	int i = 0, j = 0, k = l;
	while (i < n1 && j < n2)
		L.modificarPos((A[i].getPrecio() <= B[j].getPrecio()) ? A[i++] : B[j++], k++);

	while (i < n1) L.modificarPos(A[i++], k++);
	while (j < n2) L.modificarPos(B[j++], k++);

	delete[] A; delete[] B;
}

inline void mergeSortPrecioAscAux(Lista<Vuelo>& L, int l, int r)
{
	if (l < r) {
		int m = l + (r - l) / 2;
		mergeSortPrecioAscAux(L, l, m);
		mergeSortPrecioAscAux(L, m + 1, r);
		mergeAsc(L, l, m, r);
	}
}

inline void mergeSortPorPrecioAsc(Lista<Vuelo>& L)
{
	if (L.longitud() > 1)
		mergeSortPrecioAscAux(L, 0, L.longitud() - 1);
}