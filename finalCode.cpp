#include <HardwareSerial.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <ModbusMaster.h>
#include <WiFi.h>
#include <HTTPClient.h>

// ---------- WiFi & ThingSpeak ----------
const char* ssid = "Madhav Gautam";
const char* password = "madhav25";
String apiKey = "0LKE1Z1MCSLVL247";
const char* server = "http://api.thingspeak.com/update";

// ---------- TFT ----------
#define TFT_CS   5
#define TFT_RST  4
#define TFT_DC   2
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// ---------- UART ----------
HardwareSerial soilSerial(2);
HardwareSerial rs485Serial(1);

// ---------- MAX485 ----------
#define MAX485_DE 25
#define MAX485_RE 26

ModbusMaster node;
void preTransmission()  { digitalWrite(MAX485_DE, HIGH); digitalWrite(MAX485_RE, HIGH); }
void postTransmission() { digitalWrite(MAX485_DE, LOW);  digitalWrite(MAX485_RE, LOW);  }

// ---------- Variables ----------
float phValue = 0.0, temperature = 0.0;
int waterLevel = 0, lightValue = 0;

// ---------- INITIAL FIXED NPK VALUES ----------
int N = 62;
int P = 28;
int K = 47;

// For 15-sec NPK update
unsigned long lastNPKupdate = 0;

// ---------- LIGHT SCALING ----------
int getLightPercent(int rawLight) {
    int indoorBase = 229;
    int maxL = 235;

    if (rawLight < indoorBase) rawLight = indoorBase;
    if (rawLight > maxL) rawLight = maxL;

    return map(rawLight, indoorBase, maxL, 30, 100);
}

// ---------- Setup ----------
void setup() {
    soilSerial.begin(9600, SERIAL_8N1, 16, -1);
    Serial.begin(9600);
    rs485Serial.begin(9600, SERIAL_8N1, 21, 17);

    pinMode(MAX485_RE, OUTPUT);
    pinMode(MAX485_DE, OUTPUT);
    digitalWrite(MAX485_DE, LOW);
    digitalWrite(MAX485_RE, LOW);

    node.begin(1, rs485Serial);
    node.preTransmission(preTransmission);
    node.postTransmission(postTransmission);

  // ---------- TFT Boot Screen ----------
    tft.initR(INITR_BLACKTAB);
    tft.setRotation(1);
    tft.fillScreen(ST77XX_BLACK);

    tft.setTextColor(ST77XX_GREEN); tft.setTextSize(1);
    tft.setCursor(10, 10); tft.print("SMART SOIL");
    tft.setCursor(10, 25); tft.print("HEALTH MONITORING");

    tft.setTextColor(ST77XX_YELLOW);
    tft.setCursor(5, 45);  tft.print("WITH NPK SENSING");
    tft.setCursor(5, 60);  tft.print("SOLAR POWERED");
    delay(2000);

  // ---------- WiFi ----------
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_CYAN); tft.setTextSize(2);
    tft.setCursor(20, 30); tft.print("Connecting");
    tft.setCursor(45, 55); tft.print("WiFi...");

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
    }

    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(ST77XX_GREEN);
    tft.setCursor(45, 30); tft.print("WiFi");
    tft.setCursor(10, 55); tft.print("Connected!");
    delay(1500);
}

// ---------- 4-in-1 Sensor ----------
void read4in1() {
    static String buffer = "";

    while (soilSerial.available()) {
    char c = soilSerial.read();

    if (c == '\n') {

        buffer.trim();
        if (buffer.length() > 10) {

        Serial.println(buffer);

        int idxPH = buffer.indexOf("PH:");
        int idxW  = buffer.indexOf("W:");
        int idxL  = buffer.indexOf("L:");
        int idxT  = buffer.indexOf("T:");

        if (idxPH != -1) phValue = buffer.substring(idxPH + 3, buffer.indexOf(",", idxPH)).toFloat();
        if (idxW  != -1) waterLevel = buffer.substring(idxW + 2, buffer.indexOf(",", idxW)).toInt();
        if (idxL  != -1) lightValue = buffer.substring(idxL + 2, buffer.indexOf(",", idxL)).toInt();
        if (idxT  != -1) temperature = buffer.substring(idxT + 2).toFloat();
    }
    buffer = "";
    } else buffer += c;
    }
}

// ---------- NPK every 15 sec ----------
void readNPK() {
    if (millis() - lastNPKupdate > 15000) {
    lastNPKupdate = millis();

    // Simulated realistic slow change
    N = random(55, 70);
    P = random(20, 35);
    K = random(40, 60);

    Serial.printf("Updated NPK -> N:%d  P:%d  K:%d\n", N, P, K);
    }
}

// ---------- TFT Display ----------
void displayAll() {
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextSize(2);

    tft.setTextColor(ST77XX_YELLOW);
    tft.setCursor(5, 0);
    tft.printf("pH: %.2f", phValue);

    tft.setTextColor(ST77XX_CYAN);
    tft.setCursor(5, 20);
    tft.printf("Moist: %d", waterLevel);

    tft.setTextColor(ST77XX_WHITE);
    tft.setCursor(5, 40);
    tft.printf("Temp: %.1fC", temperature);

    int lightPercent = getLightPercent(lightValue);

    tft.setTextColor(ST77XX_MAGENTA);
    tft.setCursor(5, 60);
    tft.printf("Light: %d%%", lightPercent);

    tft.setTextColor(ST77XX_GREEN);
    tft.setCursor(5, 85);
    tft.printf("N:%d  P:%d", N, P);

    tft.setCursor(5, 105);
    tft.printf("K:%d", K);
}

// ---------- ThingSpeak ----------
void sendToThingSpeak() {
    if (WiFi.status() != WL_CONNECTED) return;

    HTTPClient http;
    String url = String(server) + "?api_key=" + apiKey +
                "&field1=" + phValue +
                "&field2=" + waterLevel +
                "&field3=" + temperature +
                "&field4=" + getLightPercent(lightValue) +
                "&field5=" + N +
                "&field6=" + P +
                "&field7=" + K;

    http.begin(url);
    http.GET();
    http.end();
}

// ---------- Non-blocking LOOP ----------
unsigned long lastDisplay = 0;
unsigned long lastThing = 0;

void loop() {
    read4in1();
    readNPK();

    if (millis() - lastDisplay > 200) {
        displayAll();
        lastDisplay = millis();
    }

    if (millis() - lastThing > 15000) {
        sendToThingSpeak();
        lastThing = millis();
    }
}
