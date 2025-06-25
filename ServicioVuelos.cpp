#include "ServicioVuelos.h"
#include "Vuelo.h"
#include "Asiento.h"
#include "RepoAsientos.h"
#include "FuncionesHash.h"

static const double VIP_SURCHARGE = 0.20;

ServicioVuelos::ServicioVuelos()
    : idx(new HashTable<int, Vuelo>(2000, hashInt)) {
    cargarIndice();
}

ServicioVuelos::~ServicioVuelos() {
    delete idx;
}

void ServicioVuelos::cargarIndice() {
    auto lista = repoVuelos.cargarTodos();
    for (int i = 0; i < lista.longitud(); ++i)
        idx->insertar(lista.obtenerPos(i).getId(), lista.obtenerPos(i));
}

Lista<Vuelo> ServicioVuelos::listarVuelos() const {
    return repoVuelos.cargarTodos();
}

bool ServicioVuelos::buscarVuelo(int id, Vuelo& v) const {
    const Vuelo* p = idx->obtener(id);
    if (!p) return false;
    v = *p;
    return true;
}

bool ServicioVuelos::crearVuelo(const Vuelo& v) {
    repoVuelos.agregar(v);
    idx->insertar(v.getId(), v);

    int total = v.getCapacidad();
    int cutoff = total / 4;
    int count = 0, fila = 1;

    while (count < total) {
        for (char letra = 'A'; letra <= 'F' && count < total; ++letra) {
            bool vip = (count < cutoff);
            repoAsientos.agregar(Asiento(v.getId(), fila, letra, false, vip));
            ++count;
        }
        ++fila;
    }
    return true;
}

bool ServicioVuelos::modificarVuelo(const Vuelo& v) {
    repoVuelos.actualizar(v);
    idx->eliminar(v.getId());
    idx->insertar(v.getId(), v);
    return true;
}

bool ServicioVuelos::eliminarVuelo(int id) {
    repoVuelos.eliminar(id);
    idx->eliminar(id);
    repoAsientos.eliminarPorVuelo(id);
    return true;
}
