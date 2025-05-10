#pragma once
#include "Nodo.h"
#include <functional>
#include <string>          // ←  string
#include <sstream>         // ←  toPrint()

using uint = unsigned int; // ←  alias moderno

using namespace std;

template <class T>
class Lista {
private:
	Nodo<T>* ini = nullptr;           // cabeza de la lista
	uint      lon = 0;                 // número de elementos
	using Comp = function<int(const T&, const T&)>;

public:
	Lista(const Lista& other) : ini(nullptr), lon(0) {          // 👉
		Nodo<T>* aux = other.ini;
		while (aux) {
			agregaFinal(aux->get_Elem());
			aux = aux->get_Sgte();
		}
	}

	Lista& operator=(const Lista& other) {                      // 👉
		if (this == &other) return *this;
		while (!esVacia()) eliminaInicial();          // limpia actual
		Nodo<T>* aux = other.ini;
		while (aux) {
			agregaFinal(aux->get_Elem());
			aux = aux->get_Sgte();
		}
		return *this;
	}

	Lista() = default;
	explicit Lista(Nodo<T>* pNodo) : ini(pNodo), lon(pNodo ? 1 : 0) {}
	~Lista();

	/*-------------------- operaciones básicas --------------------*/
	uint  longitud()  const { return lon; }
	bool  esVacia()   const { return lon == 0; }

	void agregaInicial(const T& elem);
	void agregaPos(const T& elem, uint pos);
	void agregaFinal(const T& elem) { agregaPos(elem, lon); }

	void modificarInicial(const T& elem);
	void modificarPos(const T& elem, uint pos);
	void modificarFinal(const T& elem) { modificarPos(elem, lon - 1); }

	void eliminaInicial();
	void eliminaPos(uint pos);
	void eliminaFinal() { eliminaPos(lon - 1); }

	T obtenerInicial()         const { return obtenerPos(0); }
	T obtenerPos(uint pos)     const;
	T obtenerFinal()           const { return obtenerPos(lon - 1); }

	T buscar(const T& elem)    const;

	/*-------------------- utilidades extra ----------------------*/
	string toPrint(const string& sep = " ") const;
	int  size() const { return static_cast<int>(lon); }
};

/*==============================================================*/
/*==================   IMPLEMENTACIÓN BÁSICA   =================*/
/*==============================================================*/

template <class T>
Lista<T>::~Lista() {
	while (!esVacia()) eliminaInicial();
}

template <class T>
void Lista<T>::agregaInicial(const T& elem) {
	ini = new Nodo<T>(elem, ini);
	++lon;
}

template <class T>
void Lista<T>::agregaPos(const T& elem, uint pos) {
	if (pos > lon) return;
	if (pos == 0) { agregaInicial(elem); return; }

	Nodo<T>* aux = ini;
	for (uint i = 1; i < pos; ++i) aux = aux->get_Sgte();

	aux->set_Sgte(new Nodo<T>(elem, aux->get_Sgte()));
	++lon;
}

template <class T>
void Lista<T>::modificarInicial(const T& elem) {
	if (!esVacia()) ini->set_Elem(elem);
}

template <class T>
void Lista<T>::modificarPos(const T& elem, uint pos) {
	if (pos >= lon) return;
	Nodo<T>* aux = ini;
	for (uint i = 0; i < pos; ++i) aux = aux->get_Sgte();
	aux->set_Elem(elem);
}

template <class T>
void Lista<T>::eliminaInicial() {
	if (esVacia()) return;
	Nodo<T>* tmp = ini;
	ini = ini->get_Sgte();
	delete tmp;
	--lon;
}

template <class T>
void Lista<T>::eliminaPos(uint pos) {
	if (pos >= lon) return;
	if (pos == 0) { eliminaInicial(); return; }

	Nodo<T>* ant = ini;
	for (uint i = 1; i < pos; ++i) ant = ant->get_Sgte();

	Nodo<T>* vict = ant->get_Sgte();
	ant->set_Sgte(vict->get_Sgte());
	delete vict;
	--lon;
}

template <class T>
T Lista<T>::obtenerPos(uint pos) const {
	if (pos >= lon) return T();                // ← valor‐por‐defecto
	Nodo<T>* aux = ini;
	for (uint i = 0; i < pos; ++i) aux = aux->get_Sgte();
	return aux->get_Elem();
}

template <class T>
T Lista<T>::buscar(const T& elem) const {
	Nodo<T>* aux = ini;
	while (aux) {
		if (aux->get_Elem() == elem) return aux->get_Elem();
		aux = aux->get_Sgte();
	}
	return T();                                // ← no encontrado
}

template <class T>
string Lista<T>::toPrint(const string& sep) const {
	ostringstream os;
	Nodo<T>* aux = ini;
	while (aux) {
		os << aux->get_Elem();
		aux = aux->get_Sgte();
		if (aux) os << sep;
	}
	return os.str();
}
