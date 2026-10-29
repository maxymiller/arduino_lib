#ifndef ABCLIST_H
#define ABCLIST_H

#include <Arduino.h>

class ABCList {
private:
    int ABC_NUMEOR = 52;
public:
    String listABC(int numero);
    String listInt(String abc);
    String list(int numero);
};

#endif