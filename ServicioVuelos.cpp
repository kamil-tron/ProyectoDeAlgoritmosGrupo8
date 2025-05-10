// ServicioVuelos.cpp
#include "ServicioVuelos.h"

Lista<Vuelo> ServicioVuelos::listarVuelos() const {
    return repoVuelos.cargarTodos();
}

bool ServicioVuelos::crearVuelo(const Vuelo& v) {
    // 1) Guardar la cabecera del vuelo
    repoVuelos.agregar(v);

    // 2) Generar asientos automáticamente según capacidad
    int total = v.getCapacidad();
    int count = 0;
    int fila = 1;
    char inicio = 'A';
    char fin = 'F';
    int porFila = fin - inicio + 1;

    while (count < total) {
        for (char letra = inicio; letra <= fin && count < total; ++letra) {
            // Cada asiento: (vueloId, fila, letra)
            repoAsientos.agregar(Asiento(v.getId(), fila, letra));
            ++count;
        }
        ++fila;
    }

    return true;
}

bool ServicioVuelos::modificarVuelo(const Vuelo& v) {
    // Si cambias solo la cabecera (precio, fecha, etc.), no tocamos asientos
    return repoVuelos.actualizar(v);
}

bool ServicioVuelos::eliminarVuelo(int id) {
    // 1) Eliminar vuelo
    bool ok = repoVuelos.eliminar(id);
    if (!ok) return false;
    // 2) Eliminar todos los asientos asociados
    repoAsientos.eliminarPorVuelo(id);
    return true;
}

bool ServicioVuelos::buscarVuelo(int id, Vuelo& v) const {
    return repoVuelos.buscarPorId(id, v);
}
