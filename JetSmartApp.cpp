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
				while (!(cin >> op)) {
					cursor(29, 26);
					cin.clear(); cin.ignore();
				}
				cin.ignore(10000, '\n');
				menu.ejecutar(op);
			} while (op != 13);
		}
		else {
			MenuUsuario menu(*ses);
			do {
				menu.mostrar();
				cout << "Ingrese una opcion: ";
				while (!(cin >> op)) {
					cursor(27, 18);
					cin.clear(); cin.ignore();
				}
				cin.ignore(10000, '\n');
				menu.ejecutar(op);
			} while (op != 5);
		}
		delete ses;
		cout << "   Sesion cerrada. Volviendo al menu de inicio...";
	}
}