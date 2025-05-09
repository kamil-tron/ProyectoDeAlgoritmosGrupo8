#pragma once
#include "UserEntity.h"

class Sesion {
private:
	UserEntity* usuarioActual = nullptr;
public:
	Sesion() = default;
	~Sesion() = default;      // Ya no hacemos delete aquí

	Sesion(const Sesion&) = delete;
	Sesion& operator=(const Sesion&) = delete;

	void setUsuario(UserEntity* u) { usuarioActual = u; }
	UserEntity* getUsuarioPtr() const { return usuarioActual; }
	RolEnum getRol() const { return usuarioActual->getRol(); }
	UserEntity& getUsuarioActual() const { return *usuarioActual; }
};
