#pragma once

#include "UserEntity.h"
#include <string>
#include <sstream>
#include <iostream>

using namespace std;

class Administrador final : public UserEntity {
public:
	Administrador() = default;
	Administrador(string correo, string pass)
		: UserEntity(correo, pass) {
	}

	RolEnum getRol() const override {
		return RolEnum::ADMIN;
	}

	string serialize() const {
		ostringstream oss;
		oss << correo << ',' << contrasena;
		return oss.str();
	}

	static Administrador fromString(const string& s) {
		istringstream iss(s);
		Administrador a;
		getline(iss, a.correo, ',');
		getline(iss, a.contrasena, ',');
		return a;
	}

	void mostrarPerfil() const override {
		cout << "[Administrador] " << correo << "\n";
	}
};
