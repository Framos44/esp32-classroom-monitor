# ESP32 Classroom Monitor

Sistema de monitoramento ambiental para salas de aula, construído com ESP32. Acompanha temperatura, umidade, luminosidade, qualidade do ar e nível de ruído, com foco em manter um ambiente mais confortável — inclusive para alunos com sensibilidades sensoriais.

Protótipo desenvolvido e testado no [Wokwi](https://wokwi.com/).

## Componentes utilizados

| Qtd | Componente | Observação |
|-----|------------|------------|
| 1 | ESP32 | Microcontrolador |
| 1 | Half Breadboard | Protoboard |
| 1 | Buzzer | Alerta sonoro |
| 1 | DHT22 | Sensor de temperatura e umidade |
| 1 | MQ2 | Sensor de qualidade do ar (substitui o MQ135, indisponível no Wokwi) |
| 1 | LDR | Sensor de luminosidade |
| 1 | Potenciômetro | Simula nível de som (substitui o microfone KY-038, indisponível no Wokwi) |
| 5 | NeoPixel LED Ring | Alerta visual (RGB endereçável) |
| 1 | LCD 20x4 I2C | Display de dados |
| 1 | Resistor | Pull-up para o pino de dados do DHT22 |

Nota: o resistor de pull-up do DHT22 deve ligar a linha DATA ao VCC. Ligar as duas pernas do resistor na linha DATA causa erros de leitura.

## Arquitetura

Sensores → ESP32 → Display LCD local
ESP32 → HTTP POST (WiFi) → Backend PHP + MySQL (hospedado via Plesk) → Dashboard web

## Como rodar
A simulação é executada localmente via VS Code, usando os plugins PlatformIO e Wokwi Simulator. Isso evita o alto load no servidor do Wokwi e permite testar sem interrupções.

1. Instale o VS Code com os plugins PlatformIO e Wokwi Simulator.
2. Abra o projeto na pasta local.
3. Dependências (listadas em `libraries.txt`): `LiquidCrystal I2C`, `WiFi.h` (nativa do ESP32), biblioteca DHT.
4. Inicie a simulação pelo Wokwi Simulator dentro do VS Code.
5. Acompanhe a conexão WiFi e os dados enviados pelo Serial Monitor.

A migração para hardware físico ainda está planejada como próxima etapa.

## Status atual

Concluído:

* Integração dos sensores
* Exibição no display LCD
* Conexão WiFi
* Envio de dados via HTTP POST para o backend
* Dashboard web (PHP + MySQL)
* Alerta sonoro e visual

Em andamento:

* Migração para hardware real

## Stack

Firmware em Arduino/C++ (ESP32), simulação no Wokwi, backend em PHP + MySQL, hospedagem via Plesk.

## Autor

Felipe Ramos — [github.com/Framos44](https://github.com/Framos44)
