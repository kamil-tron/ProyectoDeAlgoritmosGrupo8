#include "JetSmartApp.h"

using namespace std;

void JetSmartApp::run() {
	while (true) {
		MenuInicio inicio;
		inicio.ejecutar();

		Sesion* ses = inicio.getSesion();
		if (!ses) {
			cout << "Saliendo del sistema...\n";
			break;
		}

		int op = 0;
		if (ses->getRol() == RolEnum::ADMIN) {
			MenuAdministrador menu(*ses);
			do {
				menu.mostrar();
				cout << " Ingrese una opcion: ";
				cin >> op;
				cin.ignore(10000, '\n');
				menu.ejecutar(op);
			} while (op != 13);
		}
		else {
			MenuUsuario menu(*ses);
			do {
				menu.mostrar();
				cout << "Ingrese una opcion: ";
				cin >> op;
				cin.ignore(10000, '\n');
				menu.ejecutar(op);
			} while (op != 5);
		}

		delete ses;
		cout << "\nSesion cerrada. Volviendo al menu de inicio...\n\n";
	}
}