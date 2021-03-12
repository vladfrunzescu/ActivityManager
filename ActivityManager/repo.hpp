#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "domain.hpp"
#include <ostream>
#include <unordered_map>
#include <cstdlib>
#include <sstream>
#include <iterator>
#include <algorithm>


using std::vector;
using std::string;
using std::ostream;


class BIGrepo{

public:
    virtual void store(const Activitate& a) = 0;
    virtual const Activitate& find(string title) const = 0;
    virtual const vector<Activitate> getAll() noexcept = 0;
    virtual void modify(const Activitate& a, const string& type, const string& description, const int time) = 0;
    virtual void del(const Activitate& a) = 0;
};

class Repo:public BIGrepo {

private:
    double p;
    std::unordered_map<string, Activitate> all;

    bool exist(const Activitate& a)const;


public:
    Repo(double probs) : p{ probs } {}
    //nu permitem copierea de obiecte
    Repo(const Repo& ot) = delete;
    /*
    Salvare activitate
    arunca exceptie daca mai exista o activitate cu acelasi titlu si tip
    */
    void store(const Activitate& a)override;

    /*
    Cauta o activitate dupa titlu
    arunca exceptie daca nu exista activitate
    */
    const Activitate& find(string title) const override;

    /*
    returneaza toate activitatile salvate
    */
    const vector<Activitate> getAll() noexcept override;


    /*
    Modifica o activitata data
    modificatori: descriere-string, time-integer
    Arunca exceptie daca nu exista activitatea
    */
    void modify(const Activitate& a, const string& type, const string& description, const int time) override;


    /*
    Sterge o activitate data
    Arunca exceptie daca nu exist actgivitatea
    */
    void del(const Activitate& a) override;

};



class FileRepo : public Repo {
private:
    string filename;
    void loadAllFromFile();
    void writeAllToFile();
    double p;
public:
    FileRepo(const string& filename, double probs) :Repo(probs), filename{ filename }, p{ probs }{
        loadAllFromFile();
    }
    //FileRepo() = default;
    //nu permitem copierea de obiecte
    FileRepo(const FileRepo & ot) = delete;

    /*
    Salvare activitate
    arunca exceptie daca mai exista o activitate cu acelasi titlu si tip
    */
    void store(const Activitate& a) override {
        Repo::store(a);
        writeAllToFile();
    }

    /*
    Modifica o activitata data
    modificatori: descriere-string, time-integer
    Arunca exceptie daca nu exista activitatea
    */
    void modify(const Activitate& a, const string& type, const string& description, const int time) override {
        Repo::modify(a, type, description, time);
        writeAllToFile();
    }

    /*
    Sterge o activitate data
    Arunca exceptie daca nu exist actgivitatea
    */
    void del(const Activitate& a)override {
        Repo::del(a);
        writeAllToFile();
    }
    

};

/*
Folosit pentru a semnala situatii exceptionale care pot aparea in repo
*/
class RepoException {
    string msg;
public:
    RepoException(string m) :msg{ m } {}
    //functie friend (vreau sa folosesc membru privat msg)
    friend ostream& operator<<(ostream& out, const RepoException& ex);
};

ostream& operator<<(ostream& out, const RepoException& ex);

void testeRepo();
