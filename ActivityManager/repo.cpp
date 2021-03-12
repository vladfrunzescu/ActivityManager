#include "repo.hpp"
#include "domain.hpp"
#include <assert.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <fstream>

using std::ostream;
using std::stringstream;
using std::unordered_map;

void FileRepo::loadAllFromFile() {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        throw RepoException("Could not open the file!");
    }
    string title, type, description;
    int time;
    while (!fin.eof()) {
        fin >> title >> type >> description >> time;
        Activitate a{ title.c_str(),type.c_str(),description.c_str(),time };
        Repo::store(a);
    }
    fin.close();
}

void FileRepo::writeAllToFile() {
    std::ofstream fout(filename);
    if (!fout.is_open()) {
        throw RepoException("Could not open the file !");
    }
    vector<Activitate>alls = Repo::getAll();
    vector<Activitate>::iterator it;
    for (it=alls.begin();it<alls.end()-1;it++){
        fout << (*it).toString() << std::endl;
    }
    fout << (*(alls.end()-1)).toString();
    fout.close();
}


void Repo::store(const Activitate& a)
{
    float probs = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    if (probs < p)
        throw RepoException("EXCEPTIE CHAOS");

    if (exist(a)) {
        throw RepoException("Exista deja o activitate cu numele:" + a.getTitle());
    }
    all.insert(std::pair<string, Activitate>(a.getTitle(), a));
}



bool Repo::exist(const Activitate& a) const {
    try {
        find(a.getTitle());
        return true;
    }
    catch (RepoException&) {
        return false;
    }
}
/*
Cauta
arunca exceptie daca nu exista activitate
*/
const Activitate& Repo::find(string title) const {
    float probs = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    if (probs < p)
        throw RepoException("EXCEPTIE CHAOS");

    auto gasit = all.find(title);
    if (gasit != all.end()) {
        const Activitate& activ_gasita = gasit->second;
        return activ_gasita;
    }

    //daca nu exista arunc o exceptie
    throw RepoException("Nu exista activitate cu numele:" + title);
}

Activitate funct(std::pair<string, Activitate> itr) {
    return itr.second;
}

const vector<Activitate> Repo::getAll() noexcept {
    vector<Activitate> destinatie;
    std::unordered_map<string, Activitate>::iterator itr;

    std::transform(all.begin(), all.end(), std::back_inserter(destinatie), funct);

    return destinatie;
}



void Repo::modify(const Activitate& a, const string& type, const string& description, const int time) {
    if (exist(a)) {
        unordered_map<string, Activitate>::iterator itr;
        itr = all.find(a.getTitle());
        itr->second.setType(type);
        itr->second.setDescription(description);
        itr->second.setTime(time);
        return;
    }
    else
        throw RepoException("Nu exista activitate cu numele:" + a.getTitle());
}


void Repo::del(const Activitate& a) {
    float probs = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    if (probs < p)
        throw RepoException("EXCEPTIE CHAOS");
    if (exist(a)) {
        std::unordered_map<string, Activitate>::iterator itr;
        itr = all.find(a.getTitle());
        all.erase(itr);
    }
    else
        throw RepoException("Nu exista activitate cu numele:" + a.getTitle());
}

ostream& operator<<(ostream& out, const RepoException& ex) {
    out << ex.msg;
    return out;
}

void testAdauga() {
    Repo rep{0.0};
    rep.store(Activitate{ "titlu","tip","descriere",4 });
    assert(rep.getAll().size() == 1);
    assert(rep.find("titlu").getTitle() == "titlu");

    rep.store(Activitate{ "titlu1","tip1","descriere1",5 });
    assert(rep.getAll().size() == 2);


    try {
        rep.store(Activitate{ "titlu","tip","descriere",23 });
        assert(false);
    }
    catch (const RepoException&) {
        assert(true);
    }
    //cauta inexistent
    try {
        rep.find("titlu2");
        assert(false);
    }
    catch (const RepoException& e) {
        stringstream os;
        os << e;
        assert(os.str().find("exista") >= 0);
    }
}

void testCauta() {
    Repo rep{0.0};
    rep.store(Activitate{ "titlu","tip","descriere",4 });
    rep.store(Activitate{ "titlu1","tip1","descriere1",4 });

    auto activ = rep.find("titlu");
    assert(activ.getTitle() == "titlu");
    assert(activ.getType() == "tip");

    //arunca exceptie daca nu gaseste
    try {
        rep.find("titlu2");
        assert(false);
    }
    catch (RepoException&) {
        assert(true);
    }
}


void testModify() {
    Repo rep{0.0};
    Activitate a{ "ceva","gresit","de_modificat",4 };
    rep.store(Activitate{ "ana","are","mere",1 });
    try {
        rep.modify(a, "tip_gresit", "gresit", 5);
        assert(false);
    }
    catch (RepoException&) {
        assert(true);
    }
    Activitate a3{ "ana","are","mere",1 };
    rep.modify(a3, "mai_are_si", "pere", 2);
    Activitate a2 = rep.find("ana");
    assert(a2.getTime() == 2);
    assert(a2.getDescription() == "pere");

}

void testStergeree() {
    Repo rep{0.0};
    Activitate a{ "ana","are","mere",1 };
    Activitate a1{ "mihai","n-are","mere",1 };
    rep.store(a);
    rep.store(a1);
    rep.del(a);
    assert(rep.getAll().size() == 1);
    try {
        rep.del(Activitate{ "gigi","vine","si_cere",23 });
        assert(false);
    }
    catch (RepoException&) {
        assert(true);
    }
}


void testRandomize() {
    Repo rep{ 0.7 };
    for (int i = 0; i < 1000; i++) {
        try {
            Activitate a{ "activ","tip","descriere",i };
            rep.store(a);
            std::cout << "SUCCESS" << std::endl;
        }
        catch (RepoException& r)
        {
            std::cout << r << std::endl;
        }
    }
}

void testeRepo() {
    testAdauga();
    testCauta();
    testModify();
    testStergeree();
    testRandomize();
}

