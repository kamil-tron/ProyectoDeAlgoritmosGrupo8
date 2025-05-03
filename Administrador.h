#pragma once

#include "UserEntity.h"
#include <string>
using namespace std;

class Administrador final : public UserEntity {
public:
    Administrador() = default;
    Administrador(string correo, string pass)
        : UserEntity(correo, pass) {}

    RolEnum getRol() const override { return RolEnum::ADMIN; }

    string serialize() const;
    static Administrador fromString(const string& s);
    void mostrarPerfil() const override {
        cout << "[Administrador] " << correo << "\n";
    }
};
