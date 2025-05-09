#pragma once
#include <string>
#include <sstream>
using namespace std;

class Pago {
private:
	int reservaId;
	double monto;
	string estado;
	string fecha;
public:
	Pago() = default;

	Pago(int reservaId, double monto, const string& estado, const string& fecha)
		: reservaId(reservaId), monto(monto), estado(estado), fecha(fecha) {
	}

	int getReservaId() const { return reservaId; }
	double getMonto() const { return monto; }
	const string& getEstado() const { return estado; }
	const string& getFecha() const { return fecha; }

	string serialize() const;
	static Pago fromString(const string& s);
};
