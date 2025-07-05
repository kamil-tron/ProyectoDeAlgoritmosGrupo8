#include "MenuInicio.h"
#include "MatrizColor.h"
#include "ContadorDeDias.h"
#include <limits>

MenuInicio::MenuInicio() {
	opciones_.agregaFinal("Registrarse");
	opciones_.agregaFinal("Iniciar sesion");
	opciones_.agregaFinal("Salir");
}

void MenuInicio::mostrar() const {
	int y = 30;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// bitmask de fondo gris claro:
	const WORD BG_GRAY = BACKGROUND_RED|BACKGROUND_GREEN|BACKGROUND_BLUE;
	system("cls");
	SetConsoleTextAttribute(hConsole, BG_GRAY);
	generarmatriz();

	logo(29,5);
	cursor(60, 29);

	cout << "SISTEMA DE TICKETS - AIR PACIFIC";


	cursor(40, y);  y++;
	SetConsoleTextAttribute(hConsole, BACKGROUND_GREEN);
	cout << "=====================================================================";


	for (int i = 0; i < opciones_.longitud(); ++i) {
		cursor(60, y);y++;
		SetConsoleTextAttribute(hConsole, BG_GRAY);
		cout << (i + 1)<< ". " <<opciones_.obtenerPos(i);
	}

	cursor(60, y);
	SetConsoleTextAttribute(hConsole, BG_GRAY);
	cout << "Seleccione opcion: ";
}

void MenuInicio::ejecutar() {
	int op = 0;
	while (true) {
		mostrar();
		while (!(cin >> op)) {
			cursor(79,34);
			cin.clear(); cin.ignore();
		}
		cin.ignore(10000, '\n');
		switch (op) {
		case 1:
			system("cls");
			generarmatriz();
			opcionRegistrarse();
			break;
		case 2:
			system("cls");
			generarmatriz();
			opcionIniciarSesion();
			if (sesion) return;
			break;
		case 3:
			system("cls");
			generarmatriz();
			opcionSalir();
			return;
		default:
			cout << "Opcion invalida.\n";
		}
	}
}

void MenuInicio::opcionRegistrarse(){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	const WORD BG_GRAY = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
	string nombre, apellido, dni, correo, pass; int y = 6;
	cursor(60, 5);
	SetConsoleTextAttribute(hConsole, BG_GRAY); cursor(60, y); y++;
	cout << "Ingrese nombre: "; getline(cin, nombre); cursor(60, y); y++;
	cout << "Ingrese apellido: "; getline(cin, apellido); cursor(60, y); y++;
	cout << "Ingrese DNI: "; getline(cin, dni); cursor(60, y); y++;
	cout << "Ingrese correo: "; getline(cin, correo); cursor(60, y); y++;
	cout << "Ingrese contrasena: "; getline(cin, pass); cursor(60, y); y++;

	Usuario u(dni, nombre, apellido, correo, pass);
	if (auth.registrarUsuario(u)){
		cout << "Registro exitoso."; cursor(60, y); y++;
	}else
		cout << "DNI o correo ya registrado.";
}

void MenuInicio::opcionIniciarSesion() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	const WORD BG_GRAY = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
	string correo, pass;int y=6;
	cursor(60, 5);
	SetConsoleTextAttribute(hConsole, BG_GRAY);
	cout << "Correo: ";      getline(cin, correo);cursor(60, y);y++;
	cout << "Contrasena: ";  getline(cin, pass);cursor(60, y); y++;

	Sesion* nueva = new Sesion();
	if (auth.login(correo, pass, *nueva)) {
		cout << "Inicio de sesion exitoso.\n";	cursor(60, y); y++;
		sesion = nueva;

		ContadorDeDias::obtenerInstancia()->incrementarContador();
		int diasTranscurridos = ContadorDeDias::obtenerInstancia()->obtenerDiasTranscurridos();
		cout << "Día " << diasTranscurridos << " del simulador.\n";
	}
	else {
		delete nueva;
		cout << "Credenciales invalidas.\n";
	}
_getch();
}

void MenuInicio::opcionSalir() {
system("cls");
adios();
}