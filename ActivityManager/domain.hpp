#pragma once
#include <string>
#include <iostream>

using std::string;

class Activitate {

private:
    string title;
    string type;
    string description;
    int time;

public:
    /*constructor*/
    Activitate(const string title, const string type, const string description, int time) : title{ title }, type{ type }, description{ description }, time{ time }{}
    //Activitate(const Activitate& m) :title{ m.title }, type{ m.type }, description{ m.description }, time{ m.time }{}
    Activitate() = default;

    /*Returneaza tipul*/
    string getType() const {
        return type;
    }

    /*Returneaza titlul*/
    string getTitle() const {
        return title;
    }
    /*Returneaza durata*/
    int getTime() const noexcept {
        return time;
    }

    /*Returneaza descrierea*/
    string getDescription() const {
        return description;
    }


    /*
    Schimba descrierea actuala cu cea data
    Date de intrare: sting descriere
    */
    void setDescription(const string& descriptions) {
        this->description = descriptions;
    }
    /*
    Schimba durata actuala cu cea data
    Date de intrare: timp-integer
    */
    void setTime(const int times) noexcept {
        this->time = times;
    }
    /*
    Schimba tipul activitatii cu cel dat
    Date d eintrare: type-string
    */
    void setType(const string& types) {
        this->type = types;
    }

    /*
    Determina modul de scriere al activitatii
    */
    string toString() {
        return title + " " + type + " " + description + " " + std::to_string(time);
    }
};
