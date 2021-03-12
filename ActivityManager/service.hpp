#pragma once

#include "domain.hpp"
#include "repo.hpp"
#include <string>
#include <vector>
#include "dto.hpp"
#include "undo.hpp"
#include <memory>
#include <functional>
#include "validator.hpp"
#include "listaActuala.hpp"

using std::vector;
using std::function;
using std::unique_ptr;

class Service {
    BIGrepo& rep;
    ActivitateValidator& val;
    ListaActuala& lista_aux;

    vector<unique_ptr<ActiuneUndo>>undoActions;// <=> vector<ActiuneUndo*>undoActions, eliberarea memoriei se va face automat



public:
    Service(BIGrepo& rep, ActivitateValidator& val, ListaActuala& lista_aux) noexcept :rep{ rep }, val{ val }, lista_aux{ lista_aux } {
    }
    //nu permitem copierea de obiecte Service
    Service(const Service& ot) = delete;
    /*
     returneaza toate actvitatile in ordinea in care au fost adaugate
    */
    const vector<Activitate> getAll() noexcept {
        return rep.getAll();
    }
    /*
    Adauga o noua activitate
    arunca exceptie daca: nu se poate salva, nu este valida
    */
    void addActivitate(const string& title, const string& type, const string& description, int time);

    /*
    Sorteaza dupa descriere
    */
    vector<Activitate> sortByDescription();

    /*
    Sorteaza dupa titlu
    */
    vector<Activitate> sortByTitle();


    /*
    Sorteaza dupa tip+durata
    */
    vector<Activitate> sortByTypeTime();

    /*
    Filtreaza dupa o descriere data
    */
    vector<Activitate> filtrareDescriere(const string&);

    /*
    Filtreaza dupa un tip dat
    */
    vector<Activitate> filtrareType(const string& type);

    /*
    Modifica o descrierea si durata unei activitati date
    */
    void modifyActivitate(const string& title, const string& type, const string& description, const int time);

    /*
    Cauta o activitate dupa titlu si tip
    */
    const Activitate findActivity(const string& title)const;

    /*
    Sterge o activitate din lista
    */
    void deletee(const string& title);

    /*
    Returneaza ptin numele functiei un vector de rapoarte privind numarul de activitati de un anumit tip
    */
    vector<DTO> rapoarte();

    /*
    Adauga o activitate in lista de activitati curente(activitate cautata in lista initiala dupa titlu)
    Date de intrare: title-string
    */
    vector<Activitate> addActivitateLista(const string& title);

    /*
    Goleste lista de activitati curente
    */
    void goleste_lista();

    /*
    Genereaza o lista de activitati curente aleatoare din lista initiala de dimensiunea nr
    Date de intarre: nr-intreg
    */
    int genereaza_lista(unsigned int numar);

    /*
    Returneaza activitatile din lista curenta
    */
    const vector<Activitate> get_all_list() noexcept {
        return lista_aux.getAll();
    }

    /*
    Exporta lista de activitati curente in fisier
    */
    void export_files(const string& filename);

    /*
    Reface ultima operatie facuta
    */
    void undo();
};
void testCtr();
