#include "ServicioVuelos.h"

Lista<Vuelo> ServicioVuelos::listarVuelos() const {
    return repoVuelos.cargarTodos();
}

bool ServicioVuelos::crearVuelo(const Vuelo& v) {
    repoVuelos.agregar(v);

    int total = v.getCapacidad();
    int count = 0;
    int fila = 1;
    char inicio = 'A';
    char fin = 'F';
    int porFila = fin - inicio + 1;

    while (count < total) {
        for (char letra = inicio; letra <= fin && count < total; ++letra) {
            repoAsientos.agregar(Asiento(v.getId(), fila, letra));
            ++count;
        }
        ++fila;
    }

    return true;
}

bool ServicioVuelos::modificarVuelo(const Vuelo& v) {
    return repoVuelos.actualizar(v);
}

bool ServicioVuelos::eliminarVuelo(int id) {
    bool ok = repoVuelos.eliminar(id);
    if (!ok) return false;
    repoAsientos.eliminarPorVuelo(id);
    return true;
}

bool ServicioVuelos::buscarVuelo(int id, Vuelo& v) const {
    return repoVuelos.buscarPorId(id, v);
}
