#ifndef DISK_PLAD_CPRGD_H
#define DISK_PLAD_CPRGD_H

#include <Arduino.h>
#include <Preferences.h>
#include <LittleFS.h>

#include <maxymiller_lib/PLAD.h>
#include <maxymiller_lib/CPRGD.h>

class DISK_PLAD_CPRGD {
private:
    Preferences memo;
    PLAD plad;
    CPRGD cprgd;
    String filtros(String texto);
    String filtro = "{}:,\n\r";
    String getNome();
    String getNomeFS();
    bool onBegin = false;
    void setSave(int numero);
    void setLoad(int numero);
    void setSaveLoadOk(int numero);
    void setNomeFS(String nomeFS, String nome);
    void setRemoveNomeFS(String nomeFS, String nome);
    String getRemoveNomeFS();
    String getRemoveNome();
    void setRemoveBoot(bool boot);
    bool getRemoveBoot();
    bool criarArquivo(String nomeFS, bool tipo = true);
    int INDEX_A = 0;
    int INDEX_B = 0;
    bool MEG_ERRO = true;
public:
    boolean boot();
    bool begin();
    int onSave();
    int onLoad();
    int onSaveLoadOk();

    // bool MEG_ERRO = true;
    // false = meg de erro desligado
    // true = meg de erro ligado
    void setMegErro(bool modo);
    void save(String nomeFS, String nome);
    void load(String nomeFS, String nome);
    String postSave(String filroCPRGD);
    boolean setSaveTexto(String texto, boolean limpar);
    boolean removeTexto(String nomeFS, String nome);
    void endSave(boolean fim);
    void endLoad(boolean fim);
    String postLoad(int index);
    String getNomeList(String nomeFS);
    int indexLinha(String nomeFS, String nome);
    int getErro();
    void setErro(int error);

    // min = 0, max = inf.
    void setLinhaVer(int indexA, int indexB);
};

#endif