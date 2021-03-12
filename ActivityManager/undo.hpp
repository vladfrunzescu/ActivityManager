#include "domain.hpp"
#include "repo.hpp"
#include "listaActuala.hpp"


class ActiuneUndo {
public:
    virtual void doUndo() = 0;
    virtual ~ActiuneUndo() = default;
};

class UndoAdauga : public ActiuneUndo {
private:
    Activitate activ_adaugata;
    BIGrepo& rep;
public:
    UndoAdauga(BIGrepo& rep, Activitate& activ_adauga) : rep{ rep }, activ_adaugata{ activ_adauga }{}

    /*
    Sterge ultima activitate adaugata in lista de activitati
    */
    void doUndo() override {
        rep.del(activ_adaugata);
    }
};

class UndoSterge :public ActiuneUndo {
private:
    Activitate activ_stergere;
    BIGrepo& rep;
public:
    UndoSterge(BIGrepo& rep, Activitate& activ) :rep{ rep }, activ_stergere{ activ } {}

    /*
    Adauga activitatea staarsa anterior din lista de activitati
    */
    void doUndo()override {
        rep.store(activ_stergere);
    }
};

class UndoModifica :public ActiuneUndo {
private:
    Activitate activ_modificata;
    Activitate activ_initiala;
    BIGrepo& rep;
public:
    UndoModifica(BIGrepo& rep, Activitate& activ_noua, Activitate& activ_veche) :rep{ rep }, activ_modificata{ activ_noua }, activ_initiala{ activ_veche } {}

    /*
    Reface starea anterioara a unei activitati ce a suferit modificari
    */
    void doUndo() override {
        rep.modify(activ_modificata, activ_initiala.getType(), activ_initiala.getDescription(), activ_initiala.getTime());
    }
};

class UndoAddListaActuala : public ActiuneUndo {
private:
    ListaActuala& lista;
    Activitate activ_adaugata;
public:
    UndoAddListaActuala(ListaActuala& lista, const Activitate& a) : lista{ lista }, activ_adaugata{ a }{}
    /*
    Sterge ultima activitate introdusa in lista de activitati actuale
    */
    void doUndo() override {
        lista.del(activ_adaugata);
    }

};
