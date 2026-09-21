# Componentes

Lista de hardware utilizado no protótipo, simulado no Wokwi (`diagram.json`).

| Componente | Modelo/tipo | Função | Pino(s) no ESP32 |
|---|---|---|---|
| Microcontrolador | ESP32 DevKit C V4 | Leitura dos sensores, exibição no LCD e envio dos dados via WiFi | — |
| Sensor de temperatura e umidade | DHT22 | Mede temperatura e umidade do ambiente | GPIO 4 (SDA) |
| Resistor de pull-up | 1kΩ | Liga a linha DATA do DHT22 ao VCC | — |
| Sensor de luminosidade | LDR (fotorresistor) | Mede o nível de luz do ambiente | GPIO 34 (AO) |
| Sensor de gás | MQ2 | Mede qualidade do ar (substitui o MQ135, indisponível no Wokwi; interface elétrica idêntica) | GPIO 35 (AOUT) |
| Potenciômetro | Potenciômetro linear | Simula o nível de ruído (não há sensor de microfone disponível no Wokwi) | GPIO 32 (SIG) |
| Display | LCD 16x2 com interface I2C (endereço 0x27) | Exibe os dados em tempo real | GPIO 21 (SDA) / GPIO 22 (SCL) |

## Observações

* O resistor de pull-up do DHT22 deve ligar a linha DATA ao VCC. Ligar as duas pernas do resistor na linha DATA causa erros de leitura.
* O display LCD usa comunicação I2C, reduzindo o número de pinos necessários.
* Todos os componentes estão descritos com suas posições e conexões completas em `diagram.json`, o arquivo de simulação do Wokwi.
