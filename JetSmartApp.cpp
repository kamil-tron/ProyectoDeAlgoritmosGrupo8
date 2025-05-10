// JetSmartApp.cpp
#include "JetSmartApp.h"

using namespace std;

void JetSmartApp::run() {
	while (true) {
		MenuInicio inicio;
		inicio.ejecutar();               // login / registro / salir

		Sesion* ses = inicio.getSesion();
		if (!ses) {                      // usuario eligió «Salir»
			cout << "Saliendo del sistema...\n";
			break;
		}

		// ── menú según rol ──────────────────────────────────────
		int op = 0;
		if (ses->getRol() == RolEnum::ADMIN) {
			MenuAdministrador menu(*ses);
			do {
				menu.mostrar();
				cout << "\nIngrese una opcion: ";
				cin >> op;
				cin.ignore(10000, '\n');  // sin <limits>
				menu.ejecutar(op);        // 7 = cerrar sesión
			} while (op != 8);
		}
		else {
			MenuUsuario menu(*ses);
			do {
				menu.mostrar();
				cout << "\nIngrese una opcion: ";
				cin >> op;
				cin.ignore(10000, '\n');  // sin <limits>
				menu.ejecutar(op);        // 6 = cerrar sesión
			} while (op != 4);
		}

		delete ses;
		cout << "\nSesion cerrada. Volviendo al menu de inicio...\n\n";
	}
}