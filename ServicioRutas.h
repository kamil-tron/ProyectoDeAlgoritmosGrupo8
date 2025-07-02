#pragma once

#include <cmath>
#include <vector>
#include "Aeropuerto.h"
#include "Grafo.h"
#include "ServicioVuelos.h"
#include "RepoAeropuertos.h"
#include "HashTable.h"
#include "FuncionesHash.h"
#include "MatrizMapa.h"

#include <Windows.h>
#include <conio.h>

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
		if (prev.getFecha().empty())               // primer tramo
			return true;

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

				if (v.getOrigen() == aerO.getCodigo() &&
					v.getDestino() == aerD.getCodigo())
				{
					if (!found) {                // primer candidato
						elegido = v;
						found = true;
					}
					else if (conexionValida(elegido, v)) {
						elegido = v;            // cumple ventana - lo acepto
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

	Lista<RutaPosible> obtenerTodasLasRutas(const string& origen,
		const string& destino,
		CriterioPeso criterio)
	{
		const int MAX_RUTAS = 100;      // tope de seguridad
		Lista<RutaPosible> rutas;

		int* idxO = mapaCodigosAeropuerto->obtener(origen);
		int* idxD = mapaCodigosAeropuerto->obtener(destino);
		if (!idxO || !idxD) return rutas;

		// 1) Mejor ruta inicial
		RutaPosible base = calcularRutaDijkstra(*idxO, *idxD, criterio);
		if (base.indicesAeropuertos.esVacia()) return rutas;

		rutas.agregaFinal(base);

		// 2) Explorar rutas adicionales
		for (int idxRuta = 0; idxRuta < rutas.longitud() && rutas.longitud() < MAX_RUTAS; ++idxRuta)
		{
			const RutaPosible& rutaActual = rutas.obtenerPos(idxRuta);
			int nArcos = rutaActual.indicesAeropuertos.longitud() - 1;

			for (int posArco = 0; posArco < nArcos && rutas.longitud() < MAX_RUTAS; ++posArco)
			{
				int u = rutaActual.indicesAeropuertos.obtenerPos(posArco);
				int v = rutaActual.indicesAeropuertos.obtenerPos(posArco + 1);

				for (int j = 0; j < grafo.CantidadArcos(u) && rutas.longitud() < MAX_RUTAS; ++j)
				{
					if (grafo.ObtenerVerticeLlegada(u, j) != v) continue;

					// Copia y bloqueo temporal
					PesoVuelo original = grafo.ObtenerArco(u, j);
					PesoVuelo bloqueado = original;
					if (criterio == CriterioPeso::DISTANCIA)
						bloqueado.distancia = VALOR_INFINITO;
					else
						bloqueado.costo = VALOR_INFINITO;

					grafo.ModificarArco(u, j, bloqueado);

					// Nueva ruta
					RutaPosible nueva = calcularRutaDijkstra(*idxO, *idxD, criterio);

					// Restaurar arco
					grafo.ModificarArco(u, j, original);

					if (nueva.indicesAeropuertos.esVacia()) continue;

					bool duplicada = false;
					for (int k = 0; k < rutas.longitud(); ++k)
					{
						if (rutas.obtenerPos(k).indicesAeropuertos == nueva.indicesAeropuertos)
						{
							duplicada = true;
							break;
						}
					}
					if (!duplicada) rutas.agregaFinal(nueva);
				}
			}
		}
		return rutas;   // ya ordenadas desde la más óptima
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

	Lista<RutaPosible> todasLasRutasMasCortas(const string& origen, const string& destino) {
		return obtenerTodasLasRutas(origen, destino, CriterioPeso::DISTANCIA);
	}

	Lista<RutaPosible> todasLasRutasMasBaratas(const string& origen, const string& destino) {
		return obtenerTodasLasRutas(origen, destino, CriterioPeso::COSTO);
	}

	void pintarRutaEnMatriz(const RutaPosible& ruta,
		int valorLinea = 4,
		int valorNodo = 9)
	{
		if (ruta.indicesAeropuertos.esVacia()) return;

		// 1. Marcar aeropuertos
		for (int i = 0; i < ruta.indicesAeropuertos.longitud(); ++i) {
			int idx = ruta.indicesAeropuertos.obtenerPos(i);
			if (idx < 0 || idx >= (int)listaAeropuertos.size()) continue;

			int y = listaAeropuertos[idx].getY();  // fila
			int x = listaAeropuertos[idx].getX();  // columna

			if (y >= 0 && y < 50 && x >= 0 && x < 101)
				matrizPeru[y][x] = valorNodo;
		}

		// 2. Dibujar lineas entre aeropuertos (Bresenham)
		for (int i = 0; i + 1 < ruta.indicesAeropuertos.longitud(); ++i) {
			const Aeropuerto& a = listaAeropuertos[ruta.indicesAeropuertos.obtenerPos(i)];
			const Aeropuerto& b = listaAeropuertos[ruta.indicesAeropuertos.obtenerPos(i + 1)];

			int x0 = a.getX(), y0 = a.getY();
			int x1 = b.getX(), y1 = b.getY();

			int dx = std::abs(x1 - x0);
			int dy = -std::abs(y1 - y0);
			int sx = (x0 < x1) ? 1 : -1;
			int sy = (y0 < y1) ? 1 : -1;
			int err = dx + dy;

			while (true) {
				// No sobreescribir el nodo si ya esta marcado
				if (y0 >= 0 && y0 < 50 && x0 >= 0 && x0 < 101 &&
					matrizPeru[y0][x0] != valorNodo)
				{
					matrizPeru[y0][x0] = valorLinea;
				}

				if (x0 == x1 && y0 == y1) break;

				int e2 = 2 * err;
				if (e2 >= dy) { err += dy; x0 += sx; }
				if (e2 <= dx) { err += dx; y0 += sy; }
			}
		}
	}

};
