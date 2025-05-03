#include "MenuInicio.h"

MenuInicio::MenuInicio() {
    opciones_.agregaFinal("Registrarse");
    opciones_.agregaFinal("Iniciar sesion");
    opciones_.agregaFinal("Salir");
}

void MenuInicio::mostrar() const {
    cout << "-----------------------------------------------\n"
        << "         JETSMART - SISTEMA DE TICKETS\n"
        << "         Bienvenido al sistema JetSmart\n"
        << "-----------------------------------------------\n";
    for (int i = 0; i < opciones_.longitud(); ++i) {
        cout << (i + 1) << ". " << opciones_.obtenerPos(i) << "\n";
    }
    cout << "Seleccione opción: ";
}

void MenuInicio::ejecutar() {
    int op = 0;
    while (true) {
        mostrar();
        cin >> op;
        cin.ignore(10000, '\n');   // sin usar <limits>
        switch (op) {
        case 1:
            opcionRegistrarse();
            break;
        case 2:
            opcionIniciarSesion();
            if (sesion) return;   // sesión iniciada, salgo al app
            break;
        case 3:
            opcionSalir();
            return;
        default:
            cout << "Opcion invalida.\n";
        }
    }
}

void MenuInicio::opcionRegistrarse() {
    string nombre, dni, correo, pass;
    cout << "Ingrese nombre completo: "; getline(cin, nombre);
    cout << "Ingrese DNI: ";            getline(cin, dni);
    cout << "Ingrese correo: ";         getline(cin, correo);
    cout << "Ingrese contrasena: ";     getline(cin, pass);

    Usuario u(dni, nombre, "", correo, pass);
    if (auth.registerUser(u))
        cout << "Registro exitoso.\n";
    else
        cout << "DNI o correo ya registrado.\n";
}

void MenuInicio::opcionIniciarSesion() {
    string correo, pass;
    cout << "Correo: ";      getline(cin, correo);
    cout << "Contrasena: ";  getline(cin, pass);

    Sesion* nueva = new Sesion();
    if (auth.login(correo, pass, *nueva)) {
        cout << "Inicio de sesion exitoso.\n";
        sesion = nueva;
    }
    else {
        delete nueva;  // libero aquí si falla
        cout << "Credenciales invalidas.\n";
    }
}

void MenuInicio::opcionSalir() {
    cout << "Gracias por usar JetSmart. ¡Buen viaje!\n";
}
