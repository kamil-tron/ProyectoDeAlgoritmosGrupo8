#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class ContadorDeDias {
private:
    static ContadorDeDias* instancia;
    int contador;
    const string archivoContador = "contador_dias.txt";

    ContadorDeDias() {
        contador = leerContadorDeArchivo();
    }

    int leerContadorDeArchivo() {
        ifstream archivo(archivoContador);
        if (!archivo.is_open()) {
            ofstream nuevoArchivo(archivoContador);
            if (nuevoArchivo.is_open()) {
                nuevoArchivo << 0;
                nuevoArchivo.close();
            }
            return 0;
        }
        int contador = 0;
        archivo >> contador;
        archivo.close();
        return contador;
    }

    void guardarContadorEnArchivo() {
        ofstream archivo(archivoContador);
        if (archivo.is_open()) {
            archivo << contador;
            archivo.close();
        }
    }

public:
    static ContadorDeDias* obtenerInstancia() {
        if (!instancia) {
            instancia = new ContadorDeDias();
        }
        return instancia;
    }

    void incrementarContador() {
        contador++;
        guardarContadorEnArchivo();
    }

    int obtenerDiasTranscurridos() {
        return contador / 10;
    }

    void reiniciarContador() {
        contador = 0;
        guardarContadorEnArchivo();
    }
};

ContadorDeDias* ContadorDeDias::instancia = nullptr;
