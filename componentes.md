# Componentes

Lista de hardware utilizado no protótipo, simulado no Wokwi (`diagram.json`).

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

## Observações

* O resistor de pull-up do DHT22 deve ligar a linha DATA ao VCC. Ligar as duas pernas do resistor na linha DATA causa erros de leitura.
* O display LCD usa comunicação I2C, reduzindo o número de pinos necessários.
* Todos os componentes estão descritos com suas posições e conexões completas em `diagram.json`, o arquivo de simulação do Wokwi.
