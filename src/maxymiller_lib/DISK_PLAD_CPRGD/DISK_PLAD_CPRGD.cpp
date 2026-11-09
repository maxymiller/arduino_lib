#include "maxymiller_lib/DISK_PLAD_CPRGD.h"

boolean DISK_PLAD_CPRGD::boot() {
    /*if(!begin()) {
        return false;
    }*/
    if(!LittleFS.exists("/DISK_PLAD_CPRGD")) {
        LittleFS.mkdir("/DISK_PLAD_CPRGD");
    }
    if(getRemoveBoot()) {
        if(getErro() >= 10) {
            if(!removeTexto(getRemoveNomeFS(), getRemoveNome())) {
                setErro(getErro()+1);
                return false;
            }
        }else{
            setRemoveBoot(false);
        }
    }
    if(onSave() == 0 && (getNomeFS() == "" || getNome() == "")) {
        setErro(0);
    }
    
    return true;
}

bool DISK_PLAD_CPRGD::begin() {
    if (onBegin) {
        return true;
    }

    if (LittleFS.begin(false)) {
        onBegin = true;
    }
    return onBegin;
}

void DISK_PLAD_CPRGD::setMegErro(bool modo) {
    MEG_ERRO = modo;
}

bool DISK_PLAD_CPRGD::criarArquivo(String nomeFS, bool tipo) {
    if(!begin()) {
        return false;
    }
    File arquivo;
    if(tipo) {
        if(LittleFS.exists("/DISK_PLAD_CPRGD/"+nomeFS+".txt")) {
            return true;
        }
        arquivo = LittleFS.open("/DISK_PLAD_CPRGD/"+nomeFS+".txt", "w");
        if(!arquivo) {
            return false;
        }
    }else{
        if(LittleFS.exists("/DISK_PLAD_CPRGD/"+nomeFS)) {
            return true;
        }
        arquivo = LittleFS.open("/DISK_PLAD_CPRGD/"+nomeFS, "w");
        if(!arquivo) {
            return false;
        }
    }
    arquivo.close();
    return true;
}

int DISK_PLAD_CPRGD::onSave() {
    memo.begin("DISK_PLAD_CPRGD", true);
    int outPut = memo.getInt("DPCSave", 0);
    memo.end();
    return outPut;
}
void DISK_PLAD_CPRGD::setSave(int numero) {
    memo.begin("DISK_PLAD_CPRGD", false);
    memo.putInt("DPCSave", numero);
    memo.end();
}

int DISK_PLAD_CPRGD::onLoad() {
    memo.begin("DISK_PLAD_CPRGD", true);
    int outPut = memo.getInt("DPCLoad", 0);
    memo.end();
    return outPut;
}
void DISK_PLAD_CPRGD::setLoad(int numero) {
    memo.begin("DISK_PLAD_CPRGD", false);
    memo.putInt("DPCLoad", numero);
    memo.end();
}

int DISK_PLAD_CPRGD::onSaveLoadOk() {
    memo.begin("DISK_PLAD_CPRGD", false);
    int outPut = memo.getInt("DPCOk", 0);
    //memo.putInt("DPCOk", 0);
    memo.end();
    return outPut;
}
void DISK_PLAD_CPRGD::setSaveLoadOk(int numero) {
    memo.begin("DISK_PLAD_CPRGD", false);
    memo.putInt("DPCOk", numero);
    memo.end();
}

String DISK_PLAD_CPRGD::getNome() {
    memo.begin("DISK_PLAD_CPRGD", true);
    String outPut = memo.getString("DPCNome", "");
    memo.end();
    return outPut;
}
String DISK_PLAD_CPRGD::getNomeFS() {
    memo.begin("DISK_PLAD_CPRGD", true);
    String outPut = memo.getString("DPCNomeFS", "");
    memo.end();
    return outPut;
}
void DISK_PLAD_CPRGD::setNomeFS(String nomeFS, String nome) {
    memo.begin("DISK_PLAD_CPRGD", false);
    memo.putString("DPCNomeFS", nomeFS);
    memo.putString("DPCNome", nome);
    memo.end();
}

String DISK_PLAD_CPRGD::filtros(String texto) {
    String valor = "";
    boolean filenoBoolean = false;
    for(int i = 0; i < texto.length(); i++) {
        char inPut = texto.charAt(i);
        filenoBoolean = false;
        for(int j = 0; j < filtro.length(); j++) {
            if(inPut == filtro.charAt(j)) {
                filenoBoolean = true;
                break;
            }
        }
        if(!filenoBoolean) {
            valor+=inPut;
        }
    }
    return valor;
}
int DISK_PLAD_CPRGD::indexLinha(String nomeFS, String nome) {
    int loop = -1;
    String texto = "";
    if(!begin()) {
        if(MEG_ERRO) {
            Serial.println("Erro ao montar o LittleFS");
        }
        return -1;
    }
    File arquivo = LittleFS.open("/DISK_PLAD_CPRGD/"+nomeFS+".txt", "r");
    if(!arquivo) {
        if(MEG_ERRO) {
            Serial.println("Erro ao abrir "+nomeFS);
        }
        return -1;
    }
    while (arquivo.available()) {
        String valor = arquivo.readStringUntil('\n');

        loop++;

        int separador = valor.indexOf("}:");

        if (separador == -1) {
            continue;
        }

        int inicio = valor.indexOf('{');

        if (inicio == -1 || inicio >= separador) {
            continue;
        }

        texto = valor.substring(inicio + 1, separador);
        if(texto == nome) {
            arquivo.close();
            return loop;
        }
    }

    arquivo.close();
    return -1;
}

boolean DISK_PLAD_CPRGD::removeTexto(String nomeFS, String nome) {
    if(nomeFS == "" || nome == "") {
        return false;
    }
    if(!begin()) {
        if(MEG_ERRO) {
            Serial.println("Erro ao montar o LittleFS");
        }
        return false;
    }

    String valor = "";
    /*if(LittleFS.exists("/DISK_PLAD_CPRGD/REMOVE") && LittleFS.exists("/DISK_PLAD_CPRGD/"+nomeFS+".txt")) {
        if(!LittleFS.remove("/DISK_PLAD_CPRGD/REMOVE")) {
            return false;
        }
    }*/

    if(LittleFS.exists("/DISK_PLAD_CPRGD/"+nomeFS+".txt")) {
        File arquivo = LittleFS.open("/DISK_PLAD_CPRGD/"+nomeFS+".txt", "r");
        File arquivo2 = LittleFS.open("/DISK_PLAD_CPRGD/REMOVE", "w");
        if(!arquivo || !arquivo2) {
            if(MEG_ERRO) {
                Serial.println("Erro ao abrir");
            }
            return false;
        }

        if(getRemoveNomeFS() != nomeFS || getRemoveNome() != nome) {
            setRemoveNomeFS(nomeFS, nome);
        }
        setRemoveBoot(true);
        
        while(arquivo.available()) {
            String valor = arquivo.readStringUntil('\n');

            int separador = valor.indexOf("}:");

            if (separador == -1) {
                continue;
            }

            int inicio = valor.indexOf('{');

            if (inicio == -1 || inicio >= separador) {
                continue;
            };

            int numeroList = valor.indexOf("[");

            if (numeroList == -1 || numeroList >= separador) {
                continue;
            }

            if (valor.substring(inicio + 1, numeroList) == nome) {
                continue;
            }

            arquivo2.print(valor+"\n");

        }

        arquivo.close();
        arquivo2.close();
    }
    
    if(LittleFS.exists("/DISK_PLAD_CPRGD/"+nomeFS+".txt") && LittleFS.exists("/DISK_PLAD_CPRGD/REMOVE")) {
        if(!LittleFS.remove("/DISK_PLAD_CPRGD/"+nomeFS+".txt")) {
            return false;
        }
    }
    if(!LittleFS.exists("/DISK_PLAD_CPRGD/REMOVE")) {
        return false;
    }
    if(!LittleFS.rename("/DISK_PLAD_CPRGD/REMOVE", "/DISK_PLAD_CPRGD/"+nomeFS+".txt")) {
        return false;
    }

    setRemoveNomeFS("", "");
    setRemoveBoot(false);
    return true;
}
void DISK_PLAD_CPRGD::setRemoveNomeFS(String nomeFS, String nome) {
    memo.begin("DISK_PLAD_CPRGD", false);
    memo.putString("DPCRmNomeFS", nomeFS);
    memo.putString("DPCRmNome", nome);
    memo.end();
}
String DISK_PLAD_CPRGD::getRemoveNomeFS() {
    String outPut = "";
    memo.begin("DISK_PLAD_CPRGD", true);
    outPut = memo.getString("DPCRmNomeFS", "");
    memo.end();
    return outPut;
}
String DISK_PLAD_CPRGD::getRemoveNome() {
    String outPut = "";
    memo.begin("DISK_PLAD_CPRGD", true);
    outPut = memo.getString("DPCRmNome", "");
    memo.end();
    return outPut;
}
void DISK_PLAD_CPRGD::setRemoveBoot(bool boot) {
    memo.begin("DISK_PLAD_CPRGD", false);
    memo.putBool("DPCRmBoot", boot);
    memo.end();
}
bool DISK_PLAD_CPRGD::getRemoveBoot() {
    bool outPut = false;
    memo.begin("DISK_PLAD_CPRGD", true);
    outPut = memo.getBool("DPCRmBoot", false);
    memo.end();
    return outPut;
}

int DISK_PLAD_CPRGD::getErro() {
    int outPut = 0;
    memo.begin("DISK_PLAD_CPRGD", true);
    outPut = memo.getInt("DPCError", 0);
    memo.end();
    return outPut;
}
void DISK_PLAD_CPRGD::setErro(int error) {
    memo.begin("DISK_PLAD_CPRGD", false);
    memo.putInt("DPCError", error);
    memo.end();
}

void DISK_PLAD_CPRGD::save(String nomeFS, String nome) {
    if(nomeFS == "" || nome == "") {
        return;
    }
    String Nome = filtros(nome);
    String valor = "";
    for(int i = 0; i < Nome.length(); i++) {
        char caractere = Nome.charAt(i);

        if(caractere == '[') {
            continue;
        }
        else if(caractere == ']') {
            continue;
        }
        else{
            valor += caractere;
        }
    }
    Nome = valor;
    valor = "";
    if (!begin()) {
        if(MEG_ERRO) {
            Serial.println("Erro ao montar o LittleFS");
        }
    }else{

        setNomeFS(nomeFS, Nome);
        setSave(0);
    }
}

void DISK_PLAD_CPRGD::load(String nomeFS, String nome) {
    if(nomeFS == "" || nome == "") {
        return;
    }
    String Nome = filtros(nome);
    String valor = "";
    for(int i = 0; i < Nome.length(); i++) {
        char caractere = Nome.charAt(i);

        if(caractere == '[') {
            continue;
        }
        else if(caractere == ']') {
            continue;
        }
        else{
            valor += caractere;
        }
    }
    Nome = valor;
    valor = "";
    if (!begin()) {
        if(MEG_ERRO) {
            Serial.println("Erro ao montar o LittleFS");
        }
    }else{

        setNomeFS(nomeFS, Nome);
        setLoad(0);
    }
}

boolean DISK_PLAD_CPRGD::setSaveTexto(String texto, boolean limpar) {
    int numero = onSave();
    if(!criarArquivo(getNomeFS())) {
        return false;
    }
    if(numero == 0) {
        if (!begin()) {
            if(MEG_ERRO) {
                Serial.println("Erro ao montar o LittleFS");
            }
            return false;
        }else{
            File arquivo;
            if(limpar) {
                arquivo = LittleFS.open("/DISK_PLAD_CPRGD/SAVE0", "w");
            }else{
                arquivo = LittleFS.open("/DISK_PLAD_CPRGD/SAVE0", "a");
            }
            if (!arquivo) {
                if(MEG_ERRO) {
                    Serial.println("Erro ao abrir SAVE");
                }
                return false;
            }else{
                for(int i = 0; i < texto.length(); i++) {
                    char caractere = texto.charAt(i);

                    if(caractere == '\\') {
                        arquivo.print("\\\\");
                    }
                    else if(caractere == '\n') {
                        arquivo.print("\\n");
                    }
                    else if(caractere == '\r') {
                        arquivo.print("\\r");
                    }
                    else{
                        arquivo.print(caractere);
                    }
                }
                arquivo.print("\n");
                arquivo.close();
            }
        }
        return true;
    }
    else if(numero == 1) {
        if (!begin()) {
            if(MEG_ERRO) {
                Serial.println("Erro ao montar o LittleFS");
            }
            return false;
        }else{
            File arquivo;
            if(limpar){
                arquivo = LittleFS.open("/DISK_PLAD_CPRGD/SAVE", "w");
            }else{
                arquivo = LittleFS.open("/DISK_PLAD_CPRGD/SAVE", "a");
            }
            if (!arquivo) {
                if(MEG_ERRO) {
                    Serial.println("Erro ao abrir SAVE");
                }
                return false;
            }else{
                arquivo.print(texto);
                arquivo.close();
            }
        }
        return true;
    }
    return false;
}
void DISK_PLAD_CPRGD::endSave(boolean fim) {
    int numero = onSave();
    if((getNomeFS() == "" || getNome() == "") && numero == 0) {
        ESP.restart();
        return;
    }
    //int numeroOnSaveLoadOk = onSaveLoadOk();
    //setSaveLoadOk(numeroOnSaveLoadOk);
    if(fim) {
        if(numero == 0) {
            setSave(1);
        }
        else if(numero == 1) {
            setSave(2);
        }
        else if(numero == 2) {
            LittleFS.remove("/DISK_PLAD_CPRGD/SAVE");
            LittleFS.remove("/DISK_PLAD_CPRGD/SAVE0");
            setNomeFS("", "");
            setSave(0);
        }
        setErro(0);
    }else{
        if(getErro() >= 10) {
            setNomeFS("", "");
            setSave(0);
            setSaveLoadOk(-1);
            setErro(0);
        }else{
            setErro(getErro()+1);
        }
    }
    ESP.restart();
}

void DISK_PLAD_CPRGD::endLoad(boolean fim) {
    int numero = onLoad();
    if((getNomeFS() == "" || getNome() == "") && numero == 0) {
        ESP.restart();
        return;
    }
    
    if(fim) {
        if(numero == 0) {
            setLoad(1);
        }
        else if(numero == 1) {
            setLoad(2);
        }
        else if(numero == 2) {
            LittleFS.remove("/DISK_PLAD_CPRGD/LOAD");
            setNomeFS("", "");
            setLoad(0);
        }
        setErro(0);
        if(numero == 2) {
            return;
        }
    }else{
        if(getErro() >= 10) {
            setNomeFS("", "");
            setLoad(0);
            setSaveLoadOk(-1);
            setErro(0);
        }else{
            setErro(getErro()+1);
        }
    }
    ESP.restart();
}

void DISK_PLAD_CPRGD::setLinhaVer(int indexA, int indexB) {
    INDEX_A = indexA;
    INDEX_B = indexB;
}

String DISK_PLAD_CPRGD::postSave(String filroCPRGD) {
    int numero = onSave();
    setSaveLoadOk(0);
    String texto = "";
    if (!begin()) {
        setSaveLoadOk(-1);
        if(MEG_ERRO) {
            Serial.println("Erro ao montar o LittleFS");
        }
    }else{
        switch (numero) {
        case 1: {
                File arquivo = LittleFS.open("/DISK_PLAD_CPRGD/SAVE0", "r");
                if (!arquivo) {
                    setSaveLoadOk(-2);
                    if(MEG_ERRO) {
                        Serial.println("Erro ao abrir SAVE");
                    }
                }else{
                    int loop = 0;
                    String valor = "";
                    while(loop < INDEX_B && arquivo.available()) {
                        valor = arquivo.readStringUntil('\n');
                        if(loop >= INDEX_A) {
                            texto += valor+"\n";
                        }
                        valor = "";
                        loop++;
                    }
                    arquivo.close();
                    setSaveLoadOk(0);
                    return texto;
                }
            }
            break;
        case 2: {
                if(!removeTexto(getNomeFS(),getNome())) {
                    setSaveLoadOk(-2);
                    return "";
                }

                File arquivo = LittleFS.open("/DISK_PLAD_CPRGD/SAVE", "r");
                File arquivo2 = LittleFS.open("/DISK_PLAD_CPRGD/"+getNomeFS()+".txt", "a");
                if (!arquivo || !arquivo2) {
                    setSaveLoadOk(-2);
                    if(MEG_ERRO) {
                        Serial.println("Erro ao abrir SAVE");
                    }
                }else{
                    
                    if (!arquivo2) {
                        setSaveLoadOk(-2);
                        if(MEG_ERRO) {
                            Serial.println("Erro ao criar arquivo definitivo");
                        }
                    }else{
                        int loop = 0;
                        bool filtroBool = false;
                        String valor = "";
                        while(arquivo.available()) {
                            texto = arquivo.readStringUntil('\n');

                            for(int i = 0; i < texto.length(); i++) {
                                char caractere = texto.charAt(i);

                                if(filtroBool) {
                                    filtroBool = false;
                                    if (caractere == 'n') {
                                        valor += "\n";
                                        continue;
                                    }
                                    else if (caractere == 'r') {
                                        valor += "\r";
                                        continue;
                                    }
                                }else if(caractere == '\\') {
                                    filtroBool = true;
                                }

                                if(!filtroBool) {
                                    valor += caractere;
                                }
                            }
                            filtroBool = false;
                            texto = valor;
                            valor = "";

                            cprgd.compressao(texto, filroCPRGD, filroCPRGD);
                            texto = plad.newString("texto")+plad.newString("code");
                            texto = plad.set("texto", cprgd.getTextoIndex(), texto, false);
                            texto = plad.set("code", cprgd.getCodeIndex(), texto, false);
                            arquivo2.print("{"+getNome()+"["+String(loop)+"]}:");
                            for(int i = 0; i < texto.length(); i++) {
                                char caractere = texto.charAt(i);

                                if (caractere == '\\') {
                                    arquivo2.print("\\\\");
                                }
                                else if (caractere == '\n') {
                                    arquivo2.print("\\n");
                                }
                                else if (caractere == '\r') {
                                    arquivo2.print("\\r");
                                }
                                else if (caractere == '{') {
                                    arquivo2.print("\\{");
                                }
                                else if (caractere == '}') {
                                    arquivo2.print("\\}");
                                }
                                else if (caractere == ':') {
                                    arquivo2.print("\\:");
                                }
                                else {
                                    arquivo2.write(caractere);
                                }
                            }
                            arquivo2.print("\n");
                            loop++;
                        }
                        arquivo.close();
                        arquivo2.close();
                        setSaveLoadOk(1);
                    }
                }
                break;
            }
    
        default:
            break;
        }
    }
    return "";
}

String DISK_PLAD_CPRGD::postLoad(int index) {
    int numero = onLoad();
    setSaveLoadOk(0);
    String texto = "";
    if (!begin()) {
        setSaveLoadOk(-1);
        if(MEG_ERRO) {
            Serial.println("Erro ao montar o LittleFS");
        }
    }else{
        switch (numero) {
        case 1: {
                File arquivo = LittleFS.open("/DISK_PLAD_CPRGD/"+getNomeFS()+".txt", "r");
                File arquivo2 = LittleFS.open("/DISK_PLAD_CPRGD/LOAD", "w");
                if (!arquivo || !arquivo2) {
                    setSaveLoadOk(-2);
                    if(MEG_ERRO) {
                        Serial.println("Erro ao abrir");
                    }
                }else{
                    String valor = "";
                    while(arquivo.available()) {
                        valor = arquivo.readStringUntil('\n');

                        int fim = valor.indexOf("}:");

                        if(fim == -1) {
                            continue;
                        }

                        int start = valor.indexOf("{");

                        if(start == -1 || start >= fim) {
                            continue;
                        }

                        int numeroList = valor.indexOf("[");

                        if(numeroList == -1 || numeroList >= fim) {
                            continue;
                        }

                        if(valor.substring(start + 1, numeroList) != getNome()) {
                            continue;
                        }

                        arquivo2.print(valor+"\n");

                        valor = "";
                    }
                    arquivo.close();
                    arquivo2.close();
                    setSaveLoadOk(0);
                    return "";
                }
            }
            break;
        case 2: {
                File arquivo = LittleFS.open("/DISK_PLAD_CPRGD/LOAD", "r");
                if (!arquivo) {
                    setSaveLoadOk(-2);
                    if(MEG_ERRO) {
                        Serial.println("Erro ao abrir LOAD");
                    }
                }else{
                    //int loop = 0;
                    bool filtroBool = false;
                    String valor = "";
                    while(arquivo.available()) {
                        texto = arquivo.readStringUntil('\n');

                        int fim = texto.indexOf("}:");

                        if(fim == -1) {
                            continue;
                        }

                        int start = texto.indexOf("{");

                        if(start == -1 || start >= fim) {
                            continue;
                        }

                        int listFim = texto.indexOf("]");

                        if(listFim == -1 || listFim >= fim) {
                            continue;
                        }

                        int listStart = texto.indexOf("[");

                        if(listStart == -1 || listStart >= listFim) {
                            continue;
                        }

                        if(index != texto.substring(listStart+1, listFim).toInt()) {
                            continue;
                        }

                        texto = texto.substring(fim+2);

                        for(int i = 0; i < texto.length(); i++) {
                            char caractere = texto.charAt(i);

                            if(filtroBool) {
                                filtroBool = false;
                                if (caractere == 'n') {
                                    valor += "\n";
                                    continue;
                                }
                                else if (caractere == 'r') {
                                    valor += "\r";
                                    continue;
                                }
                            }else if(caractere == '\\') {
                                filtroBool = true;
                            }

                            if(!filtroBool) {
                                valor += caractere;
                            }
                        }
                        filtroBool = false;
                        texto = valor;
                        valor = "";

                        texto = cprgd.deCompressao(plad.getString("texto", texto), plad.getString("code", texto));
                    }
                    arquivo.close();
                    setSaveLoadOk(1);
                    return texto;
                }
                break;
            }
    
        default:
            break;
        }
    }
    return "";
}

String DISK_PLAD_CPRGD::getNomeList(String nomeFS) {
    String texto = "";

    if (!begin()) {
        if(MEG_ERRO) {
            Serial.println("Erro ao montar LittleFS");
        }
        return texto;
    }

    File arquivo = LittleFS.open(
        "/DISK_PLAD_CPRGD/" + nomeFS + ".txt", "r"
    );

    if (!arquivo) {
        if(MEG_ERRO) {
            Serial.println("Erro ao abrir arquivo");
        }
        return texto;
    }

    while (arquivo.available()) {
        String valor = arquivo.readStringUntil('\n');

        int separador = valor.indexOf("}:");

        if (separador == -1) {
            continue;
        }

        int inicio = valor.indexOf('{');

        if (inicio == -1 || inicio >= separador) {
            continue;
        }

        texto += valor.substring(inicio + 1, separador) + ",";
    }

    arquivo.close();

    return texto;
}