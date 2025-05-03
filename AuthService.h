#pragma once
#include "RepoAdmins.h"
#include "RepoUsuarios.h"
#include "Sesion.h"
#include <string>

class AuthService {
private:
    RepoAdmins   repoAdm;
    RepoUsuarios repoUsr;

public:
    AuthService() = default;

    bool login(const string& correo,
        const string& pass,
        Sesion& s) const;

    bool registerUser(const Usuario& u);
};