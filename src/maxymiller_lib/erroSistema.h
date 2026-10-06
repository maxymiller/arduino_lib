#ifndef ERROSISTEMA_H
#define ERROSISTEMA_H

//#include <Arduino.h>
#include <Preferences.h>
#include "esp_system.h"

class erroSistema {
private:
    Preferences prefs;
    const char* nomeReset(esp_reset_reason_t motivo);
    boolean erroTrue(esp_reset_reason_t motivo);

    esp_reset_reason_t motivo;
    String code;
    String mensagem;

    bool errorOk;
public:
    void salvarErroApp(String mensagem);
    String mostrarErroAnterior();
    void removeErroAnterior();
    void reboot(int seg);

    int getMotivo();
    String getCode();
    String getMensagem();
};

#endif
