#pragma once
#include <string>
#include "Lista.h"
#include <fstream>
#include "StringUtils.h"

using namespace std;

template<typename T>
class FileManager {
private:
	string filename;
public:
	explicit FileManager(const string& filename)
		: filename(filename) {
	}

    Lista<T> cargarTodos() const {
        Lista<T> items;
		ifstream ifs(filename);
		string line;
		while (getline(ifs, line)) {
			trim(line);
            if (!line.empty()) {
                items.agregaFinal(T::fromString(line));
            }
        }
        return items;
    }

	void guardar(const Lista<T>& items) const {
		ofstream ofs(filename, ios::trunc);
		for (uint i = 0; i < items.longitud(); ++i) {
			ofs << items.obtenerPos(i).serialize() << '\n';
		}
	}
};