#pragma once

#include <cmath>
#include <vector>
#include "Aeropuerto.h"
#include "Grafo.h"
#include "ServicioVuelos.h"
#include "RepoAeropuertos.h"
#include "HashTable.h"
#include "FuncionesHash.h"

using namespace std;

enum class CriterioPeso { DISTANCIA, COSTO };

struct PesoVuelo {
	double distancia;
	double costo;
};

struct RutaPosible {
	Lista<int> indicesAeropuertos;
	Lista<Vuelo> vuelos;
	double distanciaTotal{ 0 };
	double costoTotal{ 0 };
};

class ServicioRutas {
private:
	const double VALOR_INFINITO = 1e18;
	int MIN_DIAS_ESCALA = 0;
	int MAX_DIAS_ESCALA = 1;

	Grafo<PesoVuelo> grafo;
	vector<Aeropuerto> listaAeropuertos;
	ServicioVuelos servicioVuelos;
	HashTable<string, int>* mapaCodigosAeropuerto;
	RepoAeropuertos  repoAeropuertos;

	int obtenerIndiceAeropuerto(const string& codigo) {
		int* indice = mapaCodigosAeropuerto->obtener(codigo);
		if (indice) return *indice;
		int nuevoIndice = grafo.AdicionarVertice(PesoVuelo{ 0,0 });
		listaAeropuertos.push_back(Aeropuerto(codigo, 0, 0));
		mapaCodigosAeropuerto->insertar(codigo, nuevoIndice);
		return nuevoIndice;
	}

	void upsertArista(int idxO, int idxD, double dist, double costo)
	{
		int m = grafo.CantidadArcos(idxO);
		for (int a = 0; a < m; ++a) {
			if (grafo.ObtenerVerticeLlegada(idxO, a) == idxD) {
				PesoVuelo pv = grafo.ObtenerArco(idxO, a);
				if (costo < pv.costo)       pv.costo = costo;
				if (dist < pv.distancia)   pv.distancia = dist;
				grafo.ModificarArco(idxO, a, pv);
				return;
			}
		}
		// Arista nueva
		int pos = grafo.AdicionarArco(idxO, idxD);
		grafo.ModificarArco(idxO, pos, PesoVuelo{ dist, costo });
	}

	double calcularDistanciaEuclidiana(const Aeropuerto& a, const Aeropuerto& b) {
		int dx = a.getX() - b.getX();
		int dy = a.getY() - b.getY();
		return sqrt(dx * dx + dy * dy);
	}
	
	bool conexionValida(const Vuelo& prev, const Vuelo& next) {
		int delta = next.getFechaSerial() - prev.getFechaSerial();
		return delta >= MIN_DIAS_ESCALA && delta <= MAX_DIAS_ESCALA;
	}

	RutaPosible calcularRutaDijkstra(int idxO, int idxD, CriterioPeso criterio) {
		int n = grafo.CantidadVertices();
		vector<double> dist(n, VALOR_INFINITO);
		vector<int> prev(n, -1);
		vector<bool> seen(n, false);
		dist[idxO] = 0;

		// Dijkstra
		while (true) {
			int u = -1;
			double best = VALOR_INFINITO;
			for (int i = 0; i < n; ++i) {
				if (!seen[i] && dist[i] < best) {
					best = dist[i]; u = i;
				}
			}
			if (u < 0 || u == idxD) break;
			seen[u] = true;
			int m = grafo.CantidadArcos(u);
			for (int j = 0; j < m; ++j) {
				int v = grafo.ObtenerVerticeLlegada(u, j);
				PesoVuelo pv = grafo.ObtenerArco(u, j);
				double cost = (criterio == CriterioPeso::DISTANCIA) ? pv.distancia : pv.costo;
				if (dist[u] + cost < dist[v]) {
					dist[v] = dist[u] + cost;
					prev[v] = u;
				}
			}
		}

		RutaPosible ruta;
		if (dist[idxD] == VALOR_INFINITO) return ruta;

		// Reconstruir índices
		for (int v = idxD; v != -1; v = prev[v])
			ruta.indicesAeropuertos.agregaInicial(v);

		// Seleccionar vuelos con validación de escalas
		Lista<Vuelo> todos = servicioVuelos.listarVuelos();
		int lastDate = -1;
		for (int i = 0; i + 1 < ruta.indicesAeropuertos.longitud(); ++i) {
			int iO = ruta.indicesAeropuertos.obtenerPos(i);
			int iD = ruta.indicesAeropuertos.obtenerPos(i + 1);
			const auto& aerO = listaAeropuertos[iO];
			const auto& aerD = listaAeropuertos[iD];

			bool found = false;
			Vuelo elegido;
			for (int k = 0; k < todos.longitud(); ++k) {
				const Vuelo& v = todos.obtenerPos(k);
				if (v.getOrigen() == aerO.getCodigo() && v.getDestino() == aerD.getCodigo()) {
					if (lastDate < 0 || conexionValida(elegido, v)) {
						elegido = v;
						found = true;
						break;
					}
				}
			}
			if (!found) { ruta.vuelos.limpiar(); return ruta; }

			ruta.vuelos.agregaFinal(elegido);
			ruta.distanciaTotal += calcularDistanciaEuclidiana(aerO, aerD);
			ruta.costoTotal += elegido.getPrecio();
			lastDate = elegido.getFechaSerial();
		}
		return ruta;
	}
public:
	ServicioRutas()
		: mapaCodigosAeropuerto(new HashTable<string, int>(1000, hashString))
	{
		Lista<Aeropuerto> aps = repoAeropuertos.cargarTodos();
		for (int i = 0; i < aps.longitud(); ++i) {
			const Aeropuerto& a = aps.obtenerPos(i);
			int idx = grafo.AdicionarVertice(PesoVuelo{ 0,0 });
			listaAeropuertos.push_back(a);
			mapaCodigosAeropuerto->insertar(a.getCodigo(), idx);
		}

		Lista<Vuelo> vuelos = servicioVuelos.listarVuelos();
		for (int i = 0; i < vuelos.longitud(); ++i) {
			const Vuelo& v = vuelos.obtenerPos(i);
			int* o = mapaCodigosAeropuerto->obtener(v.getOrigen());
			int* d = mapaCodigosAeropuerto->obtener(v.getDestino());
			if (!o || !d) continue;
			double distReal = calcularDistanciaEuclidiana(
				listaAeropuertos[*o], listaAeropuertos[*d]);
			upsertArista(*o, *d, distReal, v.getPrecio());
		}
	}

	~ServicioRutas() {
		delete mapaCodigosAeropuerto;
	}

	RutaPosible rutaMasCorta(const string& origen, const string& destino) {
		int* idxOrigen = mapaCodigosAeropuerto->obtener(origen);
		int* idxDestino = mapaCodigosAeropuerto->obtener(destino);
		if (!idxOrigen || !idxDestino) return RutaPosible();
		return calcularRutaDijkstra(*idxOrigen, *idxDestino, CriterioPeso::DISTANCIA);
	}

	RutaPosible rutaMasBarata(const string& origen, const string& destino) {
		int* idxOrigen = mapaCodigosAeropuerto->obtener(origen);
		int* idxDestino = mapaCodigosAeropuerto->obtener(destino);
		if (!idxOrigen || !idxDestino) return RutaPosible();
		return calcularRutaDijkstra(*idxOrigen, *idxDestino, CriterioPeso::COSTO);
	}

	Lista<RutaPosible> mejoresKRutas(const string& origen, const string& destino, int cantidad) {
		Lista<RutaPosible> rutas;
		if (cantidad <= 0) return rutas;

		RutaPosible rutaInicial = rutaMasCorta(origen, destino);
		if (rutaInicial.indicesAeropuertos.esVacia()) return rutas;

		rutas.agregaFinal(rutaInicial);

		for (int i = 1; i < cantidad; i++) {
			const RutaPosible& rutaAnterior = rutas.obtenerFinal();
			int longitud = rutaAnterior.indicesAeropuertos.longitud();
			if (longitud < 2) break;

			int penultimo = rutaAnterior.indicesAeropuertos.obtenerPos(longitud - 2);
			int ultimo = rutaAnterior.indicesAeropuertos.obtenerFinal();

			int cantidadArcos = grafo.CantidadArcos(penultimo);
			bool seBloqueo = false;

			for (int j = 0; j < cantidadArcos; j++) {
				if (grafo.ObtenerVerticeLlegada(penultimo, j) == ultimo) {
					PesoVuelo pesoOriginal = grafo.ObtenerArco(penultimo, j);
					PesoVuelo pesoBloqueado = pesoOriginal;
					pesoBloqueado.distancia = pesoBloqueado.costo = VALOR_INFINITO;
					grafo.ModificarArco(penultimo, j, pesoBloqueado);

					RutaPosible nuevaRuta = rutaMasCorta(origen, destino);
					if (!nuevaRuta.indicesAeropuertos.esVacia()) rutas.agregaFinal(nuevaRuta);

					grafo.ModificarArco(penultimo, j, pesoOriginal);
					seBloqueo = true;
					break;
				}
			}
			if (!seBloqueo) break;
		}
		return rutas;
	}
};
