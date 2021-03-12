#include "service.hpp"
#include <functional>
#include <assert.h>
#include <iterator>
#include <map>
#include <vector>
#include <iostream>
#include "dto.hpp"
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
#include <numeric>

using std::vector;
using std::string;



vector<DTO> Service::rapoarte() {
    std::map<string, int>dict;
    std::map<string, int>::iterator itr;
    vector<Activitate> v{ rep.getAll() };//fac o copie
    vector<DTO>vec;
    for (std::vector<Activitate>::iterator i = v.begin(); i < v.end(); i++) {
        itr = dict.find((*i).getType());
        if (itr != dict.end())
            itr->second += 1;
        else {
            dict.insert(std::pair<string, int>((*i).getType(), 1));

        }
    }
    for (itr = dict.begin(); itr != dict.end(); ++itr) {
        DTO d{ itr->first ,itr->second };
        vec.push_back(d);
    }

    //auto sum = std::accumulate(vec.begin(), vec.end(), 0, [](int sum, const DTO& a) {return sum + a.getVal(); });

    return vec;
}


/*
Sorteaza dupa titlu
*/

vector<Activitate> Service::sortByTitle() {
    vector<Activitate>sorted{ rep.getAll() };
    std::sort(sorted.begin(), sorted.end(), [](const Activitate& a1, const Activitate& a2) {
        return a1.getTitle() < a2.getTitle();
        });
    return sorted;
}


//Sorteaza dupa description

vector<Activitate> Service::sortByDescription() {
    vector<Activitate>sorted{ rep.getAll() };
    std::sort(sorted.begin(), sorted.end(), [](const Activitate& a1, const Activitate& a2) {
        return a1.getDescription() < a2.getDescription();
        });
    return sorted;
}



//Sorteaza dupa tip+durata

vector<Activitate> Service::sortByTypeTime() {
    vector<Activitate>sorted{ rep.getAll() };
    std::sort(sorted.begin(), sorted.end(), [](const Activitate& a1, const Activitate& a2) {
        if (a1.getType() == a2.getType()) {
            return a1.getTime() < a2.getTime();
        }
        return a1.getType() < a2.getType();
        });
    return sorted;

}



/*
Adauga o activitate
arunca exceptie daca: nu se poate salva, nu este valida
*/
void Service::addActivitate(const string& title, const string& type, const string& description, int time) {
    Activitate a{ title, type, description, time };
    val.validate(a);
    rep.store(a);

    undoActions.push_back(std::make_unique<UndoAdauga>(rep, a));
}

/*
Modifica o activitate
arunca exeptie daca nu exista activitatea care se cauta pentru a fi modificata
Date de intrare: titlu,tup,descriere,duarat
*/

void Service::modifyActivitate(const string& title, const string& type, const string& description, const int time) {
    Activitate activity = rep.find(title);
    rep.modify(activity, type, description, time);
    Activitate activity_new = rep.find(title);
    undoActions.push_back(std::make_unique<UndoModifica>(rep, activity_new, activity));
}

/*
Cauta activitate dupa titlu si tip si o returneaza;
Date de intrare: 2 string -titlu, tip
*/

const Activitate Service::findActivity(const string& title)const {
    Activitate activ = rep.find(title);
    return activ;
}


vector<Activitate> Service::filtrareType(const string& type) {
    vector<Activitate>rez;
    vector<Activitate>lista_intreaga = rep.getAll();
    std::copy_if(lista_intreaga.begin(), lista_intreaga.end(), std::back_inserter(rez), [type](const Activitate& a) {
        return a.getType() == type;
        });
    return rez;
}

vector<Activitate> Service::filtrareDescriere(const string& description) {
    vector<Activitate>rez;
    vector<Activitate>lista_intreaga = rep.getAll();
    std::copy_if(lista_intreaga.begin(), lista_intreaga.end(), std::back_inserter(rez), [description](const Activitate& a) {
        return description == a.getDescription();
        });
    return rez;
}


void Service::deletee(const string& title) {
    Activitate a = rep.find(title);
    rep.del(a);

    undoActions.push_back(std::make_unique<UndoSterge>(rep, a));
}

vector<Activitate> Service::addActivitateLista(const string& title) {

    Activitate a = rep.find(title);
    lista_aux.add(a);

    undoActions.push_back(std::make_unique<UndoAddListaActuala>(lista_aux, a));
    return lista_aux.getAll();
}

void Service::goleste_lista() {
    lista_aux.golesteLista();
}

int Service::genereaza_lista(unsigned int numar) {
    auto vector_init = rep.getAll();

    if (numar > vector_init.size() || numar <= 0) {
        return -1;
    }
    return lista_aux.umple(numar, vector_init);
}

void Service::export_files(const string& filename)
{
    std::ofstream out(filename);
    if (!out.is_open()) {
        //verify if the stream is opened
        std::string msg("Error open file");
        throw RepoException(msg);
    }

    auto lista_pentru_fisier = lista_aux.getAll();
    for (auto& activs : lista_pentru_fisier) {
        out << activs.getTitle() << "," << activs.getType() << "," << activs.getDescription() << "," << activs.getTime() << std::endl;
    }
    out.close();
}

void Service::undo() {
    if (undoActions.empty())
        throw RepoException("\nNo more undo\n");
    undoActions.back()->doUndo();
    undoActions.pop_back();
}

void testAdaugaCtr() {
    Repo rep{ 0.0 };
    ActivitateValidator val;
    ListaActuala lista_aux;
    Service ctr{ rep,val,lista_aux };
    ctr.addActivitate("a", "a", "b", 6);
    assert(ctr.getAll().size() == 1);
    ctr.addActivitateLista("a");
    assert(ctr.get_all_list().size() == 1);

    try {
        ctr.addActivitateLista("b");
        assert(false);
    }
    catch (RepoException&) {
        assert(true);
    }
    //adaug ceva invalid
    try {
        ctr.addActivitate("", "", "", -1);
        assert(false);
    }
    catch (ValidateException&) {
        assert(true);
    }
    //incerc sa adaug ceva ce existadeja
    try {
        ctr.addActivitate("a", "a", "b", 6);
        assert(false);
    }
    catch (RepoException&) {
        assert(true);
    }
}
void testModifica() {
    Repo rep{ 0.0 };
    ActivitateValidator val;
    ListaActuala lista_aux;
    Service ctr{ rep,val,lista_aux };
    ctr.addActivitate("ana", "are", "mere", 1);
    ctr.modifyActivitate("ana", "are", "pere", 2);
    Activitate a = rep.find("ana");
    assert(a.getDescription() == "pere");
    assert(a.getTime() == 2);
}

void testCautare() {
    Repo rep{0.0};
    ActivitateValidator val;
    ListaActuala lista_aux;
    Service srv{ rep,val,lista_aux };
    srv.addActivitate("ana", "are", "mere", 1);
    Activitate a = srv.findActivity("ana");
    assert(a.getDescription() == "mere");

}

void testStergere() {
    Repo rep{0.0};
    ActivitateValidator val;
    ListaActuala lista_aux;
    Service ctr{ rep,val,lista_aux };
    ctr.addActivitate("ana", "are", "mere", 1);
    ctr.addActivitateLista("ana");
    ctr.deletee("ana");
    ctr.goleste_lista();
    assert(rep.getAll().size() == 0);
    assert(lista_aux.getAll().size() == 0);
}

void testFiltrari() {
    Repo rep{0.0};
    ActivitateValidator val;
    ListaActuala lista_aux;
    Service srv{ rep,val,lista_aux };
    srv.addActivitate("ana", "are", "meree", 1);
    srv.addActivitate("anaa", "are", "mereeee", 1);
    srv.addActivitate("anaaa", "nu_are", "mere", 1);
    srv.addActivitate("anaaaaa", "n-are", "mere", 1);
    srv.addActivitate("anaaaaaaa", "a", "mere", 1);
    assert(srv.filtrareDescriere("mere").size() == 3);
    assert(srv.filtrareType("are").size() == 2);

}

void testSortare() {
    Repo rep{ 0.0};
    ActivitateValidator val;
    ListaActuala lista_aux;
    Service srv{ rep,val,lista_aux };
    srv.addActivitate("c", "d", "c", 9);
    srv.addActivitate("b", "a", "b", 14);
    srv.addActivitate("a", "a", "a", 10);
    auto a = srv.sortByTitle().at(0);
    assert(a.getTitle() == "a");
    a = srv.sortByDescription().at(0);
    assert(a.getDescription() == "a");
    a = srv.sortByTypeTime().at(0);
    assert(a.getTime() == 10);



}
void testRaport() {
    Repo rep{0.0};
    ActivitateValidator val;
    ListaActuala lista_aux;
    Service srv{ rep,val,lista_aux };
    srv.addActivitate("fotbal", "sport", "ok", 1);
    srv.addActivitate("volei", "sport", "simaiok", 2);
    srv.addActivitate("citit", "cultura", "superok", 3);
    vector<DTO>vec = srv.rapoarte();
    assert(vec.size() == 2);

    assert(vec.at(0).getType() == "cultura");
    assert(vec.at(0).getVal() == 1);
    assert(vec.at(1).getType() == "sport");
    assert(vec.at(1).getVal() == 2);


}

void testRandom() {
    Repo rep{0.0};
    ActivitateValidator val;
    ListaActuala lista_aux;
    Service srv{ rep,val,lista_aux };
    srv.addActivitate("c", "d", "c", 9);
    srv.addActivitate("b", "a", "b", 14);
    srv.addActivitate("a", "a", "a", 10);

    //assert(srv.genereaza_lista(-1) == -1);
    assert(srv.genereaza_lista(10) == -1);

    srv.genereaza_lista(2);
    assert(lista_aux.getAll().size() == 2);
}

void testUndo() {
    Repo rep{0.0};
    ListaActuala lista_auxiliara;
    ActivitateValidator val;
    Service srv{ rep, val, lista_auxiliara };

    srv.addActivitate("fotbal", "sportiv", "ok", 90);
    srv.addActivitate("citit", "cultural", "super", 120);

    srv.addActivitate("baschet", "sporivt", "fun", 50);
    assert(rep.getAll().size() == 3);
    srv.undo();
    assert(rep.getAll().size() == 2);

    srv.deletee("fotbal");
    assert(rep.getAll().size() == 1);
    srv.undo();
    assert(rep.getAll().size() == 2);

    srv.modifyActivitate("citit", "educativ", "yusss", 180);
    assert(rep.find("citit").getType() == "educativ");
    srv.undo();
    assert(rep.find("citit").getType() == "cultural");

    srv.addActivitateLista("citit");
    srv.undo();
    assert(lista_auxiliara.getAll().size() == 0);

}

void testCtr() {
    testAdaugaCtr();
    testModifica();
    testCautare();
    testStergere();
    testFiltrari();
    testSortare();
    testRaport();
    testRandom();
    testUndo();
}

