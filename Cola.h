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
		while (!esVacia())
			desencolar();
	}

	bool esVacia() const {
		return frente_ == nullptr;
	}
	// Número de elementos en la cola
	int size() const {
		int cnt = 0;
		for (Nodo<T>* p = frente_; p; p = p->get_Sgte())
			++cnt;
		return cnt;
	}

	// Acceso al elemento i-ésimo (0-based, de frente hacia atrás)
	const T& at(int index) const {
		Nodo<T>* p = frente_;
		while (p && index-- > 0)
			p = p->get_Sgte();
		if (!p) throw std::out_of_range("Cola::at");
		return p->get_Elem();
	}

	void encolar(const T& elem) {
		Nodo<T>* nuevo = new Nodo<T>(elem);
		if (esVacia()) {
			frente_ = final_ = nuevo;
		}
		else {
			final_->set_Sgte(nuevo);
			final_ = nuevo;
		}
	}

	void desencolar() {
		if (esVacia()) {
			std::cout << "La cola está vacía. No se puede desencolar.\n";
			return;
		}
		Nodo<T>* temp = frente_;
		frente_ = frente_->get_Sgte();
		delete temp;

		if (frente_ == nullptr)
			final_ = nullptr;
	}

	T frente() const {
		if (esVacia()) {
			std::cout << "La cola está vacía. Retornando valor por defecto.\n";
			return T();
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
