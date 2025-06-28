#pragma once
#include <vector>

using namespace std;

template <class T>
class Grafo {
private:
    class CArco {
    public:
        T info;
        int v; // índice del vértice de llegada
        CArco(int vLlegada) {
            info = T();
            v = vLlegada;
        }
    };

    class CVertice {
    public:
        T info;
        vector<CArco*>* ady; // Lista de adyacencia
        CVertice() {
            info = T();
            ady = new vector<CArco*>();
        }
    };

    vector<CVertice*>* vertices;

public:
    Grafo() { vertices = new vector<CVertice*>(); }

    int AdicionarVertice(T info) {
        CVertice* vert = new CVertice();
        vert->info = info;
        vertices->push_back(vert);
        return vertices->size() - 1;
    }

    int CantidadVertices() { return vertices->size(); }

    T ObtenerVertice(int v) { return (vertices->at(v))->info; }

    void ModificarVertice(int v, T info) { (vertices->at(v))->info = info; }

    int AdicionarArco(int v, int vLlegada) {
        CVertice* ver = vertices->at(v);   // corregido de "vertice" a "vertices"
        CArco* arco = new CArco(vLlegada); // se usa la clase CArco como pediste
        ver->ady->push_back(arco);
        return ver->ady->size() - 1;
    }

    int CantidadArcos(int v) { return (vertices->at(v))->ady->size(); }

    T ObtenerArco(int v, int apos) {
        CVertice* vert = vertices->at(v);
        return (vert->ady->at(apos))->info;
    }

    void ModificarArco(int v, int apos, T info) {
        CVertice* vert = vertices->at(v);
        (vert->ady->at(apos))->info = info;
    }

    int ObtenerVerticeLlegada(int v, int apos) {
        CVertice* vert = vertices->at(v);
        return (vert->ady->at(apos))->v;
    }
};