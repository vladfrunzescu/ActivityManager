#pragma once
#include "service.hpp"
#include "domain.hpp"
class ConsolUI {
    Service& ctr;

    void adaugaUI();

    void tipareste(const vector<Activitate>& activs);

    void modificare();

    void cauta();

    void filtrare();

    void stergere();

    void rapoarte();

    void adauga_in_lista();

    void goleste_lista();

    void genereaza_lista();

    void exporta();

    void undo();
public:
    ConsolUI(Service& ctr) noexcept :ctr{ ctr } {
    }
    //nu permitem copierea obiectelor
    ConsolUI(const ConsolUI& ot) = delete;

    void start();


};
