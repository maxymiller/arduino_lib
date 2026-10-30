#ifndef CPRGD_H
#define CPRGD_H

#include <Arduino.h>
#include <maxymiller_lib/ABCList.h>

class CPRGD {
private:
    ABCList abcList;
    String FILROS_VALOR = "\\:;";
    String TEXTO_INDEX = "";
    String CODE_INDEX = "";
    String filros(String texto, boolean deCPRGD);
public:
    void compressao(String texto, String nao_comecar, String nao_fim);
    String deCompressao(String textoIndex, String codeIndex);
    String getTextoIndex();
    String getCodeIndex();
};

#endif