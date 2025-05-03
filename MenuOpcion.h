#pragma once

#include <string>

using namespace std;

class MenuOpcion {
public:
    virtual ~MenuOpcion() = default;
    virtual string getTexto() const = 0;
    virtual void ejecutar() = 0;
};