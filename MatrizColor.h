#pragma once

#include <Windows.h>
#include <conio.h>
#include <cstdio>

#define FILAS 44
#define COLUMNAS 156
using namespace std;

void cursor(int x, int y);
void aplicarColorPorOpcion(int opcion);
void logo(int x,int y);
void generarmatriz();

