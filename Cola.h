#pragma once
#include "Nodo.h"
#include <iostream>

template <class T>
class Cola {
private:
	Nodo<T>* frente_;
	Nodo<T>* final_;

public:
	Cola() : frente_(nullptr), final_(nullptr) {}

	~Cola() {
		while (!estaVacia())
			desencolar();
	}

	bool estaVacia() const {
		return frente_ == nullptr;
	}

	void encolar(const T& elem) {
		Nodo<T>* nuevo = new Nodo<T>(elem);
		if (estaVacia()) {
			frente_ = final_ = nuevo;
		}
		else {
			final_->set_Sgte(nuevo);
			final_ = nuevo;
		}
	}

	void desencolar() {
		if (estaVacia()) {
			std::cout << "La cola est� vac�a. No se puede desencolar.\n";
			return;
		}
		Nodo<T>* temp = frente_;
		frente_ = frente_->get_Sgte();
		delete temp;

		if (frente_ == nullptr)
			final_ = nullptr;
	}

	T frente() const {
		if (estaVacia()) {
			std::cout << "La cola est� vac�a. Retornando valor por defecto.\n";
			return T(); // Retorna un objeto por defecto si est� vac�a
		}
		return frente_->get_Elem();
	}

	void mostrar() const {
		Nodo<T>* actual = frente_;
		std::cout << "Cola (frente -> final): ";
		while (actual != nullptr) {
			std::cout << actual->get_Elem() << " -> ";
			actual = actual->get_Sgte();
		}
		std::cout << "NULL\n";
	}
};
