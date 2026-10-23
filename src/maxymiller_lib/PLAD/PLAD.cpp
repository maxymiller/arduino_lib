#include <maxymiller_lib/PLAD.h>

String PLAD::filtros(String valor, boolean dePLAD) {
    String filtro = PLAD_FILTROS;
    String valorOutput = "";
    boolean filtroBoolean = false;

    for(int i = 0; i < valor.length(); i++) {
        char caractere = valor.charAt(i);

        if(dePLAD) {
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
            for(int j = 0; j < filtro.length(); j++) {
                if(caractere == filtro.charAt(j)) {
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

String PLAD::filtrosNome(String nome) {
    String filtro = PLAD_FILTROS+"/";
    String valorOutput = "";
    boolean filtroBoolean = false;

    for(int i = 0; i < nome.length(); i++) {
        char caractere = nome.charAt(i);
        boolean filtroBoolean = false;
        for(int j = 0; j < filtro.length(); j++) {
            if(caractere == filtro.charAt(j)) {
                filtroBoolean = true;
                break;
            }
        }
        if(!filtroBoolean) {
            valorOutput += caractere;
        }
    }
    return valorOutput;
}

int PLAD::pladIndex(String nome, String PLAD) {
    String filtro = PLAD_FILTROS;
    boolean filtroBoolean = false;
    int loop = 0;
    int list = 0;
    int index = 0;
    String Nome = "";
    int ok = 0;
    int nomeIndexNumero = 0;
    for(int i = 0; i < nome.length(); i++) {
        if(nome.charAt(i) == '/' && nomeIndexNumero <= 0) {
            nomeIndexNumero = 2;
        }else if(nome.charAt(i) == '/') {
            nomeIndexNumero++;
        }else if(nomeIndexNumero <= 0) {
            nomeIndexNumero++;
        }
    }
    String* nomeIndex = new String[nomeIndexNumero];
    for(int i = 0; i < nome.length(); i++) {
        if(nome.charAt(i) != '/') {
            nomeIndex[loop] += nome.charAt(i);
        }else{
            loop++;
        }
    }
    loop = -1;
    for(int i = 0; i < PLAD.length(); i++) {
        loop++;
        char caractere = PLAD.charAt(i);
        if(!filtroBoolean) {
            if(caractere == '[') {
                list++;
            }else if(caractere == ']') {
                list--;
            }else if(caractere == ',') {
                Nome = "";
                index--;
            }else if(caractere == '=') {
                if(Nome == nomeIndex[ok] && ok == index) {
                    ok++;
                    Nome = "";
                    if(ok >= nomeIndexNumero) {
                        return loop;
                    }
                }else{
                    Nome = "";
                }
                index++;
            }else{
                Nome += caractere;
            }
        }
        if(filtroBoolean) {
            filtroBoolean = false;
        }else if(caractere == '\\') {
            filtroBoolean = true;
        }
    }
    return -1;
}

String PLAD::newString(String nome) {
    return filtrosNome(nome)+"=,";
}

String PLAD::newList(String nome) {
    return filtrosNome(nome)+"=[],";
}

String PLAD::set(String nome, String valor, String PLAD, boolean addCodePLAD) {
    String Nome = "";
    boolean list = false;
    int listNumero = -1;
    String valorOutput = "";
    boolean filtroBoolean = false;
    int numero = 0;
    for(int i = 0; i < nome.length(); i++) {
        if(nome.charAt(i) == '[') {
            list = true;
        }else if(nome.charAt(i) == ']') {
            numero = Nome.toInt();
        }else if(list){
            Nome += nome.charAt(i);
        }
    }
    Nome = "";
    for(int i = 0; i < nome.length(); i++) {
        if(nome.charAt(i) == '[') {
            break;
        }else{
            Nome += nome.charAt(i);
        }
    }
    int index = pladIndex(Nome, PLAD);
    //Serial.println(index);
    if(PLAD.charAt(index+1) == '[' && !list) {
        return "-1";
    }
    int index2 = 0;
    int index3 = index;
    for(int i = 0; i < PLAD.length()-index; i++) {
        char caractere = PLAD.charAt(i+index);
        if(!filtroBoolean) {
            if(!list && caractere == '[') {
                listNumero++;
            }else if(!list && caractere == ']') {
                listNumero--;
            }
            if(index <= -1) {
                return String(index);
            }if(list && caractere == ',') {
                //Serial.println(PLAD.substring(i+index-2, i+index+1));
                if(PLAD.substring(i+index-2, i+index+1) == "],,") {
                    return "-1";
                }
                listNumero++;
                index2=index+i;
                if(listNumero >= numero) {
                    index3+=1;
                    index2-=1;
                    /*Serial.println(PLAD.substring(index3, index3+2));
                    Serial.println(PLAD.substring(index3-1, index3+1));
                    Serial.println(PLAD.substring(index2, index2+2));
                    Serial.println(PLAD.substring(index2-1, index2+1));

                    Serial.println(PLAD.charAt(index+i));
                    Serial.println(PLAD.charAt(index+i-1));
                    Serial.println(PLAD.charAt(index+i-2));
                    Serial.println(PLAD.charAt(index3+1));
                    Serial.println(PLAD.charAt(index3));
                    Serial.println(PLAD.charAt(index3-1));*/

                    if(PLAD.substring(index3-1, index3+1) == "=[") {
                        index3+=1;
                    }
                    if(PLAD.substring(index2, index2+2) != "],") {
                        index2+=2;
                    }
                    /*Serial.println(PLAD.charAt(i+index+1));
                    Serial.println(PLAD.charAt(i+index+2));
                    Serial.println(listNumero);
                    Serial.println(numero);*/
                    
                    valorOutput = PLAD.substring(0, index3);
                    if(addCodePLAD) {
                        valorOutput += valor+",";
                    }else{
                        valorOutput += filtros(valor, false)+",";
                    }
                    valorOutput += PLAD.substring(index2);
                    break;
                }
                index3 = index+i;
            }else if(caractere == ','){
                if(listNumero <= -1) {
                    index2 = i+index;
                    valorOutput = PLAD.substring(0, index+1);
                    if(addCodePLAD) {
                        valorOutput += valor;
                    }else{
                        valorOutput += filtros(valor, false);
                    }
                    if(PLAD.charAt(index2+1) == ',') {
                        valorOutput += PLAD.substring(index2+1);
                    }else{
                        valorOutput += PLAD.substring(index2);
                    }
                    break;
                }
            }
        }
        if(filtroBoolean) {
            filtroBoolean = false;
        }else if(caractere == '\\') {
            filtroBoolean = true;
        }
    }
    return valorOutput;
}

String PLAD::getListIndex(String PLAD) {
    boolean filtroBoolean = false;
    boolean filtroBoolean2 = false;
    //String filtro = PLAD_FILTROS;
    String nome = "";
    String valorOutput = "";
    String valorInput = "";
    boolean list = false;
    int listNumero = 0;
    for (int i = 0; i < PLAD.length(); i++) {
        char caractere = PLAD.charAt(i);

        /*if(filtroBoolean) {
            for(int j = 0; j < filtro.length(); j++) {
                if(caractere == filtro.charAt(j)) {
                    filtroBoolean2 = true;
                    break;
                }
            }
        }*/
        if(filtroBoolean && !filtroBoolean2) {
            filtroBoolean = false;
        }
        if(filtroBoolean2) {
            filtroBoolean2 = false;
        }else if(caractere == '\\') {
            filtroBoolean = true;
            filtroBoolean2 = true;
        }
        if(!filtroBoolean) {
            if(caractere == '[') {
                list = true;
                listNumero = 0;
            }else if(PLAD.substring(i-1, i+1) == "],") {
                list = false;
                nome += "["+String(listNumero)+"]";
            }else if(list && caractere == ',') {
                listNumero++;
            }else if(!list && caractere == ',') {
                valorOutput += nome + ",";
                int indexLoop = nome.length();
                for(int j = 0; j < nome.length(); j++) {
                    indexLoop--;
                    if(nome.charAt(nome.length()-j) == '/') {
                        break;
                    }
                }
                nome = nome.substring(0, indexLoop);
                valorInput = "";
            }else if(caractere == '=') {
                if(nome != "") {
                    nome += "/";
                }
                nome += valorInput;
                valorInput = "";
            }else{
                valorInput += caractere;
            }
        }
    }
    return valorOutput;
}

String PLAD::getString(String nome, String PLAD) {
    String Nome = "";
    boolean list = false;
    int listNumero = -1;
    String valorOutput = "";
    boolean filtroBoolean = false;
    int numero = 0;
    for(int i = 0; i < nome.length(); i++) {
        if(nome.charAt(i) == '[') {
            list = true;
        }else if(nome.charAt(i) == ']') {
            numero = Nome.toInt();
        }else if(list){
            Nome += nome.charAt(i);
        }
    }
    Nome = "";
    for(int i = 0; i < nome.length(); i++) {
        if(nome.charAt(i) == '[') {
            break;
        }else{
            Nome += nome.charAt(i);
        }
    }
    int index = pladIndex(Nome, PLAD);
    //Serial.println(index);
    if(PLAD.charAt(index+1) == '[' && !list) {
        return "-1";
    }
    int index2 = 0;
    int index3 = index;
    for(int i = 0; i < PLAD.length()-index; i++) {
        char caractere = PLAD.charAt(i+index);
        if(!filtroBoolean) {
            if(!list && caractere == '[') {
                listNumero++;
            }else if(!list && caractere == ']') {
                listNumero--;
            }
            if(index <= -1) {
                return String(index);
            }if(list && caractere == ',') {
                //Serial.println(PLAD.substring(i+index-2, i+index+1));
                if(PLAD.substring(i+index-2, i+index+1) == "],,") {
                    return "-1";
                }
                listNumero++;
                index2=index+i;
                if(listNumero >= numero) {
                    index3+=1;
                    index2-=1;

                    if(PLAD.substring(index3-1, index3+1) == "=[") {
                        index3+=1;
                    }
                    if(PLAD.substring(index2, index2+2) != "],") {
                        index2+=2;
                    }
                    
                    valorOutput = PLAD.substring(index3, index2-1);
                    valorOutput = filtros(valorOutput, true);
                    break;
                }
                index3 = index+i;
            }else if(caractere == ','){
                if(listNumero <= -1) {
                    index2 = i+index;
                    valorOutput = PLAD.substring(index+1, index2);
                    
                    
                    if(PLAD.charAt(index2+1) == ',') {
                        valorOutput += ",";
                    }
                    valorOutput = filtros(valorOutput, true);
                    break;
                }
            }
        }
        if(filtroBoolean) {
            filtroBoolean = false;
        }else if(caractere == '\\') {
            filtroBoolean = true;
        }
    }
    return valorOutput;
}
