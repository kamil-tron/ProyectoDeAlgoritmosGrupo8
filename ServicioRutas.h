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
	Lista<int> indicesAeropuertos;
	Lista<Vuelo> vuelos;
	double distanciaTotal{ 0 };
	double costoTotal{ 0 };
};

class ServicioRutas {
private:
	const double VALOR_INFINITO = 1e18;

	Grafo<PesoVuelo> grafo;
	vector<Aeropuerto> listaAeropuertos;
	ServicioVuelos& servicioVuelos;
	HashTable<string, int>* mapaCodigosAeropuerto;

	int obtenerIndiceAeropuerto(const string& codigo) {
		int* indice = mapaCodigosAeropuerto->obtener(codigo);
		if (indice) return *indice;
		int nuevoIndice = grafo.AdicionarVertice(PesoVuelo{ 0,0 });
		listaAeropuertos.push_back(Aeropuerto(codigo, 0, 0));
		mapaCodigosAeropuerto->insertar(codigo, nuevoIndice);
		return nuevoIndice;
	}

	void agregarArista(int indiceOrigen, int indiceDestino, double distancia, double costo) {
		int posArista = grafo.AdicionarArco(indiceOrigen, indiceDestino);
		grafo.ModificarArco(indiceOrigen, posArista, PesoVuelo{ distancia, costo });
	}

	double calcularDistanciaEuclidiana(const Aeropuerto& a, const Aeropuerto& b) {
		int dx = a.getX() - b.getX();
		int dy = a.getY() - b.getY();
		return sqrt(dx * dx + dy * dy);
	}

	RutaPosible calcularRutaDijkstra(int indiceOrigen, int indiceDestino, CriterioPeso criterio) {
		int totalNodos = grafo.CantidadVertices();
		vector<double> distancia(totalNodos, VALOR_INFINITO);
		vector<int> anterior(totalNodos, -1);
		vector<bool> visitado(totalNodos, false);
		distancia[indiceOrigen] = 0;

		while (true) {
			int actual = -1;
			double mejorDistancia = VALOR_INFINITO;

			for (int i = 0; i < totalNodos; i++) {
				if (!visitado[i] && distancia[i] < mejorDistancia) {
					mejorDistancia = distancia[i];
					actual = i;
				}
			}

			if (actual == -1 || actual == indiceDestino) break;

			visitado[actual] = true;
			int cantidadAristas = grafo.CantidadArcos(actual);

			for (int j = 0; j < cantidadAristas; j++) {
				int vecino = grafo.ObtenerVerticeLlegada(actual, j);
				PesoVuelo peso = grafo.ObtenerArco(actual, j);
				double valor = (criterio == CriterioPeso::DISTANCIA) ? peso.distancia : peso.costo;
				if (distancia[actual] + valor < distancia[vecino]) {
					distancia[vecino] = distancia[actual] + valor;
					anterior[vecino] = actual;
				}
			}
		}

		RutaPosible ruta;
		if (distancia[indiceDestino] == VALOR_INFINITO) return ruta;

		for (int v = indiceDestino; v != -1; v = anterior[v]) {
			ruta.indicesAeropuertos.agregaInicial(v);
		}

		Lista<Vuelo> vuelosDisponibles = servicioVuelos.listarVuelos();
		for (int i = 0; i + 1 < ruta.indicesAeropuertos.longitud(); i++) {
			int origenIdx = ruta.indicesAeropuertos.obtenerPos(i);
			int destinoIdx = ruta.indicesAeropuertos.obtenerPos(i + 1);
			const Aeropuerto& origen = listaAeropuertos[origenIdx];
			const Aeropuerto& destino = listaAeropuertos[destinoIdx];

			for (int j = 0; j < vuelosDisponibles.longitud(); j++) {
				const Vuelo& vuelo = vuelosDisponibles.obtenerPos(j);
				if (vuelo.getOrigen() == origen.getCodigo() && vuelo.getDestino() == destino.getCodigo()) {
					ruta.vuelos.agregaFinal(vuelo);
					ruta.distanciaTotal += calcularDistanciaEuclidiana(origen, destino);
					ruta.costoTotal += vuelo.getPrecio();
					break;
				}
			}
		}
		return ruta;
	}

public:
	ServicioRutas(ServicioVuelos& servicioVuelos)
		: servicioVuelos(servicioVuelos), mapaCodigosAeropuerto(new HashTable<string, int>(1000, hashString)) {
		Lista<Vuelo> vuelos = servicioVuelos.listarVuelos();
		for (int i = 0; i < vuelos.longitud(); i++) {
			const Vuelo& vuelo = vuelos.obtenerPos(i);
			int idxOrigen = obtenerIndiceAeropuerto(vuelo.getOrigen());
			int idxDestino = obtenerIndiceAeropuerto(vuelo.getDestino());
			agregarArista(idxOrigen, idxDestino, 1.0, vuelo.getPrecio());
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
