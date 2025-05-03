#pragma once

#include "MenuOpcion.h"
#include <functional>

class MenuOpcionTexto : public MenuOpcion {
private:
    string texto;
    function<void()> accion;
public:
    MenuOpcionTexto(string texto, function<void()> accion)
        : texto(move(texto)), accion(move(accion)) {}

    string getTexto() const override {
        return texto;
    }

    void ejecutar() override {
        accion();
    }
};