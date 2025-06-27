#pragma once

#include <cmath>
#include <vector>
#include "Aeropuerto.h"
#include "Grafo.h"
#include "ServicioVuelos.h"
#include "HashTable.h"
#include "FuncionesHash.h"

using namespace std;

enum class CriterioPeso { DISTANCIA, COSTO };

struct PesoVuelo {
	double distancia;
	double costo;
};

struct RutaPosible {
	Lista<int> vertices;
	Lista<Vuelo> vuelos;
	double distanciaTotal{ 0 };
	double costoTotal{ 0 };
};

class ServicioRutas {
private:
	const double INF_VALUE = 1e18;

	Grafo<PesoVuelo> g;
	vector<Aeropuerto> aeropuertos;
	ServicioVuelos& svcVuelos;
	HashTable<string, int>* idexAeropuertos;

	int obtenerIndice(const std::string& codigo) {
		int* p = idexAeropuertos->obtener(codigo);
		if (p) return *p;
		int idx = g.AdicionarVertice(PesoVuelo{ 0,0 });
		aeropuertos.push_back(Aeropuerto(codigo, 0, 0));
		idexAeropuertos->insertar(codigo, idx);
		return idx;
	}

	void agregarArco(int idxOri, int idxDst, double dist, double costo) {
		int pos = g.AdicionarArco(idxOri, idxDst);
		g.ModificarArco(idxOri, pos, PesoVuelo{ dist, costo });
	}

	double distanciaEuclidiana(const Aeropuerto& a, const Aeropuerto& b) {
		int dx = a.getX() - b.getX();
		int dy = a.getY() - b.getY();
		return sqrt((dx * dx + dy * dy) * 1.0);
	}

	RutaPosible dijkstra(int s, int t, CriterioPeso criterio)  {
		int n = g.CantidadVertices();
		vector<double> dist(n, INF_VALUE);
		vector<int> prev(n, -1);
		vector<bool> visitado(n, false);
		dist[s] = 0;

		while (true) {
			int u = -1; double best = INF_VALUE;
			for (int i = 0; i < n; i++)
			{
				if(!visitado[i] && dist[i] < best) {
					best = dist[i];
					u = i;
				}
			}

			if (u == -1 || u == t) break; // No hay más nodos o llegamos al destino
			visitado[u] = true;

			int m = g.CantidadArcos(u);
			for (int k = 0; k < m; k++)
			{
				int v = g.ObtenerVerticeLlegada(u, k);
				PesoVuelo peso = g.ObtenerArco(u, k);
				double w = (criterio == CriterioPeso::DISTANCIA) ? peso.distancia : peso.costo;
				if (dist[u] + w < dist[v])
				{
					dist[v] = dist[u] + w;
					prev[v] = u;
				}
			}
		}

		RutaPosible ruta;
		if (dist[t] == INF_VALUE) return ruta; // No hay ruta
		for (int v = t; v != -1; v = prev[v])
			ruta.vertices.agregaInicial(v);
	}
};