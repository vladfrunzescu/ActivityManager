#pragma once
#include <string>
#include "domain.hpp"
#include <vector>

using std::vector;
using std::string;
using std::ostream;

class ValidateException {
    vector<string> msgs;
public:
    ValidateException(const vector<string>& errors) :msgs{ errors } {}
    //functie friend (vreau sa folosesc membru privat msg)
    friend ostream& operator<<(ostream& out, const ValidateException& ex);
};

ostream& operator<<(ostream& out, const ValidateException& ex);

class ActivitateValidator {
public:
    void validate(const Activitate& p);
};

void testValidator();
