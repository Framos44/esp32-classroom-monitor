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
| 1 | LED verde | Alerta visual — nível normal |
| 1 | LED amarelo | Alerta visual — nível de atenção |
| 1 | LED vermelho | Alerta visual — nível crítico |
| 1 | LCD 20x4 I2C | Display de dados |
| 3 | Resistor | Limitador de corrente para os LEDs (verde, amarelo, vermelho) |

## Observações

* O resistor de pull-up do DHT22 deve ligar a linha DATA ao VCC. Ligar as duas pernas do resistor na linha DATA causa erros de leitura.
* Apenas um dos três LEDs (verde, amarelo ou vermelho) fica aceso por vez, conforme o maior nível de alerta entre os sensores monitorados.
* O display LCD usa comunicação I2C, reduzindo o número de pinos necessários.
* Todos os componentes estão descritos com suas posições e conexões completas em `diagram.json`, o arquivo de simulação do Wokwi.
