#include "ServicioVuelos.h"

static Lista<Vuelo>* listaTemporal = nullptr;

static void insertarEnListaTemporal(Vuelo v) {
	if (listaTemporal) listaTemporal->agregaFinal(v);
}

ServicioVuelos::ServicioVuelos()
	: idx(new HashTable<int, Vuelo>(2000, hashInt)),
	vuelosPorFecha(new ArbolAVL<Vuelo>(insertarEnListaTemporal)) {
	cargarIndice();
}

ServicioVuelos::~ServicioVuelos() {
	delete idx;
	delete vuelosPorFecha;
}

void ServicioVuelos::cargarIndice() {
	auto lista = repoVuelos.cargarTodos();
	for (int i = 0; i < lista.longitud(); ++i) {
		Vuelo v = lista.obtenerPos(i);
		idx->insertar(v.getId(), v);
		vuelosPorFecha->insertar(v);
	}
}

Lista<Vuelo> ServicioVuelos::listarVuelos() const {
	return repoVuelos.cargarTodos();
}

Lista<Vuelo> ServicioVuelos::listarVuelosPorFecha() const {

	Lista<Vuelo> ordenados;

	listaTemporal = &ordenados;
	vuelosPorFecha->inOrden();
	listaTemporal = nullptr;

	return ordenados;
}

bool ServicioVuelos::buscarVuelo(int id, Vuelo& v) const {
	const Vuelo* p = idx->obtener(id);
	if (!p) return false;
	v = *p;
	return true;
}

bool ServicioVuelos::crearVuelo(const Vuelo& v) {
	if (!repoAeropuertos.existe(v.getOrigen()) ||
		!repoAeropuertos.existe(v.getDestino()))
	{
		cout << "Error: aeropuerto no registrado ("
			<< v.getOrigen() << " o " << v.getDestino() << ").\n";
		return false;
	}

	repoVuelos.agregar(v);
	idx->insertar(v.getId(), v);
	vuelosPorFecha->insertar(v);

	int total = v.getCapacidad();
	int vipCut = total / 4;
	int count = 0;
	int fila = 1;

	while (count < total) {
		for (char letra = 'A'; letra <= 'F' && count < total; ++letra) {
			bool vip = (count < vipCut);
			repoAsientos.agregar(Asiento(v.getId(), fila, letra, false, vip));
			++count;
		}
		++fila;
	}
	return true;
}

bool ServicioVuelos::modificarVuelo(const Vuelo& v) {
	if (!repoAeropuertos.existe(v.getOrigen()) ||
		!repoAeropuertos.existe(v.getDestino()))
	{
		cout << "Error: aeropuerto no registrado ("
			<< v.getOrigen() << " o " << v.getDestino() << ").\n";
		return false;
	}

	Vuelo viejo;
	if (!buscarVuelo(v.getId(), viejo)) {
		cout << "Error: vuelo no encontrado.\n";
		return false;
	}

	repoVuelos.actualizar(v);
	idx->eliminar(v.getId());
	idx->insertar(v.getId(), v);

	delete vuelosPorFecha;
	vuelosPorFecha = new ArbolAVL<Vuelo>(insertarEnListaTemporal);
	cargarIndice();

	return true;
}

bool ServicioVuelos::eliminarVuelo(int id) {
	repoVuelos.eliminar(id);
	idx->eliminar(id);
	repoAsientos.eliminarPorVuelo(id);

	delete vuelosPorFecha;
	vuelosPorFecha = new ArbolAVL<Vuelo>(insertarEnListaTemporal);
	cargarIndice();

	return true;
}

Lista<Aeropuerto> ServicioVuelos::listarAeropuertos() const {
	return repoAeropuertos.cargarTodos();
}