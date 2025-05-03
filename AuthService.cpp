#include "AuthService.h"

bool AuthService::login(const string& correo,
    const string& pass,
    Sesion& s) const {
    Administrador adm;
    if (repoAdm.buscarPorCorreo(correo, adm)
        && adm.checkPassword(pass)) {
        s.setUsuario(new Administrador(adm));
        return true;
    }
    Usuario usr;
    if (repoUsr.buscarPorCorreo(correo, usr)
        && usr.checkPassword(pass)) {
        s.setUsuario(new Usuario(usr));
        return true;
    }
    return false;
}


bool AuthService::registerUser(const Usuario& u) {
    Usuario tmp;
    if (repoUsr.buscarPorDni(u.getDni(), tmp)
        || repoUsr.buscarPorCorreo(u.getCorreo(), tmp))
        return false;

    repoUsr.agregar(u);
    return true;
}