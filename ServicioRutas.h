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
#include "Ordenamientos.h"
#include "Lista.h"
#include "RutaPosible.h"

using namespace std;

enum class CriterioPeso { DISTANCIA, COSTO };

//
// ───────────────── Arista: ahora guarda TODOS los vuelos ──────────────────
//
struct ArcoVuelo {
    double distMin{ 0 };      // distancia mínima A→B
    double costoMin{ 0 };     // precio mínimo A→B
    Lista<int> ids;         // ids de TODOS los vuelos A→B
};


class ServicioRutas {
private:
    const double VALOR_INFINITO = 1e18;
    int  MIN_DIAS_ESCALA = 0;
    int  MAX_DIAS_ESCALA = 2;

    Grafo<ArcoVuelo> grafo;                       
    Lista<Aeropuerto> listaAeropuertos;
    ServicioVuelos     servicioVuelos;
    HashTable<string, int>* mapaCodigosAeropuerto;
    RepoAeropuertos    repoAeropuertos;

    // ───────────────────────── auxiliares internos ────────────────────────
    bool esRutaDirecta(const RutaPosible& r) const {
        return r.indicesAeropuertos.longitud() == 2;   // un solo tramo
    }
    int obtenerIndiceAeropuerto(const string& codigo) {
        int* indice = mapaCodigosAeropuerto->obtener(codigo);
        if (indice) return *indice;

        int nuevo = grafo.AdicionarVertice(ArcoVuelo{ 0,0 });
        listaAeropuertos.agregaFinal(Aeropuerto(codigo, 0, 0));
        mapaCodigosAeropuerto->insertar(codigo, nuevo);
        return nuevo;
    }

    void upsertArista(int idxO, int idxD,
        double dist, double costo, int idVuelo)
    {
        int m = grafo.CantidadArcos(idxO);
        for (int a = 0; a < m; ++a) {
            if (grafo.ObtenerVerticeLlegada(idxO, a) == idxD) {

                ArcoVuelo av = grafo.ObtenerArco(idxO, a);
                if (costo < av.costoMin) av.costoMin = costo;
                if (dist < av.distMin)  av.distMin = dist;
                av.ids.agregaFinal(idVuelo);               // agrega el vuelo
                grafo.ModificarArco(idxO, a, av);
                return;
            }
        }
        // arista nueva
        int pos = grafo.AdicionarArco(idxO, idxD);
        ArcoVuelo nuevo{ dist, costo };
        nuevo.ids.agregaFinal(idVuelo);
        grafo.ModificarArco(idxO, pos, nuevo);
    }

    double calcularDistanciaEuclidiana(const Aeropuerto& a,
        const Aeropuerto& b)
    {
        int dx = a.getX() - b.getX();
        int dy = a.getY() - b.getY();
        return sqrt(dx * dx + dy * dy);
    }

    bool conexionValida(const Vuelo& prev, const Vuelo& next) {
        if (prev.getFecha().empty()) return true;      // primer tramo
        int delta = next.getFechaSerial() - prev.getFechaSerial();
        return delta >= MIN_DIAS_ESCALA && delta <= MAX_DIAS_ESCALA;
    }

    // ────────────────────────── Dijkstra básico ───────────────────────────
    RutaPosible calcularRutaDijkstra(int idxO, int idxD,
        CriterioPeso criterio)
    {
        int n = grafo.CantidadVertices();
        vector<double> dist(n, VALOR_INFINITO);
        vector<int>    prev(n, -1);
        vector<bool>   seen(n, false);
        dist[idxO] = 0;

        // ─── Dijkstra clásico ─────────────────────────────────────
        while (true) {
            int u = -1; double best = VALOR_INFINITO;
            for (int i = 0; i < n; ++i)
                if (!seen[i] && dist[i] < best) { best = dist[i]; u = i; }
            if (u < 0 || u == idxD) break;
            seen[u] = true;

            int m = grafo.CantidadArcos(u);
            for (int j = 0; j < m; ++j) {
                int v = grafo.ObtenerVerticeLlegada(u, j);
                ArcoVuelo av = grafo.ObtenerArco(u, j);
                double cost = (criterio == CriterioPeso::DISTANCIA)
                    ? av.distMin : av.costoMin;
                if (dist[u] + cost < dist[v]) {
                    dist[v] = dist[u] + cost;
                    prev[v] = u;
                }
            }
        }

        RutaPosible ruta;
        if (dist[idxD] == VALOR_INFINITO) return ruta;

        // ─── reconstruir la secuencia de índices ─────────────────
        for (int v = idxD; v != -1; v = prev[v])
            ruta.indicesAeropuertos.agregaInicial(v);

        // ─── seleccionar vuelos tramo a tramo ────────────────────
        for (int i = 0; i + 1 < ruta.indicesAeropuertos.longitud(); ++i) {

            int iO = ruta.indicesAeropuertos.obtenerPos(i);
            int iD = ruta.indicesAeropuertos.obtenerPos(i + 1);

            // 1. localizar el arco iO→iD y copiarlo
            int mArcos = grafo.CantidadArcos(iO);
            ArcoVuelo arco; bool found = false;
            for (int a = 0; a < mArcos; ++a)
                if (grafo.ObtenerVerticeLlegada(iO, a) == iD) {
                    arco = grafo.ObtenerArco(iO, a);
                    found = true; break;
                }
            if (!found) { ruta.vuelos.limpiar(); return ruta; }

            // 2. ordenar ids del arco por fecha ascendente
            Lista<int> idsOrd = arco.ids;               // copia
            mergeSortIdsPorFechaAsc(idsOrd, servicioVuelos);

            // 3. elegir el vuelo que cumple la ventana
            Vuelo elegido; bool ok = false;
            for (int z = 0; z < idsOrd.longitud(); ++z) {
                int id = idsOrd.obtenerPos(z);
                Vuelo cand;
                if (!servicioVuelos.buscarVuelo(id, cand)) continue;
                if (conexionValida(elegido, cand)) {
                    elegido = cand; ok = true; break;
                }
            }
            if (!ok) { ruta.vuelos.limpiar(); return ruta; }

            // 4. acumular tramo
            ruta.vuelos.agregaFinal(elegido);
            ruta.distanciaTotal += arco.distMin;
            ruta.costoTotal += elegido.getPrecio();
        }
        return ruta;
    }


    // ─────────── helpers para deduplicar rutas en mejoresKRutas ───────────
    bool secuenciasIguales(const Lista<int>& a, const Lista<int>& b) const {
        if (a.longitud() != b.longitud())return false;
        for (int i = 0; i < a.longitud(); ++i)
            if (a.obtenerPos(i) != b.obtenerPos(i))return false;
        return true;
    }
    string hashRuta(const RutaPosible& r) const {
        string k;
        for (int i = 0; i < r.indicesAeropuertos.longitud(); ++i) {
            int idx = r.indicesAeropuertos.obtenerPos(i);
            // antes: listaAeropuertos[idx].getCodigo()
            k += listaAeropuertos.obtenerPos(idx).getCodigo();
            if (i + 1 < r.indicesAeropuertos.longitud()) k += '-';
        }
        return k;
    }

public:
    // ───────────────────────── constructor ─────────────────────────
 ServicioRutas(): mapaCodigosAeropuerto(new HashTable<string, int>(1000, hashString))
    {
        // 1) cargar todos los aeropuertos
        Lista<Aeropuerto> aps = repoAeropuertos.cargarTodos();
        for (int i = 0; i < aps.longitud(); ++i) {
            const Aeropuerto& a = aps.obtenerPos(i);
            int idx = grafo.AdicionarVertice(ArcoVuelo{ 0,0 });
         
            listaAeropuertos.agregaFinal(a);
            mapaCodigosAeropuerto->insertar(a.getCodigo(), idx);
        }

        // 2) cargar todos los vuelos y crear las aristas
        Lista<Vuelo> vuelos = servicioVuelos.listarVuelos();
        for (int i = 0; i < vuelos.longitud(); ++i) {
            const Vuelo& v = vuelos.obtenerPos(i);
            int* o = mapaCodigosAeropuerto->obtener(v.getOrigen());
            int* d = mapaCodigosAeropuerto->obtener(v.getDestino());
            if (!o || !d) continue;
            double dist = calcularDistanciaEuclidiana(
                listaAeropuertos.obtenerPos(*o),
                listaAeropuertos.obtenerPos(*d)
            );
            upsertArista(*o, *d, dist, v.getPrecio(), v.getId());
        }
    }
    // ────────────────────── rutas más corta / barata ─────────────────────
    RutaPosible rutaMasCorta(const string& o, const string& d) {
        int* io = mapaCodigosAeropuerto->obtener(o);
        int* id = mapaCodigosAeropuerto->obtener(d);
        if (!io || !id) return RutaPosible();
        return calcularRutaDijkstra(*io, *id, CriterioPeso::DISTANCIA);
    }
    RutaPosible rutaMasBarata(const string& o, const string& d) {
        int* io = mapaCodigosAeropuerto->obtener(o);
        int* id = mapaCodigosAeropuerto->obtener(d);
        if (!io || !id) return RutaPosible();
        return calcularRutaDijkstra(*io, *id, CriterioPeso::COSTO);
    }

    // ─────────────────────── mejores K rutas (Yen lite) ──────────────────
    Lista<RutaPosible> mejoresKRutas(const string& o,
        const string& d,
        int k)
    {
        Lista<RutaPosible> res;
        if (k <= 0) return res;

        /* hash para no repetir la misma secuencia de aeropuertos */
        HashTable<string, bool> vistos(600, hashString);

 
        Lista<CriterioPeso> crits;
        crits.agregaFinal(CriterioPeso::DISTANCIA);
        crits.agregaFinal(CriterioPeso::COSTO);

        for (int c = 0; c < crits.longitud() && res.longitud() < k; ++c) {

            /* 1. ruta base según la métrica */
            RutaPosible base =
                (crits.obtenerPos(c) == CriterioPeso::DISTANCIA)
                ? rutaMasCorta(o, d)
                : rutaMasBarata(o, d);

            /* 1-bis. si es directa, bloquear su único arco y recalcular */
            if (!base.indicesAeropuertos.esVacia() &&
                esRutaDirecta(base))
            {
                int u = base.indicesAeropuertos.obtenerPos(0);
                int v = base.indicesAeropuertos.obtenerPos(1);

                int m = grafo.CantidadArcos(u);
                for (int a = 0; a < m; ++a)
                    if (grafo.ObtenerVerticeLlegada(u, a) == v) {

                        ArcoVuelo bak = grafo.ObtenerArco(u, a);
                        grafo.ModificarArco(u, a,
                            { VALOR_INFINITO,VALOR_INFINITO });

                        base = (crits.obtenerPos(c) == CriterioPeso::DISTANCIA)
                            ? rutaMasCorta(o, d)
                            : rutaMasBarata(o, d);

                        grafo.ModificarArco(u, a, bak);
                        break;
                    }
            }

            if (base.indicesAeropuertos.esVacia()) continue;

            /* listas de trabajo */
            Lista<RutaPosible> explor;
            explor.agregaFinal(base);
            string hBase = hashRuta(base);
            vistos.insertar(hBase, true);
            if (base.vuelos.longitud() >= 2) res.agregaFinal(base);

            /* 2. Yen “lite” */
            for (int idx = 0;
                idx < explor.longitud() && res.longitud() < k;
                ++idx)
            {
                const RutaPosible& ref = explor.obtenerPos(idx);
                int n = ref.indicesAeropuertos.longitud();

                for (int p = 0; p + 1 < n && res.longitud() < k; ++p) {

                    int u = ref.indicesAeropuertos.obtenerPos(p);
                    int v = ref.indicesAeropuertos.obtenerPos(p + 1);

                    int aCnt = grafo.CantidadArcos(u);
                    for (int a = 0;
                        aCnt && a < aCnt && res.longitud() < k;
                        ++a)
                        if (grafo.ObtenerVerticeLlegada(u, a) == v) {

                            ArcoVuelo bak = grafo.ObtenerArco(u, a);
                            grafo.ModificarArco(u, a,
                                { VALOR_INFINITO,VALOR_INFINITO });

                            RutaPosible cand =
                                (crits.obtenerPos(c) == CriterioPeso::DISTANCIA)
                                ? rutaMasCorta(o, d)
                                : rutaMasBarata(o, d);

                            grafo.ModificarArco(u, a, bak);

                            if (cand.indicesAeropuertos.esVacia()
                                || esRutaDirecta(cand)) continue;

                            string h = hashRuta(cand);
                            if (vistos.obtener(h)) continue;

                            explor.agregaFinal(cand);
                            vistos.insertar(h, true);
                            if (cand.vuelos.longitud() >= 2)
                                res.agregaFinal(cand);
                        }
                }
            }
        }
        return res;
    }
    // ──────────────────────── utilidades de dibujo ───────────────────────
    void pintarRutaEnMatriz(const RutaPosible& ruta,
        int valorLinea,
        int valorNodo,
        int nodoFinal)
    {
        if (ruta.indicesAeropuertos.esVacia()) return;

        int len = ruta.indicesAeropuertos.longitud();
        // ─── marcamos nodos ─────────────────────────────────────────────────
        for (int i = 0; i < len; ++i) {
            int idx = ruta.indicesAeropuertos.obtenerPos(i);
            const Aeropuerto& A = listaAeropuertos.obtenerPos(idx);
            int y = A.getY(), x = A.getX();
            if (y >= 0 && y < 50 && x >= 0 && x < 101) {
                // si es el último nodo, usa nodoFinal; si no, valorNodo
                matrizPeru[y][x] = (i == len - 1 ? nodoFinal : valorNodo);
            }
        }

        // ─── dibujamos líneas ────────────────────────────────────────────────
        for (int i = 0; i + 1 < len; ++i) {
            const Aeropuerto& A = listaAeropuertos
                .obtenerPos(ruta.indicesAeropuertos.obtenerPos(i));
            const Aeropuerto& B = listaAeropuertos
                .obtenerPos(ruta.indicesAeropuertos.obtenerPos(i + 1));

            int x0 = A.getX(), y0 = A.getY();
            int x1 = B.getX(), y1 = B.getY();
            int dx = abs(x1 - x0), dy = -abs(y1 - y0);
            int sx = (x0 < x1) ? 1 : -1, sy = (y0 < y1) ? 1 : -1;
            int err = dx + dy;

            while (true) {
                if (y0 >= 0 && y0 < 50 && x0 >= 0 && x0 < 101 &&
                    matrizPeru[y0][x0] != valorNodo &&
                    matrizPeru[y0][x0] != nodoFinal)
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