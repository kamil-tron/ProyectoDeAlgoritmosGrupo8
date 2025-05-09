#pragma once
#include "Roles.h"
#include <string>
#include <iostream>

using namespace std;

class UserEntity {
protected:
	string correo;
	string contrasena;

	UserEntity() = default;

	UserEntity(const string& c, const string& p)
		: correo(c), contrasena(p) {
	}

public:
	virtual ~UserEntity() = default;

	virtual RolEnum getRol() const = 0;

	const string& getCorreo() const { return correo; }

	bool checkPassword(const string& pass) const { return contrasena == pass; }

	virtual void mostrarPerfil() const = 0;
};
