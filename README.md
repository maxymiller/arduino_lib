# arduino_lib

---

## Erro

Exemplo:

```
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


