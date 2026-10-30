#include "maxymiller_lib/CPRGD.h"

void CPRGD::compressao(String texto, String nao_comecar, String nao_fim) {
    TEXTO_INDEX = "";
    CODE_INDEX = "";
    boolean naoComecarFim = false;
    boolean naoFim = false;
    boolean comecar = false;
    int index = 0;
    String valor = "";
    for(int i = 0; i < texto.length(); i++) {
        char caractere = texto.charAt(i);

        naoComecarFim = false;
        if(!comecar) {
            for(int j = 0; j < nao_comecar.length(); j++) {
                if(caractere == nao_comecar.charAt(j)) {
                    naoComecarFim = true;
                    break;
                }
            }
        }

        if(comecar && !naoComecarFim) {
            for(int j = 0; j < nao_fim.length(); j++) {
                if(caractere == nao_fim.charAt(j)) {
                    naoComecarFim = true;
                    naoFim = true;
                    break;
                }
            }
        }
        if(i >= texto.length()-1) {
            //Serial.println(i);
            //Serial.println(texto.length());
            naoComecarFim = true;
            naoFim = true;
        }

        if(naoComecarFim && valor == "" && !comecar) {
            index++;
            //valor += abcList.listABC(index)+":";
            valor += ":";
        }
        if(naoComecarFim && !comecar){
            valor += caractere;
        }
        if(valor != "" && naoComecarFim && comecar) {
            naoComecarFim = false;
            comecar = false;
            valor += caractere;
        }
        if(valor != "" && !naoComecarFim && !comecar) {
            valor += ";";
            if(TEXTO_INDEX.indexOf(filros(valor.substring(valor.indexOf(":")+1, valor.length()-1), false)) <= -1) {
                //Serial.println("1:::"+valor.substring(valor.indexOf(":")+1, valor.length()-1));
                TEXTO_INDEX += abcList.listABC(index) + ":" + filros(valor.substring(valor.indexOf(":")+1, valor.length()-1), false) + ";";
                //Serial.println(TEXTO_INDEX);
            }
            valor = "";
        }

        if(!naoComecarFim) {
            if(!comecar) {
                if(TEXTO_INDEX.indexOf(abcList.listABC(index)+":") >= 0 || TEXTO_INDEX.indexOf(abcList.listABC(1)+":") < 0) {
                    index++;
                }
                valor += ":";
            }
            
            if(!naoFim) {
                valor += caractere;
            }


            comecar = true;
            naoFim = false;

            if(!comecar) {
                for(int j = 0; j < nao_fim.length(); j++) {
                    if(caractere == nao_fim.charAt(j)) {
                        naoComecarFim = true;
                        break;
                    }
                }
            }
        }
    }

    valor = "";
    for(int i = 0; i < texto.length(); i++) {
        char caractere = texto.charAt(i);
        valor += filros(String(caractere), false);

        index = TEXTO_INDEX.indexOf(":"+valor+";");
        if(index >= 0) {
            int loop = 0;
            while(index - loop > 0 && TEXTO_INDEX.charAt(index - loop - 1) != ';') {
                loop++;
            }
            CODE_INDEX += TEXTO_INDEX.substring(index-loop, index)+",";
            valor = "";
        }
    }
}

String CPRGD::deCompressao(String textoIndex, String codeIndex) {
    String valor = "";
    String valorOutput = "";
    boolean filorBoolean = false;
    for(int i = 0; i < codeIndex.length(); i++) {
        char caractere = codeIndex.charAt(i);
        

        if(caractere == ',') {
            int indexABC = valor.length();
            int index = textoIndex.indexOf(valor+":");
            int index2 = 0;
            for(int j = 0; j < textoIndex.length()-index; j++) {
                char caractere2 = textoIndex.charAt(j+index);
                if(!filorBoolean) {
                    if(caractere2 == ';') {
                        index2 = index+j;
                        break;
                    }
                }

                if(filorBoolean) {
                    filorBoolean = false;
                }else if(caractere2 == '\\') {
                    filorBoolean = true;
                }
            }
            valorOutput += textoIndex.substring(index+indexABC+1, index2);
            valor = "";
        }else{
            valor += caractere;
        }
    }
    return filros(valorOutput, true);
}

String CPRGD::filros(String texto, boolean deCPRGD) {
    String valorOutput = "";
    boolean filtroBoolean = false;
    for(int i = 0; i < texto.length(); i++) {
        char caractere = texto.charAt(i);
        if(deCPRGD) {
            if(filtroBoolean) {
                filtroBoolean = false;
            }else if(caractere == '\\') {
                filtroBoolean = true;
            }

            if(!filtroBoolean) {
                valorOutput += caractere;
            }
        }else{
            filtroBoolean = false;
            for(int j = 0; j < FILROS_VALOR.length(); j++) {
                if(caractere == FILROS_VALOR.charAt(j)) {
                    filtroBoolean = true;
                    break;
                }
            }

            if(filtroBoolean) {
                valorOutput += "\\";
            }
            valorOutput += caractere;
        }
    }
    return valorOutput;
}

String CPRGD::getTextoIndex() {
    return TEXTO_INDEX;
}
String CPRGD::getCodeIndex() {
    return CODE_INDEX;
}