# arduino_lib

---

## Erro

```
#include <Arduino.h>

#include <maxymiller_lib/erroSistema.h>

erroSistema erro;

void setup() {
    Serial.begin(115200);

    String megError = erro.mostrarErroAnterior();
    if(megError != "") {
      Serial.println(megError);
      //tela.ligar();

      tela.escrever("", 0);

      tela.escrever(String(erro.getMotivo()), 2);
      tela.escrever(String(erro.getCode()), 3);

      erro.removeErroAnterior();
      erro.reboot(5);
    }

    erro.salvarErroApp("Erro do Boot");

    // código

    erro.removeErroAnterior();
}

void loop() {
}
```
