
#include <stdlib.h>

#include "ui.hpp"
#include "service.hpp"
#include "repo.hpp"
#include "validator.hpp"
#include "listaActuala.hpp"
#include <ctime>
#include <cstdlib>



void testAll() {
    testeRepo();
    testCtr();
    testValidator();
    testListaActuala();
}

int main()
    {
        testAll();
        srand(static_cast <unsigned> (time(0)));
        float p = 0.0;
        try {
            FileRepo rep{ "fisier.txt",p };
            ActivitateValidator val;
            ListaActuala lista_aux;
            Service ctr{ rep,val,lista_aux };
            ConsolUI ui{ ctr };
            ui.start();
        }
        catch (RepoException& r) {
            std::cout << r;
        }
    }
        
    

    

