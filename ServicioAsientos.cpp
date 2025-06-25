#include "ServicioAsientos.h"

using namespace std;

const double ServicioAsientos::VIP_SURCHARGE = 0.20;

bool ServicioAsientos::listarDisponibles(int vueloId, Lista<Asiento>& out) const {
    auto todos = repoAsientos.listarPorVuelo(vueloId);
    for (int i = 0; i < todos.longitud(); ++i) {
        if (!todos.obtenerPos(i).isOcupado())
            out.agregaFinal(todos.obtenerPos(i));
    }
    return !out.esVacia();
}

bool ServicioAsientos::precioAsiento(int vueloId, const string& codigo, double& precio) const {
    Vuelo v;
    if (!repoVuelos.buscarPorId(vueloId, v)) return false;
    int fila = stoi(codigo.substr(0, codigo.size() - 1));
    char letra = codigo.back();
    Asiento a;
    if (!repoAsientos.buscar(vueloId, fila, letra, a)) return false;
    precio = v.getPrecio();
    if (a.isVip()) precio *= (1.0 + VIP_SURCHARGE);
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
        Asiento a;
        if (!repoAsientos.buscar(vueloId, fila, letra, a) || a.isOcupado()) return false;
        a.setOcupado(true);
        repoAsientos.actualizar(a);
    }
    return true;
}

bool ServicioAsientos::liberar(int vueloId, const Lista<string>& codigos) {
    for (int i = 0; i < codigos.longitud(); ++i) {
        int fila = stoi(codigos.obtenerPos(i).substr(0, codigos.obtenerPos(i).size() - 1));
        char letra = codigos.obtenerPos(i).back();
        Asiento a;
        if (!repoAsientos.buscar(vueloId, fila, letra, a)) return false;
        a.setOcupado(false);
        repoAsientos.actualizar(a);
    }
    return true;
}
