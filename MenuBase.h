#pragma once
#include "Sesion.h"
#include "MenuOpcion.h"
#include "Lista.h"
#include"MatrizColor.h"
#include <iostream>
class MenuBase {
protected:
	Sesion& sesion;
	Lista<MenuOpcion*> opciones;
	
public:
	
	explicit MenuBase(Sesion& s) : sesion(s) {}
	virtual ~MenuBase() {
		for (int i = 0; i < opciones.longitud(); ++i)
			delete opciones.obtenerPos(i);
	}
	virtual void mostrar() const {
		int y = 14;
		for (int i = 0; i < opciones.longitud(); ++i) {
			cout << (i + 1) << ". "
				<< opciones.obtenerPos(i)->getTexto();cursor(8,y);
			y++;
		}
	}
	virtual void ejecutar(int opcion) {
		if (opcion == 0 || opcion > opciones.longitud())
			return;
		opciones.obtenerPos(opcion - 1)->ejecutar();
	}
	void agregarOpcion(MenuOpcion* opt) {
		opciones.agregaFinal(opt);
	}
};