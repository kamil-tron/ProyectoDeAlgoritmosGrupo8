#pragma once
#include "FileManager.h"
#include "Administrador.h"
#include "Lista.h"
#include <string>

using namespace std;

class RepoAdmins : public FileManager<Administrador> {
public:
	RepoAdmins()
		: FileManager("administradores.txt") {
	}

	bool buscarPorCorreo(const string& correo, Administrador& a) const {
		auto items = cargarTodos();
		for (int i = 0; i < items.longitud(); ++i) {
			Administrador tmp = items.obtenerPos(i);
			if (tmp.getCorreo() == correo) {
				a = tmp;
				return true;
			}
		}
		return false;
	}
};