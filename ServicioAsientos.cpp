#include "ServicioAsientos.h"

const double ServicioAsientos::VIP_SURCHARGE = 0.20;

ServicioAsientos::ServicioAsientos()
    : idx(new HashTable<string, Asiento>(8000, hashString)) {
    cargarIndice();
}

ServicioAsientos::~ServicioAsientos() {
    delete idx;
}

void ServicioAsientos::cargarIndice() {
    auto vuelos = repoVuelos.cargarTodos();
    for (int i = 0; i < vuelos.longitud(); ++i) {
        int vId = vuelos.obtenerPos(i).getId();
        auto lista = repoAsientos.listarPorVuelo(vId);
        for (int j = 0; j < lista.longitud(); ++j) {
            const Asiento& a = lista.obtenerPos(j);
            idx->insertar(makeKey(vId, a.getFila(), a.getLetra()), a);
        }
    }
}

bool ServicioAsientos::listarDisponibles(int vueloId, Lista<Asiento>& out) const {
    auto lista = repoAsientos.listarPorVuelo(vueloId);
    for (int i = 0; i < lista.longitud(); ++i)
        if (!lista.obtenerPos(i).isOcupado())
            out.agregaFinal(lista.obtenerPos(i));
    return !out.esVacia();
}

bool ServicioAsientos::precioAsiento(int vueloId, const string& codigo, double& precio) const {
    Vuelo v;
    if (!repoVuelos.buscarPorId(vueloId, v)) return false;
    int fila = stoi(codigo.substr(0, codigo.size() - 1));
    char letra = codigo.back();
    const Asiento* a = idx->obtener(makeKey(vueloId, fila, letra));
    if (!a) return false;
    precio = v.getPrecio();
    if (a->isVip()) precio *= (1.0 + VIP_SURCHARGE);
    return true;
}

double ServicioAsientos::totalSeleccion(int vueloId, const Lista<string>& codigos) const {
    double total = 0.0, p = 0.0;
    for (int i = 0; i < codigos.longitud(); ++i) {
        if (!precioAsiento(vueloId, codigos.obtenerPos(i), p)) return 0.0;
        total += p;
    }
    return total;
}

bool ServicioAsientos::ocupar(int vueloId, const Lista<string>& codigos) {
    for (int i = 0; i < codigos.longitud(); ++i) {
        int fila = stoi(codigos.obtenerPos(i).substr(0, codigos.obtenerPos(i).size() - 1));
        char letra = codigos.obtenerPos(i).back();
        Asiento* a = idx->obtener(makeKey(vueloId, fila, letra));
        if (!a || a->isOcupado()) return false;
        a->setOcupado(true);
        repoAsientos.actualizar(*a);
    }
    return true;
}

bool ServicioAsientos::liberar(int vueloId, const Lista<string>& codigos) {
    for (int i = 0; i < codigos.longitud(); ++i) {
        int fila = stoi(codigos.obtenerPos(i).substr(0, codigos.obtenerPos(i).size() - 1));
        char letra = codigos.obtenerPos(i).back();
        Asiento* a = idx->obtener(makeKey(vueloId, fila, letra));
        if (!a) return false;
        a->setOcupado(false);
        repoAsientos.actualizar(*a);
    }
    return true;
}
