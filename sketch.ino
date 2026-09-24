// Bibliotecas necessarias
#include <Arduino.h>
// Bibliotecas necessarias
#include <DHT.h>              // Biblioteca do sensor de temperatura/umidade DHT22
#include <Wire.h>             // Biblioteca de comunicação I2C (usada pelo LCD)
#include <LiquidCrystal_I2C.h> // Biblioteca do display LCD via I2C
#include <WiFi.h>             // Conexão Wi-Fi do ESP32
#include <WiFiClientSecure.h> // Cliente Wi-Fi com suporte a HTTPS
#include <HTTPClient.h>       // Envio de requisições HTTP (POST pro servidor)

// Acesso ao Wi-Fi do Wokwi
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* serverURL = "https://microcontrolador.papaixinho.com.br/api/dados.php"; // Endpoint que recebe os dados via POST

// Definição das portas do ESP
#define DHTTYPE DHT22   // Modelo do sensor DHT usado (DHT22)
#define DHTPIN 4        // Pino de dados do DHT22 (temperatura/umidade)
#define LDRPIN 34       // Pino analógico do sensor de luminosidade (LDR)
#define MQ2PIN 35       // Pino analógico do sensor de qualidade do ar (MQ2)
#define SOMPIN 32       // Pino analógico do potenciômetro (simula sensor de som)
#define BUZZERPIN 25    // Pino digital do buzzer passivo (alarme sonoro)
#define frequencia 2000 // Frequência (Hz) do som emitido pelo buzzer
#define LEDGPIN 12      // Pino digitais do LED Verde
#define LEDYPIN 13      // Pino digitais do LED Amarelo
#define LEDRPIN 14      // Pino digitais do LED Vermelho

// Objetos globais dos componentes
DHT dht(DHTPIN, DHTTYPE);            // Sensor DHT22
LiquidCrystal_I2C lcd(0x27, 20, 4);  // LCD 20x4 no endereço I2C 0x27

int contadorExtremo = 0; // contator de leituras extremas seguidas

// Inicialização do prototipo
void setup() {
  Serial.begin(115200); // Inicia comunicação serial pra debug
  lcd.init();
  lcd.backlight();
  Serial.println("ESP32 iniciado - projeto de monitoramento ambiental");

  pinMode(BUZZERPIN, OUTPUT); // Configura o buzzer como saída digital

  dht.begin(); // Inicializa o sensor DHT22

  WiFi.begin(ssid, password); // Inicia a conexão Wi-Fi
  Serial.print("Conectando ao WiFi");

  // Inicializa os LEDs
  pinMode(LEDGPIN, OUTPUT);
  pinMode(LEDYPIN, OUTPUT);
  pinMode(LEDRPIN, OUTPUT);

  // Aguarda a conexão Wi-Fi ser estabelecida antes de seguir
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  // --- Leitura dos sensores ---
  float umidade = dht.readHumidity();
  float temperatura = dht.readTemperature();
  String tempTexto;
  String umiTexto;

  // Se a leitura do DHT22 falhar, aborta esse ciclo do loop e tenta de novo depois
  if (isnan(umidade) || isnan(temperatura)) {
    Serial.println("Erro ao ler o sensor DHT22!");
    return;
  }

  // --- Classificação da temperatura em faixas de estado ---
  if (temperatura <= 0) {
    tempTexto = "Temperatura Negativa";
  } else if (temperatura <= 15) {
    tempTexto = "Temperatura frio";
  } else if (temperatura <= 26) {
    tempTexto = "Temperatura amena";
  } else if (temperatura <= 32) {
    tempTexto = "Temperatura Alta";
  } else if (temperatura <= 45) {
    tempTexto = "Temperatura Muito Alta";
  } else {
    tempTexto = "Temperatura Extremo";
  }

  // --- Classificação da umidade em faixas de estado ---
  if (umidade <= 12) {
    umiTexto = "Emergência";
  } else if (umidade > 12 && umidade <= 20) {
    umiTexto = "Alerta";
  } else if (umidade > 20 && umidade <= 30) {
    umiTexto = "Atenção";
  } else if (umidade > 30 && umidade < 40) {
    umiTexto = "Atenção II";
  } else if (umidade >= 40 && umidade <= 70) {
    umiTexto = "Ideal";
  } else {
    umiTexto = "Umidade alta";
  }

  // --- Leitura e classificação da luminosidade (LDR) ---
  int luminosidade = analogRead(LDRPIN);
  String luzTexto;

  if (luminosidade <= 1365) {
    luzTexto = "Muito claro";
  } else if (luminosidade <= 2730) {
    luzTexto = "Adequado";
  } else {
    luzTexto = "Escuro";
  }

  // --- Leitura e classificação da qualidade do ar (MQ2) ---
  int qualidadeAr = analogRead(MQ2PIN);
  String qualidadeArTexto;

  if (qualidadeAr <= 3132) {
    qualidadeArTexto = "Bom";
  } else if (qualidadeAr > 3132 && qualidadeAr <= 3665) {
    qualidadeArTexto = "Ruim";
  } else {
    qualidadeArTexto = "Extremo";
  }

  // --- Leitura e classificação do nível de som (potenciômetro simulando microfone) ---
  int nivelSom = analogRead(SOMPIN);
  String nivelSomTexto;

  if (nivelSom <= 1500) {
    nivelSomTexto = "Baixo";
  } else if (nivelSom > 1500 && nivelSom <= 3000) {
    nivelSomTexto = "Moderado";
  } else {
    nivelSomTexto = "Alto";
  }

  // Definindo o nivel do LED
  int nivelTemp = 0, nivelUmi = 0, nivelLumi = 0, nivelAr = 0, nivelS = 0;

  if (tempTexto == "Temperatura Extremo" || tempTexto == "Temperatura Negativa") {
    nivelTemp = 2;
  } else if (tempTexto == "Temperatura Alta" || tempTexto == "Temperatura Muito Alta") {
    nivelTemp = 1;
  } else {
    nivelTemp = 0;
  }

  if (umiTexto  == "Emergência" || umiTexto == "Umidade alta") {
    nivelUmi = 2;
  } else if (umiTexto == "Alerta" || umiTexto == "Atenção" || umiTexto == "Atenção II") {
    nivelUmi = 1;
  } else {
    nivelUmi = 0;
  }

  if (luzTexto   == "Muito claro" || luzTexto  == "Escuro") {
    nivelLumi = 2;
  } else {
    nivelLumi = 0;
  }

  if (qualidadeArTexto   == "Extremo") {
    nivelAr = 2;
  } else if (qualidadeArTexto  == "Ruim") {
    nivelAr = 1;
  } else {
    nivelAr = 0;
  }

  if (nivelSomTexto    == "Alto") {
    nivelS = 2;
  } else if (nivelSomTexto   == "Moderado") {
    nivelS = 1;
  } else {
    nivelS = 0;
  }

  int maiorNivel = max(nivelTemp, max(nivelUmi, max(nivelLumi, max(nivelAr, nivelS))));

  // Acendendo os LEDs
  if (maiorNivel == 2){
    digitalWrite(LEDRPIN, HIGH);
    digitalWrite(LEDYPIN, LOW);
    digitalWrite(LEDGPIN, LOW);
  } else if (maiorNivel == 1){
    digitalWrite(LEDRPIN, LOW);
    digitalWrite(LEDYPIN, HIGH);
    digitalWrite(LEDGPIN, LOW);
  } else {
    digitalWrite(LEDRPIN, LOW);
    digitalWrite(LEDYPIN, LOW);
    digitalWrite(LEDGPIN, HIGH);
  }

  // --- Verifica se algum sensor está em estado crítico (lógica OU) ---
  bool alertaAtivo = false;

  alertaAtivo = (tempTexto == "Temperatura Extremo") || (tempTexto == "Temperatura Negativa") ||
                (umiTexto == "Emergência") || (umiTexto == "Umidade alta") ||
                (luzTexto == "Muito claro") || (luzTexto == "Escuro") ||
                (qualidadeArTexto == "Extremo") || (nivelSomTexto == "Alto");

  // Contabilizador de quantas vezes algums dados ficou como "Extremo"
  if (alertaAtivo) {
    contadorExtremo++;
  } else {
    contadorExtremo = 0;
  }

  // Soa o alarme: só liga após 3 leituras extremas seguidas (evita disparo por 1 leitura isolada/ruído);
  // desliga automaticamente assim que uma leitura normaliza (contador zera)
  if (contadorExtremo >= 3) {
    tone(BUZZERPIN, frequencia);
  } else {
    noTone(BUZZERPIN);
  }

  // --- Debug via serial ---
  Serial.print("Umidade: ");
  Serial.print(umidade);
  Serial.print("%  Temperatura: ");
  Serial.print(temperatura);
  Serial.println("°C");
  Serial.print("Luminosidade: ");
  Serial.println(luzTexto);
  Serial.print("Qualidade do ar: ");
  Serial.println(qualidadeArTexto);
  Serial.print("Nivel do som: ");
  Serial.println(nivelSom);

  // --- Exibição no LCD ---
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperatura);
  lcd.print("C U:");
  lcd.print(umidade);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Ar:");
  lcd.print(qualidadeArTexto);

  lcd.setCursor(0, 2);
  lcd.print("Luminosidade:");
  lcd.print(luzTexto);

  lcd.setCursor(0, 3);
  lcd.print("Som:");
  lcd.print(nivelSomTexto);

  // --- Montagem do JSON com os dados brutos dos sensores ---
  String jsonData = "{";
  jsonData += "\"temperatura\":" + String(temperatura) + ",";
  jsonData += "\"umidade\":" + String(umidade) + ",";
  jsonData += "\"luminosidade\":" + String(luminosidade) + ",";
  jsonData += "\"qualidade_ar\":" + String(qualidadeAr) + ",";
  jsonData += "\"nivel_som\":" + String(nivelSom);
  jsonData += "}";

  Serial.print("JSON montado: ");
  Serial.println(jsonData);

  // --- Envio dos dados para o servidor via HTTP POST ---
  HTTPClient http;
  http.begin(serverURL);
  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST(jsonData);

  if (httpResponseCode > 0) {
    Serial.print("Resposta do servidor: ");
    Serial.println(httpResponseCode);
    String resposta = http.getString();
    Serial.println(resposta);
  } else {
    Serial.print("Erro ao enviar: ");
    Serial.println(httpResponseCode);
  }

  http.end();

  delay(5000); // Intervalo entre ciclos de leitura/envio
}
