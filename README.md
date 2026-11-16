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
| 1 | LED verde | Alerta visual — nível normal |
| 1 | LED amarelo | Alerta visual — nível de atenção |
| 1 | LED vermelho | Alerta visual — nível crítico |
| 1 | LCD 20x4 I2C | Display de dados |
| 1 | Botão (push button) | Liga/desliga o sistema |
| 1 | Botão (push button) | Silencia o buzzer (mute) |
| 3 | Resistor | Limitador de corrente para os LEDs (verde, amarelo, vermelho) |

Notas:

* O DHT22 não usa resistor de pull-up na linha de dados.
* Os botões usam o pull-up interno do ESP32 (`INPUT_PULLUP`): pino em HIGH quando soltos, LOW quando apertados — não precisam de resistor externo.
* Diagrama de montagem completo: `diagram.json` (simulação Wokwi) e `Diagrama.png`.

## Arquitetura

Sensores → ESP32 → Display LCD local + LEDs + buzzer
ESP32 → HTTP POST (WiFi) → Backend PHP + MySQL (hospedado via Plesk) → Dashboard web (`site/dashboard`)

O site (`site/`) também inclui uma página institucional (`site/index.html`) e uma página de visualização do modelo 3D (`site/3d`).

## Como rodar
A simulação é executada localmente via VS Code, usando os plugins PlatformIO e Wokwi Simulator. Isso evita o alto load no servidor do Wokwi e permite testar sem interrupções.

1. Instale o VS Code com os plugins PlatformIO e Wokwi Simulator.
2. Abra o projeto na pasta local.
3. Dependências (listadas em `libraries.txt`): `LiquidCrystal I2C`, `WiFi.h` (nativa do ESP32), biblioteca DHT.
4. Inicie a simulação pelo Wokwi Simulator dentro do VS Code.
5. Acompanhe a conexão WiFi e os dados enviados pelo Serial Monitor.

A migração para hardware físico ainda está planejada como próxima etapa.

## Estrutura do repositório

* `sketch.ino`, `diagram.json`, `libraries.txt`, `Diagrama.png` — firmware e simulação Wokwi
* `componentes.md` — lista detalhada de hardware
* `modelo3D/` — arquivos do modelo 3D da caixa do protótipo (`.stl`) e imagens de referência
* `site/index.html` — página institucional do projeto (SIMAEP)
* `site/dashboard/` — dashboard web com as leituras em tempo real (PHP + MySQL)
* `site/3d/` — página de visualização interativa do modelo 3D
* `site/api/dados.php` — endpoint que recebe os dados enviados pelo ESP32
* `site/banco/schema.sql` — schema da tabela `leituras`
* `site/imagens/` — logos e imagens usadas pelo site

## Status atual

Concluído:

* Integração dos sensores
* Exibição no display LCD
* Conexão WiFi
* Envio de dados via HTTP POST para o backend
* Dashboard web (PHP + MySQL)
* Alerta sonoro e visual
* Botões de liga/desliga e mute
* Página institucional e visualização do modelo 3D
* Modelagem 3D da caixa do protótipo

Em andamento:

* Migração para hardware real

## Stack

Firmware em Arduino/C++ (ESP32), simulação no Wokwi, backend em PHP + MySQL, hospedagem via Plesk, site institucional em HTML/CSS/JS.

## Autor

Felipe Ramos — [github.com/Framos44](https://github.com/Framos44)
