#pragma once

#include "UserEntity.h"
#include <string>
#include <iostream>

class Usuario final : public UserEntity {
private:
	string dni, nombre, apellido;

public:
	Usuario() = default;

	Usuario(const string& dni,
		const string& nombre,
		const string& apellido,
		const string& correo,
		const string& pass)
		: UserEntity(correo, pass),
		dni(dni),
		nombre(nombre),
		apellido(apellido)
	{
	}

	RolEnum getRol() const override { return RolEnum::USER; }
	const string& getDni() const { return dni; }
	const string& getNombre() const { return nombre; }
	const string& getApellido() const { return apellido; }

	void setDni(const string& d) { dni = d; }
	void setNombre(const string& n) { nombre = n; }
	void setApellido(const string& a) { apellido = a; }

	void mostrarPerfil() const override {
		cout << "Nombre : " << nombre << ' ' << apellido << '\n'
			<< "DNI    : " << dni << '\n'
			<< "Correo : " << correo << '\n';
	}

	string serialize() const;
	static Usuario fromString(const string& s);
};
