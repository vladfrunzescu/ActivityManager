#include "ui.hpp"

#include "domain.hpp"
#include <iostream>
#include <string>
#include <string.h>

using std::string;
using std::cout;
using std::cin;


void ConsolUI::tipareste(const vector<Activitate>& activs) {
    if (activs.size() == 0) {
        cout << "\nN-avem asa ceva\n";
        cout << "\n";
        return;
    }
    cout << "\nActivitati:\n";
    for (const auto& activ : activs) {
        cout << activ.getTitle() << ' ' << activ.getType() << ' ' << activ.getDescription() << ' ' << activ.getTime() << '\n';
    }
    cout << "\n";

}

void ConsolUI::adaugaUI() {
    string type, description, title;
    string string_time;
    while (1) {
        cout << "Dati titlu:";
        cin >> title;
        cout << "Dati tip:";
        cin >> type;
        cout << "Dati descriere:";
        cin >> description;
        cout << "dati si o durata:";
        cin >> string_time;
        const int time = stoi(string_time);
        if (time == 0 && string_time != "0")
        {
            cout << "Durata trebuie sa fie pozitiva sau macar numar te rog!\n\n";
            continue;
        }
        ctr.addActivitate(title, type, description, time);
        cout << "\nAdaugat cu succes\n\n";
        return;
    }
}

void ConsolUI::modificare() {
    string  type, title, description;
    string string_time;
    int time = 0;
    while (1) {
        cout << "Dati titlul activitatii pe care doriti sa il modificati: ";
        cin >> title;
        cout << "Dati noul tip : ";
        cin >> type;
        cout << "Adaugati noua descriere: ";
        cin >> description;
        cout << "Adaugati noua durata: ";
        cin >> string_time;
        try {
            time = stoi(string_time);
            ctr.modifyActivitate(title, type, description, time);
            return;
        }
        catch (const std::invalid_argument&) {
            cout << "\nNumar intreg te rog!!! \n\n";
        }
    }
}

void ConsolUI::cauta() {
    string type, title;
    cout << "Introduceti titlul activitatii pe care o cautati: ";
    cin >> title;


    Activitate a = ctr.findActivity(title);
    cout << a.getTitle() << " " << a.getType() << " " << a.getDescription() << " " << a.getTime() << "\n";

}


void ConsolUI::filtrare() {
    string comanda;
    cout << "Introduceti modul de filtrare (descriere/tip): ";
    cin >> comanda;
    vector<Activitate> activs;
    if (comanda != "descriere" && comanda != "tip")
    {
        cout << "\nSunteti degeaba!\n";
        return;
    }
    if (comanda == "tip") {
        cout << "Introduceti un tip dupa care doriti filtrarea:";
        string type;
        cin >> type;

        activs = ctr.filtrareType(type);
    }
    else if (comanda == "descriere")
    {
        string descriere;
        cout << "Introduceti descrierea: ";
        cin >> descriere;
        activs = ctr.filtrareDescriere(descriere);
    }
    if (activs.size() == 0)
    {
        cout << "\nN-avem asa ceva\n";
        return;
    }
    for (const auto& activ : activs) {
        cout << activ.getTitle() << " " << activ.getType() << " " << activ.getDescription() << " " << activ.getTime() << "\n";
    }

}

void ConsolUI::stergere()
{
    cout << "Introduceti titlul activitatii pe care doriti sa o stergeti \n";
    string title, type;
    cout << "Title: "; cin >> title;

    ctr.deletee(title);
}



void ConsolUI::rapoarte() {
    vector<DTO>dtosss = ctr.rapoarte();
    cout << '\n';
    for (auto elem : dtosss)
        cout << elem.getType() << " " << elem.getVal() << '\n';
    cout << '\n';


}

void ConsolUI::adauga_in_lista() {

    string titlu;
    cout << "Cititi va rog titlul activitatii pe care doriti sa o introduceti: ";
    cin >> titlu;
    tipareste(ctr.addActivitateLista(titlu));

}


void ConsolUI::goleste_lista() {
    ctr.goleste_lista();
    cout << "\nLista vida!\n\n";
}

void ConsolUI::genereaza_lista() {
    string nr;
    cout << "Introduceti numarul de elemente: "; cin >> nr;
    try {
        const unsigned int numar = stoi(nr);

        if (ctr.genereaza_lista(numar) == -1) {
            cout << "\nCerere invaliad!\n\n";
            return;
        }
        tipareste(ctr.get_all_list());
    }
    catch (const std::invalid_argument&) {
        cout << "\nNumar intreg te rog!!!\n\n";
    }
}



void ConsolUI::exporta() {
    string filename;
    cout << "Introduceti numele fisierului: "; cin >> filename;
    ctr.export_files(filename);
    tipareste(ctr.get_all_list());

}

void ConsolUI::undo() {
    ctr.undo();
}

void ConsolUI::start() {
    while (true) {
        cout << "Meniu:\n";
        cout << "1 Adauga\n2 Tipareste\n3 Modifica\n4 Cauta\n5 Filtare dupa descriere/tip\n6 Stergere\n7 Sortare dupa titlu\n8 Sortare dupa descriere\n9 Sortare dupa tip+durata\n10 Statistici\n11 Adauga in lista de activitati de azi\n12 Goleste lista curenta de activitati\n13 Genereaza random lista\n14 Exporta\n15 Undo\n0 Iesire\nDati comanda:";

        string comanda;
        cin >> comanda;

        try {
            const int cmd = stoi(comanda);
            try {
                switch (cmd) {
                case 1:
                    adaugaUI();
                    break;
                case 2:
                    tipareste(ctr.getAll());
                    break;
                case 3:
                    modificare();
                    break;
                case 4:
                    cauta();
                    break;
                case 5:
                    filtrare();
                    break;
                case 6:
                    stergere();
                    break;
                case 7:
                    tipareste(ctr.sortByTitle());
                    break;
                case 8:
                    tipareste(ctr.sortByDescription());
                    break;
                case 9:
                    tipareste(ctr.sortByTypeTime());
                    break;
                case 10:
                    rapoarte();
                    break;
                case 11:
                    adauga_in_lista();
                    break;
                case 12:
                    goleste_lista();
                    break;
                case 13:
                    genereaza_lista();
                    break;
                case 14:
                    exporta();
                    break;
                case 15:
                    undo();
                    break;
                case 0:
                    return;
                default:
                    cout << "Comanda invalida\n";
                }
            }
            catch (const RepoException& ex) {
                cout << ex << '\n';
            }
            catch (const ValidateException& ex) {
                cout << ex << '\n';
            }
        }
        catch (const std::invalid_argument&) {
            cout << "\nInvalid commmand\n\n";
        }
    }
}
