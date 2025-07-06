#pragma once
#include "Cola.h"
#include "Lista.h"   // tu clase Lista<T>

template<class T>
class NodoArbol {
public:
    T elemento;
    NodoArbol* izq = nullptr;
    NodoArbol* der = nullptr;
};

template<class T>
class ArbolB {
private:
    NodoArbol<T>* raiz = nullptr;

    bool _insertar(NodoArbol<T>*& nodo, const T& e) {
        if (!nodo) {
            nodo = new NodoArbol<T>();
            nodo->elemento = e;
            return true;
        }
        if (e < nodo->elemento)
            return _insertar(nodo->izq, e);
        else
            return _insertar(nodo->der, e);
    }

    bool _buscar(NodoArbol<T>* nodo, const T& e) const {
        if (!nodo) return false;
        if (e == nodo->elemento) return true;
        return (e < nodo->elemento)
            ? _buscar(nodo->izq, e)
            : _buscar(nodo->der, e);
    }

    void _enOrden(NodoArbol<T>* nodo, Lista<T>& out) const {
        if (!nodo) return;
        _enOrden(nodo->izq, out);
        out.agregaFinal(nodo->elemento);
        _enOrden(nodo->der, out);
    }

    void _preOrden(NodoArbol<T>* nodo, Lista<T>& out) const {
        if (!nodo) return;
        out.agregaFinal(nodo->elemento);
        _preOrden(nodo->izq, out);
        _preOrden(nodo->der, out);
    }

    void _postOrden(NodoArbol<T>* nodo, Lista<T>& out) const {
        if (!nodo) return;
        _postOrden(nodo->izq, out);
        _postOrden(nodo->der, out);
        out.agregaFinal(nodo->elemento);
    }

    void _porNivel(NodoArbol<T>* nodo, Lista<T>& out) const {
        if (!nodo) return;
        Cola<NodoArbol<T>*> cola;
        cola.encolar(nodo);
        while (!cola.estaVacia()) {
            NodoArbol<T>* actual = cola.desencolar();
            out.agregaFinal(actual->elemento);
            if (actual->izq) cola.encolar(actual->izq);
            if (actual->der) cola.encolar(actual->der);
        }
    }

    int _cantidad(NodoArbol<T>* nodo) const {
        if (!nodo) return 0;
        return 1 + _cantidad(nodo->izq) + _cantidad(nodo->der);
    }

    int _altura(NodoArbol<T>* nodo) const {
        if (!nodo) return 0;
        int ai = 1 + _altura(nodo->izq);
        int ad = 1 + _altura(nodo->der);
        return ai > ad ? ai : ad;
    }

public:
    ArbolB() = default;

    bool insertar(const T& e) {
        return _insertar(raiz, e);
    }

    bool buscar(const T& e) const {
        return _buscar(raiz, e);
    }

    Lista<T> enOrden() const {
        Lista<T> res;
        _enOrden(raiz, res);
        return res;
    }

    Lista<T> preOrden() const {
        Lista<T> res;
        _preOrden(raiz, res);
        return res;
    }

    Lista<T> postOrden() const {
        Lista<T> res;
        _postOrden(raiz, res);
        return res;
    }

    Lista<T> porNivel() const {
        Lista<T> res;
        _porNivel(raiz, res);
        return res;
    }

    int cantidad() const {
        return _cantidad(raiz);
    }

    int altura() const {
        return _altura(raiz);
    }
};
