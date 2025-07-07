#pragma once
#include "HashEntidad.h"
#include <iostream>
using namespace std;

//Hash table con templates
template<typename K, typename T>
class HashTable {
private:
    HashEntidad<K, T>** tabla;
    int numElementos;
    int TABLE_SIZE;

    int (*hashFunc)(const K&, int);

public:
    HashTable(int size, int (*hashFunction)(const K&, int)) {
        TABLE_SIZE = size;
        numElementos = 0;
        hashFunc = hashFunction;
        tabla = new HashEntidad<K, T>* [TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++) {
            tabla[i] = nullptr;
        }
    }

    ~HashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            delete tabla[i];
        }
        delete[] tabla;
    }

    void insertar(K clave, T valor) {
        if (numElementos == TABLE_SIZE) return;

        int base = hashFunc(clave, TABLE_SIZE);
        int hash = base;
        int step = 0;

        while (tabla[hash] != nullptr) {
            hash = (base + step) % TABLE_SIZE;
            step++;
        }

        tabla[hash] = new HashEntidad   <K, T>(clave, valor);
        numElementos++;
    }

    int buscar(const K& clave) const {
        int base = hashFunc(clave, TABLE_SIZE);
        int i = base;
        int step = 0;

        while (true) {
            if (tabla[i] == nullptr) return -1;
            if (tabla[i]->getClave() == clave) return i;
            step++;
            i = (base + step) % TABLE_SIZE;
        }
    }

    bool eliminar(K clave) {
        int base = hashFunc(clave, TABLE_SIZE);
        int hash = base;
        int step = 0;

        while (true) {
            if (tabla[hash] == nullptr) return false;

            if (!tabla[hash]->estaEliminado() && tabla[hash]->getClave() == clave) {
                tabla[hash]->marcarEliminado();
                numElementos--;
                return true;
            }

            step++;
            hash = (base + step) % TABLE_SIZE;

            if (step >= TABLE_SIZE) return false;
        }
    }

    bool contiene(const K& clave) const {
        int idx = buscar(clave);
        return idx != -1 && !tabla[idx]->estaEliminado();
    }

    T* obtener(const K& clave) {
        int idx = buscar(clave);
        if (idx == -1 || tabla[idx]->estaEliminado()) return nullptr;
        return &(tabla[idx]->getValor());
    }

    const T* obtener(const K& clave) const {
        int idx = buscar(clave);
        if (idx == -1 || tabla[idx]->estaEliminado()) return nullptr;
        return &(tabla[idx]->getValor());
    }

    int size() const { return TABLE_SIZE; }
    int sizeActual() const { return numElementos; }
};
