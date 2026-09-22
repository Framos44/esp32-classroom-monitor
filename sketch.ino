// Bibliotecas necessarias
#include <DHT.h>              // Biblioteca do sensor de temperatura/umidade DHT22
#include <Wire.h>             // Biblioteca de comunicação I2C (usada pelo LCD)
#include <LiquidCrystal_I2C.h> // Biblioteca do display LCD via I2C
#include <WiFi.h>             // Conexão Wi-Fi do ESP32
#include <WiFiClientSecure.h> // Cliente Wi-Fi com suporte a HTTPS
#include <HTTPClient.h>       // Envio de requisições HTTP (POST pro servidor)
#include <Adafruit_NeoPixel.h> // Biblioteca dos anéis de LED endereçável (NeoPixel)

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

// Objetos globais dos componentes
DHT dht(DHTPIN, DHTTYPE);            // Sensor DHT22
LiquidCrystal_I2C lcd(0x27, 20, 4);  // LCD 20x4 no endereço I2C 0x27
// Cada anel de NeoPixel tem 16 LEDs; um anel por sensor, cada um em seu próprio pino
Adafruit_NeoPixel anelTemperatura(16, 33, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel anelUmidade(16, 27, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel anelLuminosidade(16, 14, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel anelQualidadedoar(16, 13, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel anelSom(16, 12, NEO_GRB + NEO_KHZ800);

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

  // Inicializa cada anel de NeoPixel (necessário antes de usar setPixelColor/show)
  anelTemperatura.begin();
  anelUmidade.begin();
  anelLuminosidade.begin();
  anelQualidadedoar.begin();
  anelSom.begin();

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
  } else if (temperatura <= 22) {
    tempTexto = "Temperatura amena";
  } else if (temperatura <= 30) {
    tempTexto = "Temperatura Alta";
  } else if (temperatura <= 45) {
    tempTexto = "Temperatura Muito Alta";
  } else {
    tempTexto = "Temperatura Extremo";
  }

  // --- Classificação da umidade em faixas de estado ---
  // Faixas baseadas em referência de conforto/saúde humana (ideal: 40-70%)
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

  // --- Pintando os anéis de LED conforme o estado de cada sensor ---
  int corR, corG, corB; // Variáveis reutilizadas pra montar a cor (R, G, B) de cada anel

  // Anel da temperatura
  if (tempTexto == "Temperatura Extremo" || tempTexto == "Temperatura Negativa") {
    corR = 255; corG = 0; corB = 0;     // Vermelho: extremos críticos
  } else if (tempTexto == "Temperatura frio") {
    corR = 0; corG = 255; corB = 100;   // Verde-azulado: frio, mas não crítico
  } else if (tempTexto == "Temperatura amena") {
    corR = 0; corG = 255; corB = 0;     // Verde puro: temperatura ideal
  } else if (tempTexto == "Temperatura Alta") {
    corR = 200; corG = 0; corB = 10;    // Vermelho intermediário: alta, mas não extrema
  } else {
    corR = 200; corG = 0; corB = 0;     // Cobre "Muito Alta"
  }

  // Aplica a cor decidida em todos os 16 LEDs do anel e publica no hardware
  for (int i = 0; i < 16; i++) {
    anelTemperatura.setPixelColor(i, corR, corG, corB);
  }
  anelTemperatura.show();

  // Anel da umidade
  if (umiTexto == "Emergência") {
    corR = 255; corG = 0; corB = 0;     // Vermelho: emergência
  } else if (umiTexto == "Alerta") {
    corR = 200; corG = 200; corB = 0;   // Amarelo: alerta
  } else if (umiTexto == "Atenção") {
    corR = 255; corG = 180; corB = 0;   // Amarelo-alaranjado: atenção
  } else if (umiTexto == "Atenção II") {
    corR = 255; corG = 100; corB = 10;  // Laranja: faixa intermediária (30-40%)
  } else if (umiTexto == "Ideal") {
    corR = 0; corG = 255; corB = 0;     // Verde: faixa ideal
  } else {
    corR = 200; corG = 0; corB = 0;     // Cobre "Umidade alta"
  }

  for (int i = 0; i < 16; i++) {
    anelUmidade.setPixelColor(i, corR, corG, corB);
  }
  anelUmidade.show();

  // Anel da luminosidade
  if (luzTexto == "Muito claro" || luzTexto == "Escuro") {
    corR = 255; corG = 0; corB = 0;     // Vermelho: extremos de luminosidade
  } else {
    corR = 0; corG = 255; corB = 0;     // Verde: luminosidade adequada
  }

  for (int i = 0; i < 16; i++) {
    anelLuminosidade.setPixelColor(i, corR, corG, corB);
  }
  anelLuminosidade.show();

  // Anel da qualidade do ar
  if (qualidadeArTexto == "Extremo") {
    corR = 255; corG = 0; corB = 0;     // Vermelho: ar extremo
  } else if (qualidadeArTexto == "Bom") {
    corR = 0; corG = 255; corB = 0;     // Verde: ar bom
  } else {
    corR = 200; corG = 50; corB = 0;    // Laranja: cobre "Ruim"
  }

  for (int i = 0; i < 16; i++) {
    anelQualidadedoar.setPixelColor(i, corR, corG, corB);
  }
  anelQualidadedoar.show();

  // Anel do som
  if (nivelSomTexto == "Alto") {
    corR = 255; corG = 0; corB = 0;     // Vermelho: som alto
  } else if (nivelSomTexto == "Moderado") {
    corR = 150; corG = 255; corB = 0;   // Verde-amarelado: som moderado
  } else {
    corR = 0; corG = 255; corB = 0;     // Verde: cobre "Baixo" (melhor caso)
  }

  for (int i = 0; i < 16; i++) {
    anelSom.setPixelColor(i, corR, corG, corB);
  }
  anelSom.show();

  // --- Verifica se algum sensor está em estado crítico (lógica OU) ---
  bool alertaAtivo = false;

  alertaAtivo = (tempTexto == "Temperatura Extremo") || (tempTexto == "Temperatura Negativa") ||
                (umiTexto == "Emergência") || (umiTexto == "Umidade alta") ||
                (luzTexto == "Muito claro") || (luzTexto == "Escuro") ||
                (qualidadeArTexto == "Extremo") || (nivelSomTexto == "Alto");

  // Soa o alarme: reativo — liga enquanto houver condição crítica, desliga quando normaliza
  if (alertaAtivo) {
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
  Serial.println(luminosidade);
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
