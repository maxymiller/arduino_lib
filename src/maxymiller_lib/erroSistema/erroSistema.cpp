#include <maxymiller_lib/erroSistema.h>

void erroSistema::salvarErroApp(String mensagem) {
    prefs.begin("erro", false);

    prefs.putBool("temErro", true);
    prefs.putString("mensagem", mensagem);

    prefs.end();
}

String erroSistema::mostrarErroAnterior() {
  prefs.begin("erro", false);

  bool temErro = prefs.getBool("temErro", false);

  motivo = esp_reset_reason();

  errorOk = prefs.getBool("errorOk", false);

  if(erroTrue(motivo) && !errorOk) {
    prefs.putInt("errorMotivo", (int)motivo);
    prefs.putBool("errorOk", true);
    prefs.end();
    ESP.restart();
  }else if(errorOk){
    motivo = (esp_reset_reason_t)prefs.getInt("errorMotivo");
  }/*else if(temErro) {
    removeErroAnterior();
    prefs.end();
    reboot(0);
  }*/
  
  code = nomeReset(motivo);

  //if (temErro || erroTrue(motivo)) {
  if (errorOk) {
    mensagem = prefs.getString(
      "mensagem",
      "Erro desconhecido"
    );

    String megError = "";

    megError += "\n";
    megError += "================================\n";
    megError += "    ERRO DO BOOT ANTERIOR\n";
    megError += "================================\n";

    megError += "Reset reason: ";
    megError += String((int)motivo)+"\n";

    megError += "Error code: ";
    megError += code +"\n";

    megError += "Mensagem: ";
    megError += mensagem+"\n";

    megError += "================================\n";
    megError += "\n";

    // Apaga o marcador depois de mostrar
    //prefs.putBool("temErro", false);

    prefs.end();

    return megError;
  }else{
    prefs.end();
    return "";
  }
}

void erroSistema::removeErroAnterior() {
  prefs.begin("erro", false);
  prefs.putBool("temErro", false);
  prefs.putString("mensagem", "Erro desconhecido");
  prefs.end();

}

void erroSistema::reboot(int seg) {
    delay(seg*1000);
    //removeErroAnterior();
    prefs.begin("erro", false);
    prefs.putBool("errorOk", false);
    prefs.end();
    ESP.restart();
}

const char* erroSistema::nomeReset(esp_reset_reason_t motivo) {
    switch (motivo) {
        case ESP_RST_UNKNOWN:
            return "UNKNOWN";

        case ESP_RST_POWERON:
            return "POWERON";

        case ESP_RST_EXT:
            return "EXTERNAL";

        case ESP_RST_SW:
            return "SOFTWARE";

        case ESP_RST_PANIC:
            return "PANIC";

        case ESP_RST_INT_WDT:
            return "INT_WDT";

        case ESP_RST_TASK_WDT:
            return "TASK_WDT";

        case ESP_RST_WDT:
            return "WDT";

        case ESP_RST_DEEPSLEEP:
            return "DEEPSLEEP";

        case ESP_RST_BROWNOUT:
            return "BROWNOUT";

        case ESP_RST_SDIO:
            return "SDIO";

        default:
            return "UNKNOWN";
    }
}
boolean erroSistema::erroTrue(esp_reset_reason_t motivo) {
    switch (motivo) {
        case ESP_RST_PANIC:
        case ESP_RST_INT_WDT:
        case ESP_RST_TASK_WDT:
        case ESP_RST_WDT:
        case ESP_RST_BROWNOUT:
            return true;

        default:
            return false;
    }
}

int erroSistema::getMotivo() {
    return (int)motivo;
}
String erroSistema::getCode() {
    return code;
}
String erroSistema::getMensagem() {
    return mensagem;
}
