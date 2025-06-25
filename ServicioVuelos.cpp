#include "ServicioVuelos.h"
#include "Vuelo.h"
#include "Asiento.h"
#include "RepoAsientos.h"

static const double VIP_SURCHARGE = 0.20;

Lista<Vuelo> ServicioVuelos::listarVuelos() const {
    return repoVuelos.cargarTodos();
}

bool ServicioVuelos::crearVuelo(const Vuelo& v) {
    // 1) Guarda el vuelo
    repoVuelos.agregar(v);

    // 2) Prepara la generación de asientos
    int total = v.getCapacidad();
    int cutoff = total / 4;          // primeros 25% ? VIP
    int count = 0;
    int fila = 1;
    char inicio = 'A';
    char fin = 'F';

    // 3) Crea todos los asientos, marcando vip según `count < cutoff`
    while (count < total) {
        for (char letra = inicio; letra <= fin && count < total; ++letra) {
            bool vip = (count < cutoff);
            // Asiento(vueloId, fila, letra, ocupado=false, vip)
            repoAsientos.agregar(Asiento(v.getId(), fila, letra, /*ocupado=*/false, vip));
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