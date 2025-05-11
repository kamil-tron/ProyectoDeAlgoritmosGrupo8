#pragma once
#include "AuthService.h"
#include "Sesion.h"
#include "Lista.h"
#include "Usuario.h"
#include "MenuUsuario.h"
#include "MenuAdministrador.h"
#include <iostream>
#include <string>
#include <Windows.h>

class MenuInicio {
private:
	AuthService auth;
	Sesion* sesion = nullptr;
	Lista<string> opciones_;
	void opcionRegistrarse();
	void opcionIniciarSesion();
	void opcionSalir();
public:
	MenuInicio();
	~MenuInicio() = default;
	Sesion* getSesion() const { return sesion; }
	void setSesion(Sesion* s) { sesion = s; }
	void mostrar() const;
	void ejecutar();
};
