# arduino_lib

---

## Erro

Exemplo:

```cpp
#include <Arduino.h>

#include <maxymiller_lib/erroSistema.h>

erroSistema erro;

void setup() {
    Serial.begin(115200);

    String megError = erro.mostrarErroAnterior();
    if(megError != "") {
      Serial.println(megError);

      erro.reboot(5);
    }
    erro.removeErroAnterior();

    erro.salvarErroApp("Erro do Boot");

    // código

    erro.removeErroAnterior();
}

void loop() {
}
```
| Código | Constante           | Significado                          |
| -----: | ------------------- | ------------------------------------ |
|    `0` | `ESP_RST_UNKNOWN`   | Motivo desconhecido                  |
|    `1` | `ESP_RST_POWERON`   | Ligou/energizou o ESP32              |
|    `2` | `ESP_RST_EXT`       | Reset externo                        |
|    `3` | `ESP_RST_SW`        | Reset por software (`ESP.restart()`) |
|    `4` | `ESP_RST_PANIC`     | Exceção/Panic/Guru Meditation        |
|    `5` | `ESP_RST_INT_WDT`   | Interrupt Watchdog                   |
|    `6` | `ESP_RST_TASK_WDT`  | Task Watchdog                        |
|    `7` | `ESP_RST_WDT`       | Outro Watchdog                       |
|    `8` | `ESP_RST_DEEPSLEEP` | Retorno do Deep Sleep                |
|    `9` | `ESP_RST_BROWNOUT`  | Brownout: tensão/alimentação baixa   |
|   `10` | `ESP_RST_SDIO`      | Reset através de SDIO                |

```cpp
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
```
---

## PLAD
protocolo leve armazenamento de dado

```cpp
#include <maxymiller_lib/PLAD.h>
```

---

## ABCList
list de abc e int

```cpp
#include <maxymiller_lib/ABCList.h>
```

---

## CPRGD
compressão palavra repetitivo gigantes de dado

```cpp
#include <maxymiller_lib/CPRGD.h>
```
---

## DISK_PLAD_CPRGD
disk PLAD e CPRGD

e um `fileFS`

usar o "teste" como exemplo base

teste 1:
```cpp
#include <Arduino.h>

#include <LittleFS.h>

#include "maxymiller_lib/DISK_PLAD_CPRGD.h"
#include <maxymiller_lib/PLAD.h>

DISK_PLAD_CPRGD dpc;
PLAD plad;

void setup() {
  Serial.begin(115200);

  dpc.begin();
  dpc.boot();

  File arquivo = LittleFS.open("/DISK_PLAD_CPRGD/teste.txt", "r");
  while(arquivo.available()) {
    String texto = arquivo.readStringUntil('\n');
    for(int i = 0; i < texto.length(); i++) {
      if(texto.charAt(i) == '\r'){
        Serial.print(" {\\r} ");
      }else if(texto.charAt(i) == '\n') {
        Serial.print(" {\\n} ");
      }else{
        Serial.print(texto.charAt(i));
      }
      delay(10);
    }
    Serial.println();
  }
  arquivo.close();

  if(dpc.onSave() > 0) {
    int max = 2;
    dpc.setLinhaVer(0, max);
    String valor = dpc.postSave("[],=");

    if(dpc.onSaveLoadOk() < 0) {
      dpc.endSave(false);
    }

    if(dpc.onSave() == 1) {
      boolean ok = false;
      int loop = 0;
      int calc = 0;
      String texto = plad.newList("nome")+plad.newList("texto");
      while(valor != "") {

        texto = plad.set("nome["+String(loop)+"]", valor.substring(0, valor.indexOf("\n")), texto, false);
        valor = valor.substring(valor.indexOf("\n")+1);
        texto = plad.set("texto["+String(loop)+"]", valor.substring(0, valor.indexOf("\n")), texto, false);

        /*if(loop == 0) {
          ok = dpc.setSaveTexto(texto+"\n", true);
        }else{
          ok = dpc.setSaveTexto(texto+"\n", false);
        }*/

        loop++;
        calc = max*loop;

        Serial.println(valor);

        /*if(!ok) {
          dpc.endSave(false);
        }*/

        dpc.setLinhaVer(0+calc, max+calc);

        valor = dpc.postSave("");

      }
      if(loop == 0) {
        ok = dpc.setSaveTexto(texto+"\n", true);
      }else{
        ok = dpc.setSaveTexto(texto+"\n", false);
      }
      dpc.endSave(ok);
    }

    if(dpc.onSaveLoadOk() >= 0) {
      dpc.endSave(true);
    }else{
      dpc.endSave(false);
    }
  }

  if(dpc.onLoad() > 0) {
    int loop = 0;
    String valor = dpc.postLoad(loop);
    if(dpc.onLoad() == 2) {
      String texto = "";
      while(valor != "") {

        //

        Serial.println(valor);

        loop++;

        valor = dpc.postLoad(loop);

      }
    }
    Serial.println(valor);

    if(dpc.onSaveLoadOk() >= 0) {
      dpc.endLoad(true);
    }else{
      dpc.endLoad(false);
    }
  }

  Serial.println(dpc.getNomeList("teste"));
  int loop = 0;
  while(dpc.indexLinha("teste", "teste"+String(loop)+"[0]") >= 0) {
    Serial.println(dpc.indexLinha("teste", "teste"+String(loop)+"[0]"));
    loop++;
  }
  Serial.print("save ou load: ");
}

void loop() {
  if(Serial.available()) {
    String line = Serial.readStringUntil('\n');
    Serial.println(line);

    if(line == "save") {
      String line2 = "n";
      int loop = 0;
      int loop2 = 0;
      while(dpc.indexLinha("teste", "teste"+String(loop)+"[0]") >= 0) {
        loop++;
      }
      dpc.save("teste", "teste"+String(loop));
      while(line2 != "s") {
        Serial.print("nome: ");
        while(!Serial.available()) {
          delay(100);
        }
        line = Serial.readStringUntil('\n');
        
        //dpc.save("teste", "teste4");
        if(loop2 == 0) {
          dpc.setSaveTexto(line, true);
        }else{
          dpc.setSaveTexto(line, false);
        }
        loop2++;
        Serial.println(line);

        Serial.print("texto: ");
        while(!Serial.available()) {
          delay(100);
        }
        line = Serial.readStringUntil('\n');
        Serial.println(line);
        
        Serial.print("exit? (s/n): ");
        
        line2 = "";
        while(!(line2 == "s" || line2 == "n")){
          while(!Serial.available()) {
            delay(100);
          }
          line2 = Serial.readStringUntil('\n');
          Serial.println(line2);
        }
        if(line2 == "n") {
          dpc.setSaveTexto(line, false);
        }
      }
      dpc.endSave(dpc.setSaveTexto(line, false));

      /*dpc.save("teste", "teste0");
      for(int i = 0; i < 100; i++) {
        dpc.setSaveTexto(String(i)+",", false);
      }
      dpc.endSave(true);*/

    }else if(line == "load") {

      Serial.print("nome index: ");
      while(!Serial.available()) {
        delay(100);
      }
      line = Serial.readStringUntil('\n');
      Serial.println(line);

      dpc.load("teste", line);

      dpc.endLoad(true);
    }
  }
}

```

teste 2:
```cpp
#include <Arduino.h>

#include <LittleFS.h>

#include "maxymiller_lib/DISK_PLAD_CPRGD.h"
#include <maxymiller_lib/PLAD.h>

DISK_PLAD_CPRGD dpc;
PLAD plad;

void setup() {
  Serial.begin(115200);

  dpc.begin();
  dpc.boot();

  File arquivo = LittleFS.open("/DISK_PLAD_CPRGD/teste.txt", "r");
  if(arquivo) {
    while(arquivo.available()) {
      String texto = arquivo.readStringUntil('\n');
      for(int i = 0; i < texto.length(); i++) {
        if(texto.charAt(i) == '\r'){
          Serial.print(" {\\r} ");
        }else if(texto.charAt(i) == '\n') {
          Serial.print(" {\\n} ");
        }else{
          Serial.print(texto.charAt(i));
        }
        delay(1);
      }
      Serial.println();
    }
    arquivo.close();
  }

  if(dpc.onSave() > 0) {
    int max = 2;
    dpc.setLinhaVer(0, max);
    String valor = dpc.postSave("[],=");

    if(dpc.onSaveLoadOk() < 0) {
      dpc.endSave(false);
    }

    if(dpc.onSave() == 1) {
      boolean ok = false;
      int loop = 0;
      int calc = 0;
      String texto = plad.newList("nome")+plad.newList("texto");
      while(valor != "") {

        texto = plad.set("nome["+String(loop)+"]", valor.substring(0, valor.indexOf("\n")), texto, false);
        valor = valor.substring(valor.indexOf("\n")+1);
        texto = plad.set("texto["+String(loop)+"]", valor.substring(0, valor.indexOf("\n")), texto, false);

        if(loop == 0) {
          ok = dpc.setSaveTexto(texto+"\n", true);
        }else{
          ok = dpc.setSaveTexto(texto+"\n", false);
        }

        loop++;
        calc = max*loop;

        Serial.println(valor);

        if(!ok) {
          dpc.endSave(false);
        }

        dpc.setLinhaVer(0+calc, max+calc);

        valor = dpc.postSave("");

      }
      dpc.endSave(ok);
    }

    if(dpc.onSaveLoadOk() >= 0) {
      dpc.endSave(true);
    }else{
      dpc.endSave(false);
    }
  }

  if(dpc.onLoad() > 0) {
    int loop = 0;
    String valor = dpc.postLoad(loop);
    if(dpc.onLoad() == 2) {
      String texto = "";
      while(valor != "") {

        //

        Serial.println(valor);

        loop++;

        valor = dpc.postLoad(loop);

      }
    }
    Serial.println(valor);

    if(dpc.onSaveLoadOk() >= 0) {
      dpc.endLoad(true);
    }else{
      dpc.endLoad(false);
    }
  }

  Serial.println(dpc.getNomeList("teste"));
  int loop = 0;
  while(dpc.indexLinha("teste", "teste"+String(loop)+"[0]") >= 0) {
    Serial.println(dpc.indexLinha("teste", "teste"+String(loop)+"[0]"));
    loop++;
  }
  Serial.print("save ou load: ");
}

void loop() {
  if(Serial.available()) {
    String line = Serial.readStringUntil('\n');
    Serial.println(line);

    if(line == "save") {
      String line2 = "n";
      int loop = 0;
      int loop2 = 0;
      while(dpc.indexLinha("teste", "teste"+String(loop)+"[0]") >= 0) {
        loop++;
      }
      dpc.save("teste", "teste"+String(loop));
      for(int i = 0; i < 100; i++) {
        if(i == 0) {
          dpc.setSaveTexto(String(i), true);
        }else{
          dpc.setSaveTexto(String(i), false);
        }
        dpc.setSaveTexto(String(i), false);
      }
      dpc.endSave(true);

    }else if(line == "load") {

      Serial.print("nome index: ");
      while(!Serial.available()) {
        delay(100);
      }
      line = Serial.readStringUntil('\n');
      Serial.println(line);

      dpc.load("teste", line);

      dpc.endLoad(true);
    }
  }
}
```
