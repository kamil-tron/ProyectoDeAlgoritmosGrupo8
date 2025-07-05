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

inline int particionReservas(Lista<Reserva>& lista,
	int p, int r,
	ServicioReservas& svc) {
	// pivot = total de la última reserva
	Reserva pivRes = lista.obtenerPos(r);
	double piv = svc.calcularTotal(pivRes.getVueloId(), pivRes.getAsientos());

	int i = p - 1;
	for (int j = p; j < r; ++j) {
		Reserva curr = lista.obtenerPos(j);
		double currPrecio = svc.calcularTotal(curr.getVueloId(), curr.getAsientos());
		if (currPrecio <= piv) {
			++i;
			// swap lista[i] <–> lista[j]
			Reserva tmp = lista.obtenerPos(i);
			lista.modificarPos(curr, i);
			lista.modificarPos(tmp, j);
		}
	}
	// swap pivot en su posición final
	Reserva tmp = lista.obtenerPos(i + 1);
	lista.modificarPos(pivRes, i + 1);
	lista.modificarPos(tmp, r);
	return i + 1;
}

inline void quickSortReservasAux(Lista<Reserva>& lista,
	int p, int r,
	ServicioReservas& svc) {
	if (p < r) {
		int q = particionReservas(lista, p, r, svc);
		quickSortReservasAux(lista, p, q - 1, svc);
		quickSortReservasAux(lista, q + 1, r, svc);
	}
}

inline void quickSortReservasPorPrecioAsc(Lista<Reserva>& lista,
	ServicioReservas& svc) {
	if (lista.longitud() > 1)
		quickSortReservasAux(lista, 0, lista.longitud() - 1, svc);
}

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
inline void heapifyMin(Lista<Reserva>& lista, int n, int i, ServicioReservas& svc) {
	int smallest = i;
	int l = 2 * i + 1;
	int r = 2 * i + 2;

	// precio de i
	Reserva ri = lista.obtenerPos(i);
	double pi = svc.calcularTotal(ri.getVueloId(), ri.getAsientos());

	// hijo izquierdo
	if (l < n) {
		Reserva rl = lista.obtenerPos(l);
		double pl = svc.calcularTotal(rl.getVueloId(), rl.getAsientos());
		if (pl < pi) {
			smallest = l;
			pi = pl;
		}
	}

	// hijo derecho
	if (r < n) {
		Reserva rr = lista.obtenerPos(r);
		double pr = svc.calcularTotal(rr.getVueloId(), rr.getAsientos());
		if (pr < pi) {
			smallest = r;
			// pi = pr;  // no es necesario luego
		}
	}

	if (smallest != i) {
		// intercambia lista[i] <-> lista[smallest]
		Reserva tmp = lista.obtenerPos(i);
		lista.modificarPos(lista.obtenerPos(smallest), i);
		lista.modificarPos(tmp, smallest);

		// recursión en el hijo donde cayó el valor
		heapifyMin(lista, n, smallest, svc);
	}
}

inline void heapSortDesc(Lista<Reserva>& lista, ServicioReservas& svc) {
	int n = lista.longitud();
	if (n < 2) return;

	// 1) Construir min-heap
	for (int i = n / 2 - 1; i >= 0; --i)
		heapifyMin(lista, n, i, svc);

	// 2) Extraer elementos uno a uno desde el final
	for (int i = n - 1; i > 0; --i) {
		// swap raíz (menor) con lista[i]
		Reserva tmp = lista.obtenerPos(0);
		lista.modificarPos(lista.obtenerPos(i), 0);
		lista.modificarPos(tmp, i);

		// reconstruir heap en rango [0…i)
		heapifyMin(lista, i, 0, svc);
	}
	// al terminar, `lista` queda de mayor a menor
}