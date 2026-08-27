# ESP32 Classroom Monitor

Sistema de monitoramento ambiental para salas de aula, construído com ESP32. Acompanha temperatura, umidade, luminosidade, qualidade do ar e nível de ruído, com foco em manter um ambiente mais confortável — inclusive para alunos com sensibilidades sensoriais.

Protótipo desenvolvido e testado no [Wokwi](https://wokwi.com/).

## Sensores e hardware

* DHT22 — temperatura e umidade
* LDR — luminosidade
* MQ2 — qualidade do ar (substitui o MQ135, indisponível no Wokwi; interface elétrica idêntica)
* Potenciômetro — nível de ruído (não há sensor de microfone disponível no Wokwi, então o potenciômetro simula essa entrada)
* Display LCD 16x2 (I2C, endereço 0x27) — exibição dos dados em tempo real

Nota: o resistor de pull-up do DHT22 deve ligar a linha DATA ao VCC. Ligar as duas pernas do resistor na linha DATA causa erros de leitura.

## Arquitetura

Sensores → ESP32 → Display LCD local
ESP32 → HTTP POST (WiFi) → Backend PHP + MySQL (hospedado via Plesk) → Dashboard web (em desenvolvimento)

## Classificação de qualidade do ar

Limites calibrados empiricamente com o simulador MQ2 do Wokwi:

* Abaixo de 3132: bom
* De 3132 a 3665: moderado
* Acima de 3665: ruim

## Como rodar

1. Abra o projeto no Wokwi.
2. Dependências (listadas em `libraries.txt`): `LiquidCrystal I2C`, `WiFi.h` (nativa do ESP32), biblioteca DHT.
3. Inicie a simulação.
4. Acompanhe a conexão WiFi e os dados enviados pelo Serial Monitor.

A migração para hardware físico ainda está planejada como próxima etapa.

## Status atual

Concluído:

* Integração dos sensores
* Exibição no display LCD
* Conexão WiFi

Em andamento:

* Envio de dados via HTTP POST para o backend
* Dashboard web (PHP + MySQL)
* Migração para hardware real

## Stack

Firmware em Arduino/C++ (ESP32), simulação no Wokwi, backend em PHP + MySQL, hospedagem via Plesk.

## Autor

Felipe Ramos — [github.com/Framos44](https://github.com/Framos44)
