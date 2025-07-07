#pragma once
#include "Nodo.h"
#include <stdexcept>  // para out_of_range

template <typename T>
class Pila {
private:
    Nodo<T>* cima_ = nullptr;
    int tamanio_ = 0;

public:
    Pila() = default;
    ~Pila();

    void apilar(const T& elem);     // push
    void desapilar();               // pop
    T cima() const;                 // top
    bool esVacia() const;
    int tamanio() const;

    // compatibilidad con size()/at() en el menú:
    int size() const { return tamanio_; }

    // Acceso al elemento i-ésimo (0 = cima, 1 = siguiente, …)
    const T& at(int index) const {
        if (index < 0 || index >= tamanio_)
            throw std::out_of_range("Pila::at");
        Nodo<T>* p = cima_;
        while (index-- > 0) {
            p = p->get_Sgte();
        }
        return p->get_Elem();
    }
};

// Destructor
template <typename T>
Pila<T>::~Pila() {
    while (!esVacia())
        desapilar();
}

template <typename T>
void Pila<T>::apilar(const T& elem) {
    cima_ = new Nodo<T>(elem, cima_);
    ++tamanio_;
}

template <typename T>
void Pila<T>::desapilar() {
    if (esVacia()) return;
    Nodo<T>* tmp = cima_;
    cima_ = cima_->get_Sgte();
    delete tmp;
    --tamanio_;
}

template <typename T>
T Pila<T>::cima() const {
    if (esVacia()) return T(); // Devuelve valor por defecto si está vacía
    return cima_->get_Elem();
}

template <typename T>
bool Pila<T>::esVacia() const {
    return cima_ == nullptr;
}

template <typename T>
int Pila<T>::tamanio() const {
    return tamanio_;
}
