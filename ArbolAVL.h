#pragma once
#include <algorithm>  // for std::max

template<class T>
class NodoAVL {
public:
    T elemento;
    NodoAVL* izq;
    NodoAVL* der;
    int altura;
    NodoAVL()
        : izq(nullptr), der(nullptr), altura(1) // altura inicial de la hoja
    {
    }
};

template <class T>
class ArbolAVL {
private:
    NodoAVL<T>* raiz;
    void (*procesar)(T);

    int _altura(NodoAVL<T>* nodo) {
        return nodo ? nodo->altura : 0;
    }

    void _rotarDerecha(NodoAVL<T>*& nodo) {
        NodoAVL<T>* p = nodo->izq;
        nodo->izq = p->der;
        p->der = nodo;
        // actualizar alturas desde abajo hacia arriba
        nodo->altura = 1 + std::max(_altura(nodo->izq), _altura(nodo->der));
        p->altura = 1 + std::max(_altura(p->izq), _altura(p->der));
        nodo = p;
    }

    void _rotarIzquierda(NodoAVL<T>*& nodo) {
        NodoAVL<T>* p = nodo->der;
        nodo->der = p->izq;
        p->izq = nodo;
        // actualizar alturas desde abajo hacia arriba
        nodo->altura = 1 + std::max(_altura(nodo->izq), _altura(nodo->der));
        p->altura = 1 + std::max(_altura(p->izq), _altura(p->der));
        nodo = p;
    }

    void _balanceo(NodoAVL<T>*& nodo) {
        if (!nodo) return;
        int hizq = _altura(nodo->izq);
        int hder = _altura(nodo->der);
        int fb = hder - hizq;

        // caso subarbol derecho pesado
        if (fb > 1) {
            if (_altura(nodo->der->izq) > _altura(nodo->der->der))
                _rotarDerecha(nodo->der);
            _rotarIzquierda(nodo);
        }
        // caso subarbol izquierdo pesado
        else if (fb < -1) {
            if (_altura(nodo->izq->der) > _altura(nodo->izq->izq))
                _rotarIzquierda(nodo->izq);
            _rotarDerecha(nodo);  // <-- rotar el propio nodo
        }

        // actualizar altura del nodo actual
        hizq = _altura(nodo->izq);
        hder = _altura(nodo->der);
        nodo->altura = 1 + std::max(hizq, hder);
    }

    bool _insertar(NodoAVL<T>*& nodo, const T& e) {
        if (!nodo) {
            nodo = new NodoAVL<T>();
            nodo->elemento = e;
            // altura ya inicializada en constructor
            return true;
        }
        bool inserted = false;
        if (e < nodo->elemento) {
            inserted = _insertar(nodo->izq, e);
        }
        else if (e > nodo->elemento) {
            inserted = _insertar(nodo->der, e);
        }
        else {
            return false;  // duplicado
        }
        if (inserted) {
            _balanceo(nodo);
        }
        return inserted;
    }

    void _inOrden(NodoAVL<T>* nodo) {
        if (!nodo) return;
        _inOrden(nodo->izq);
        procesar(nodo->elemento);
        _inOrden(nodo->der);
    }

public:
    explicit ArbolAVL(void (*fn)(T))
        : raiz(nullptr), procesar(fn)
    {
    }

    bool insertar(const T& e) {
        return _insertar(raiz, e);
    }

    void inOrden() {
        _inOrden(raiz);
    }
};
