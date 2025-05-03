#pragma once
#include "FileManager.h"
#include "Usuario.h"
#include "Lista.h"
#include <string>

using namespace std;

class RepoUsuarios : public FileManager<Usuario> {
public:
    RepoUsuarios()
        : FileManager("usuarios.txt") {}

    bool buscarPorDni(const string& dni, Usuario& u) const {
        auto items = cargarTodos();
        for (int i = 0; i < items.longitud(); ++i) {
            Usuario tmp = items.obtenerPos(i);
            if (tmp.getDni() == dni) {
                u = tmp;
                return true;
            }
        }
        return false;
    }

    bool buscarPorCorreo(const string& correo, Usuario& u) const {
        auto items = cargarTodos();
        for (int i = 0; i < items.longitud(); ++i) {
            Usuario tmp = items.obtenerPos(i);
            if (tmp.getCorreo() == correo) {
                u = tmp;
                return true;
            }
        }
        return false;
    }

    void agregar(const Usuario& u) {
        auto items = cargarTodos();
        items.agregaFinal(u);
        guardar(items);
    }
};