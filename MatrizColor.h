#pragma once

#include <iostream>
#include <Windows.h>
#include <conio.h>

#define FILAS 20
#define COLUMNAS 100
using namespace std;
void aplicarColorPorOpcion(int opcion) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    switch (opcion) {
    case 0: // Blanco sobre negro (default)
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        break;
    case 1: // Negro sobre blanco
        SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
        break;
    case 2: // Blanco sobre azul oscuro
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_BLUE);
        break;
    case 3: // Azul oscuro sobre azul claro
        SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
        break;

        // Opciones nuevas
    case 4: // Rojo brillante sobre negro
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
        break;
    case 5: // Verde brillante sobre negro
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        break;
    case 6: // Amarillo brillante sobre negro (Rojo + Verde)
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
        break;
    case 7: // Cian brillante sobre negro (Verde + Azul)
        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        break;
    case 8: // Magenta brillante sobre negro (Rojo + Azul)
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
        break;
    case 9: // Texto negro sobre verde claro
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
        break;
    case 10: // Texto negro sobre rojo claro
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_INTENSITY);
        break;
    case 11: // Texto blanco brillante sobre rojo oscuro
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED);
        break;
    case 12: // Texto negro sobre amarillo brillante (Rojo + Verde claros)
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
        break;
    case 13: // Texto blanco sobre magenta oscuro
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_RED | BACKGROUND_BLUE);
        break;
    case 14: // Texto negro sobre cian brillante
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
        break;
    case 15: // Gris claro sobre negro (intensidad media)
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        break;

    default: // Blanco sobre negro
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        break;
    }
}


int matriz[FILAS][COLUMNAS] =
{ {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0},
{0,6,1,1,1,1,1,0,6,1,1,1,1,1,0,6,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,6,1,6,6,6,1,0,6,6,6,1,0,0,0,6,1,6,6,6,1,0,0,0,0,0,0,0,0,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,0,0,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,6,1,0,0,6,1,0,0,0,6,1,0,0,0,6,1,0,0,6,1,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,0,0,0,0,0,0,4,0,0,0,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,6,1,0,0,6,1,0,0,0,6,1,0,0,0,6,1,0,0,6,1,0,0,0,0,3,3,3,3,3,3,3,3,3,0,0,0,3,3,0,0,3,3,0,0,3,3,0,0,3,3,0,0,3,3,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,6,1,0,0,6,1,0,0,0,6,1,0,0,0,6,1,0,0,6,1,0,0,3,3,0,0,0,0,3,3,3,3,0,0,0,0,3,3,0,0,3,3,0,0,3,3,0,0,3,3,0,0,3,3,0,0,0,0,0,0,0,0,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0},
{0,6,1,0,0,6,1,0,0,0,6,1,0,0,0,6,1,0,0,6,1,0,0,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0},
{0,6,1,0,0,6,1,0,0,0,6,1,0,0,0,6,1,0,0,6,1,0,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0},
{0,6,1,1,1,1,1,0,0,0,6,1,0,0,0,6,1,1,1,1,1,0,0,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,6,1,6,6,6,1,0,0,0,6,1,0,0,0,6,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,6,1,0,0,6,1,0,0,0,6,1,0,0,0,6,1,6,1,1,0,0,0,0,2,2,2,2,0,2,2,2,2,0,2,2,2,2,0,2,2,2,2,0,2,2,2,2,0,2,2,2,2,0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,4,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0},
{0,6,1,0,0,6,1,0,0,0,6,1,0,0,0,6,1,0,6,1,0,0,0,0,2,0,0,2,0,2,0,0,2,0,2,2,0,0,0,0,2,2,0,0,2,0,0,0,0,0,2,2,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,6,1,0,0,6,1,0,0,0,6,1,0,0,0,6,1,0,6,1,1,0,0,0,2,0,0,2,0,2,0,0,2,0,2,2,0,0,0,0,2,2,0,0,2,0,0,0,0,0,2,2,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0},
{0,6,1,0,0,6,1,0,0,0,6,1,0,0,0,6,1,0,0,6,1,0,0,0,2,2,2,2,0,2,2,2,2,0,2,2,0,0,0,0,2,2,0,0,2,2,2,2,0,0,2,2,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0},
{0,6,1,0,0,6,1,0,0,0,6,1,0,0,0,6,1,0,0,6,1,0,0,0,2,0,0,0,0,2,2,2,2,0,2,2,0,0,0,0,2,2,0,0,2,0,0,0,0,0,2,2,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,4,0,0,0,0,0,0,0,0,0},
{0,6,1,0,0,6,1,0,0,0,6,1,0,0,0,6,1,0,0,6,1,0,0,0,2,0,0,0,0,2,0,0,2,0,2,2,0,0,0,0,2,2,0,0,2,0,0,0,0,0,2,2,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,6,1,0,0,6,1,0,6,1,1,1,1,1,0,6,1,0,0,6,1,0,0,0,2,0,0,0,0,2,0,0,2,0,2,2,2,2,0,2,2,2,2,0,2,0,0,0,0,2,2,2,2,0,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,4,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};
void generarmatriz() {
    for (int f = 0; f < FILAS; f++) {
        for (int c = 0; c < COLUMNAS; c++) {
            if (matriz[f][c] == 0) aplicarColorPorOpcion(0);
            if (matriz[f][c] == 1) aplicarColorPorOpcion(10);
            if (matriz[f][c] == 2) aplicarColorPorOpcion(2);
            if (matriz[f][c] == 3) aplicarColorPorOpcion(9);
            if (matriz[f][c] == 4) aplicarColorPorOpcion(6);
            if (matriz[f][c] == 5) aplicarColorPorOpcion(2);
            if (matriz[f][c] == 6) aplicarColorPorOpcion(11);

            cout << " ";
        }
        cout << endl;
    }
}