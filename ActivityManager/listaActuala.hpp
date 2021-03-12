#pragma once
#include <iostream>
#include <vector>
#include "domain.hpp"
#include "repo.hpp"
#include <random>
#include <chrono>
#include <assert.h>
#include <iterator>

class ListaActuala {

private:
    std::vector<Activitate> activitati;

public:
    ListaActuala() = default;

    //returneaza toata lista de activitati curente
    std::vector<Activitate> getAll() {
        return activitati;
    }

    //sterge toata lista de activitati curente
    void golesteLista() {
        activitati.clear();
    }

    //adauga o activitate noua in lista de activitati curente
    //arunca exceptie daca activitatea exista deja in litsa
    void add(const Activitate& a) {
        for (auto activ : activitati) {
            if (activ.getTitle() == a.getTitle())
                throw RepoException("ACtivitatea exista deja in lista");
        }
        activitati.push_back(a);
    }

    void del(const Activitate& a) {
        std::vector<Activitate>::iterator it;
        for (it = activitati.begin(); it < activitati.end();it++) {
            if ((*it).getTitle() == a.getTitle())
            {
                activitati.erase(it);
                return;
            }
        }
    }

    //genereaza un numar aleator de activitati curente
    //Date de intrare: nr-intreg, vector Activitati
    int umple(unsigned int nr, vector<Activitate> repository) {

        auto  seed = std::chrono::system_clock::now().time_since_epoch().count();
        unsigned int transf_toUnsignedInt = static_cast<unsigned int>(seed);
        std::shuffle(repository.begin(), repository.end(), std::default_random_engine(transf_toUnsignedInt)); //amesteca vectorul initial
        golesteLista();
        for (auto activ : repository) {
            if (nr == 0)
                break;
            add(activ);
            nr--;
        }
        return 0;
    }

};


void testListaActuala();
