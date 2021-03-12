#include "validator.hpp"
#include <assert.h>
#include <sstream>

void ActivitateValidator::validate(const Activitate& a) {
    vector<string> msgs;
    if (a.getTime() < 0) msgs.push_back("Timp negativ!!!");
    if (a.getType().size() == 0) msgs.push_back("Tip vid!!!");
    if (a.getDescription().size() == 0) msgs.push_back("Descriere invalida!!!");
    if (a.getTitle().size() == 0) msgs.push_back("Titlu invalid!!!");
    if (msgs.size() > 0) {
        throw ValidateException(msgs);
    }
}

ostream& operator<<(ostream& out, const ValidateException& ex) {
    for (const auto& msg : ex.msgs) {
        out << msg << " ";
    }
    return out;
}

void testValidator() {
    ActivitateValidator v;
    Activitate a{ "","", "",-1 };
    try {
        v.validate(a);
    }
    catch (const ValidateException& ex) {
        std::stringstream sout;
        sout << ex;
        auto const mesaj = sout.str();
        assert(mesaj.find("negativ") >= 0);
        assert(mesaj.find("vid") >= 0);
    }

}
