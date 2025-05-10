#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class ContadorDeDias {
private:
    static ContadorDeDias* instancia;
    int contador;
    const string archivoContador = "contador_dias.txt";  // Nombre del archivo

    // Constructor privado
    ContadorDeDias() {
        contador = leerContadorDeArchivo();  // Leer el contador desde el archivo
    }

    // Funci�n que lee el contador desde el archivo
    int leerContadorDeArchivo() {
        ifstream archivo(archivoContador);  // Abrir el archivo en modo lectura

        if (!archivo.is_open()) {
            // Si el archivo no se abre, crearlo con valor 0
            ofstream nuevoArchivo(archivoContador);  // Crear el archivo en modo escritura
            if (nuevoArchivo.is_open()) {
                nuevoArchivo << 0;  // Escribir el valor 0 en el archivo
                nuevoArchivo.close();  // Cerrar el archivo de salida
            }
            return 0;  // Retornar 0 si el archivo no exist�a
        }

        int contador = 0;
        archivo >> contador;  // Leer el contador desde el archivo
        archivo.close();  // Cerrar el archivo de lectura
        return contador;  // Retornar el contador le�do
    }

    // Funci�n para guardar el contador en el archivo
    void guardarContadorEnArchivo() {
        ofstream archivo(archivoContador);  // Crear flujo de salida local
        if (archivo.is_open()) {
            archivo << contador;  // Escribir el contador en el archivo
            archivo.close();  // Cerrar el archivo de salida
        }
    }

public:
    // M�todo est�tico para obtener la instancia �nica (Singleton)
    static ContadorDeDias* obtenerInstancia() {
        if (!instancia) {
            instancia = new ContadorDeDias();  // Crear la instancia si no existe
        }
        return instancia;
    }

    // M�todo para incrementar el contador de d�as
    void incrementarContador() {
        contador++;
        guardarContadorEnArchivo();  // Guardar el contador en el archivo
    }

    // M�todo para obtener los d�as transcurridos (cada 10 unidades de contador es un d�a)
    int obtenerDiasTranscurridos() {
        return contador / 10;
    }

    // M�todo para reiniciar el contador (si es necesario)
    void reiniciarContador() {
        contador = 0;
        guardarContadorEnArchivo();  // Guardar el contador reiniciado en el archivo
    }
};

// Inicializaci�n de la instancia est�tica
ContadorDeDias* ContadorDeDias::instancia = nullptr;
