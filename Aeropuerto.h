#pragma once
#include <string>
#include <sstream>

using namespace std;

class Aeropuerto {
private:
	string codigo; // ej: "LIM" = Lima, Perú
	int x{}; // coordenada horizontal
	int y{}; // coordenada vertical

public:
	Aeropuerto() = default;

	Aeropuerto(string codigo, int x, int y) : codigo(move(codigo)), x(x), y(y) {}

	const string& getCodigo() const { return codigo; }

	int getX() const { return x; }
	int getY() const { return y; }

	void setX(int newX) { x = newX; }
	void setY(int newY) { y = newY; }

	bool operator==(const Aeropuerto& other) const {
		return codigo == other.codigo;
	}

	bool operator<(const Aeropuerto& other) const {
		return codigo < other.codigo;
	}

	string serialize() const {
		ostringstream oss;
		oss << codigo << ',' << x << ',' << y;
		return oss.str();
	}

	static Aeropuerto fromString(const string& s) {
		istringstream iss(s);
		Aeropuerto a;
		char comma;
		getline(iss, a.codigo, ',');
		iss >> a.x >> comma >> a.y;
		return a;
	}
};