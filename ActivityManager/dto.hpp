#pragma once
#include <string>
using std::string;

class DTO {
private:
    string tip;
    int val;

public:
    //constructor
    DTO(const string tip, int val) : tip{ tip }, val{ val }{};
    
    //returneaza tupul dto-ului
    string getType() const {
        return tip;
    }

    //returneaza valoarea dto-ului
    int getVal() const noexcept {
        return val;
    }
    
    //seteaza valoarea dto-ului
    void setVal(int vals) noexcept {
        this->val += vals;
    }


};
