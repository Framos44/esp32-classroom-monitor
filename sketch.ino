#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* serverURL = "https://microcontrolador.papaixinho.com.br/api/dados.php";

#define DHTPIN 4
#define LDRPIN 34
#define MQ2PIN 35
#define SOMPIN 32
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  Serial.println("ESP32 iniciado - projeto de monitoramento ambiental");
  dht.begin();
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado!");
  Serial.print("Endereco IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  float umidade = dht.readHumidity();
  float temperatura = dht.readTemperature();

  if (isnan(umidade) || isnan(temperatura)) {
    Serial.println("Erro ao ler o sensor DHT22!");
    return;
  }

  int luminosidade = analogRead(LDRPIN);

  int qualidadeAr = analogRead(MQ2PIN);
  String qualidadeArTexto;

  if (qualidadeAr <= 3132) {
    qualidadeArTexto = "Bom";
  } else if (qualidadeAr > 3132 && qualidadeAr <= 3665) {
    qualidadeArTexto = "Ruim";
  } else {
    qualidadeArTexto = "Extremo";
  }

  int nivelSom = analogRead(SOMPIN);
  String nivelSomTexto;

  if (nivelSom <= 1500) {
    nivelSomTexto = "Baixo";
  } else if (nivelSom > 1500 && nivelSom <= 3000) {
    nivelSomTexto = "Moderado";
  } else {
    nivelSomTexto = "Alto";
  }

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
  Serial.println(nivelSomTexto);

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

  String jsonData = "{";
  jsonData += "\"temperatura\":" + String(temperatura) + ",";
  jsonData += "\"umidade\":" + String(umidade) + ",";
  jsonData += "\"luminosidade\":" + String(luminosidade) + ",";
  jsonData += "\"qualidade_ar\":" + String(qualidadeAr) + ",";
  jsonData += "\"nivel_som\":" + String(nivelSom);
  jsonData += "}";

  Serial.print("JSON montado: ");
  Serial.println(jsonData);

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

  delay(5000);
}
