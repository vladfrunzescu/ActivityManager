#include "listaActuala.hpp"

void testListaActuala()
{
    ListaActuala l;
    assert(l.getAll().size() == 0);
    Activitate a{ "fotbal","sportiv","super",10 };
    l.add(a);
    assert(l.getAll().size() == 1);
    Activitate b{ "fotbal","sportiv","ok",432 };
    try {
        l.add(b);
        assert(false);
    }
    catch (RepoException&) {
        assert(true);
    }
    l.golesteLista();
    assert(l.getAll().size() == 0);
    
    l.add(a);
    l.del(a);
    assert(l.getAll().size() == 0);

    vector<Activitate> repo;
    repo.push_back(a);
    Activitate c{ "baschet","sportiv","superr",45 };
    Activitate d{ "cititi","cultural","exceptional",124 };
    repo.push_back(c);
    repo.push_back(d);
    assert(l.umple(2, repo) == 0);
    assert(l.getAll().size() == 2);

}
