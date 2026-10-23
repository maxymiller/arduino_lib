#ifndef PLAD_H
#define PLAD_H

#include <Arduino.h>

class PLAD {
private:
    //String filtros(String valor, boolean dePLAD);
    String PLAD_FILTROS = "\"[],\\=";
    String filtros(String valor, boolean dePLAD);
    String filtrosNome(String nome);
public:
    int pladIndex(String nome, String PLAD);
    String newString(String nome);
    String newList(String nome);
    String set(String nome, String valor, String PLAD, boolean addCodePLAD);
    String getListIndex(String PLAD);
    String getString(String nome, String PLAD);
};

#endif