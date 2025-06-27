#pragma once
#include "RepoAeropuertos.h"

class ServicioAeropuertos {
private:
    RepoAeropuertos repo;
public:
    bool crear(const Aeropuerto& a) { return repo.agregar(a); }
    Lista<Aeropuerto> listar()  const { return repo.cargarTodos(); }
    bool existe(const string& cod) { return repo.existe(cod); }
};