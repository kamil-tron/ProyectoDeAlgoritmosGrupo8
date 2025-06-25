#pragma once

#include <string>
#include <sstream>

using namespace std;

class CheckIn {
private:
    int id{};
    int vueloId{};
    string reservaCod;
    string email;
    string fechaHora;

public:
    CheckIn() = default;

    CheckIn(int id_, int vueloId_, string reservaCod_, string email_)
        : id(id_), vueloId(vueloId_), reservaCod(move(reservaCod_)), email(move(email_)) {}

    int getId() const { return id; }
    int getVueloId() const { return vueloId; }
    const string& getReservaCod() const { return reservaCod; }
    const string& getEmail() const { return email; }

    string serialize() const {
        ostringstream oss;
        oss << id << ','
            << vueloId << ','
            << reservaCod << ','
            << email;
        return oss.str();
    }

    static CheckIn fromString(const string& line) {
        istringstream iss(line);
        CheckIn c;
        string field;

        getline(iss, field, ',');
        c.id = stoi(field);

        getline(iss, field, ',');
        c.vueloId = stoi(field);

        getline(iss, c.reservaCod, ',');
        getline(iss, c.email);

        return c;
    }
};
