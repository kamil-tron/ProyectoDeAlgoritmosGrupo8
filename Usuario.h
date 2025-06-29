#pragma once

#include "UserEntity.h"
#include "StringUtils.h"
#include <string>
#include <iostream>
#include <sstream>

using namespace std;

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
	void setPassword(const string& p) { contrasena = p; }

	void mostrarPerfil() const override {
		cout << "Nombre : " << nombre << ' ' << apellido << '\n'
			<< "DNI    : " << dni << '\n'
			<< "Correo : " << correo << '\n';
	}

	string serialize() const {
		ostringstream ss;
		ss << dni << ','
			<< nombre << ','
			<< apellido << ','
			<< correo << ','
			<< contrasena;
		return ss.str();
	}

	static Usuario fromString(const string& s) {
		istringstream ss(s);
		string dni, nom, ape, mail, pass;

		getline(ss, dni, ',');
		getline(ss, nom, ',');
		getline(ss, ape, ',');
		getline(ss, mail, ',');
		getline(ss, pass);
		trim(pass);

		return Usuario(dni, nom, ape, mail, pass);
	}
};
