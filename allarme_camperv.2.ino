#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "BluetoothSerial.h"
#include <Wire.h>
#include <MPU6050.h>

// Pin Display ST7789
#define TFT_CS    15
#define TFT_DC    2
#define TFT_RST   4
#define TFT_BL    32

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
BluetoothSerial SerialBT;

// Pin collegamento hardware LED RGB KY-016
const int ledR = 12;   // Rosso
const int ledG = 13;   // Verde
const int ledB = 27;   // Blu
const int releClacson = 14;

// Soglia movimento (modificabile da Bluetooth)
int sogliaMovimento = 1000;

// --- Stabilizzazione ---
const unsigned long stabilizzazioneSec = 10; // tempo in secondi
unsigned long allarmeAttivatoMillis = 0;
bool allarmeInserito = false;
bool giaConnesso = false;
bool stabilizzazioneScritta = false;

// Variabili per auto-taratura dei centri
long sommaX = 0, sommaY = 0, sommaZ = 0;
int campioni = 0;
int centroX = 0, centroY = 0, centroZ = 0;
bool centriCalcolati = false;

// MPU6050
MPU6050 mpu;

// Funzione per mostrare testo centrato sul display
void showMessage(String testo, uint16_t colore = ST77XX_GREEN) {
  tft.fillScreen(ST77XX_BLACK);
  int16_t x1, y1;
  uint16_t w, h;
  tft.setTextSize(2);
  tft.setTextColor(colore);
  tft.getTextBounds(testo, 0, 0, &x1, &y1, &w, &h);
  int x = (tft.width()  - w) / 2 - x1;
  int y = (tft.height() - h) / 2 - y1;
  tft.setCursor(x, y);
  tft.print(testo);
}

// Funzione per impostare il colore RGB (0=spento, 255=acceso)
void setLedColor(int r, int g, int b) {
  analogWrite(ledR, r);
  analogWrite(ledG, g);
  analogWrite(ledB, b);
}

void setup() {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  tft.init(240, 320);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);

  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  pinMode(releClacson, OUTPUT);
  setLedColor(0, 255, 0); // Allarme disinserito: verde acceso

  // MODIFICA: Relè disattivato all'accensione (HIGH su modulo attivo LOW)
  digitalWrite(releClacson, HIGH);

  Serial.begin(115200);
  SerialBT.begin("ESP32-CAMPER");
  SerialBT.println("Allarme pronto!");

  showMessage("ALLARME CAMPER", ST77XX_GREEN);

  // MPU6050 INIT
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    showMessage("ERRORE MPU6050!", ST77XX_RED);
    Serial.println("MPU6050 non collegato!");
    while (1);
  }
}

void loop() {
  // Stato connessione Bluetooth
  if (SerialBT.hasClient()) {
    if (!giaConnesso) {
      showMessage("ALLARME CONNESSO", ST77XX_YELLOW);
      giaConnesso = true;
    }
  } else {
    if (giaConnesso) {
      showMessage("ALLARME CAMPER", ST77XX_GREEN);
      giaConnesso = false;
    }
  }

  // Ricezione comandi Bluetooth
  if (SerialBT.available()) {
    String comando = SerialBT.readStringUntil('\n');
    comando.trim();
    Serial.println("Comando ricevuto via Bluetooth: " + comando);

    if (comando.length() == 0) return; // IGNORA COMANDI VUOTI

    if (comando.startsWith("SOGLIA:")) {
      String sogliaStr = comando.substring(7);
      int nuovaSoglia = sogliaStr.toInt();
      if (nuovaSoglia > 0 && nuovaSoglia < 5000) {
        sogliaMovimento = nuovaSoglia;
        SerialBT.println("Soglia aggiornata: " + String(sogliaMovimento));
        Serial.println("Soglia aggiornata: " + String(sogliaMovimento));
        showMessage("SOGLIA: " + String(sogliaMovimento), ST77XX_CYAN);
        delay(1000);
        showMessage(allarmeInserito ? "ALLARME INSERITO" : "ALLARME DISINSERITO", allarmeInserito ? ST77XX_RED : ST77XX_GREEN);
      } else {
        SerialBT.println("Valore soglia non valido!");
        Serial.println("Valore soglia non valido!");
        showMessage("SOGLIA ERRATA", ST77XX_YELLOW);
        delay(1000);
        showMessage(allarmeInserito ? "ALLARME INSERITO" : "ALLARME DISINSERITO", allarmeInserito ? ST77XX_RED : ST77XX_GREEN);
      }
    }
    else if (comando == "GET_SOGLIA") {
      SerialBT.println("Soglia attuale: " + String(sogliaMovimento));
      Serial.println("Soglia attuale: " + String(sogliaMovimento));
    }
    else if (comando == "1") {
      allarmeInserito = true;
      allarmeAttivatoMillis = millis(); // memorizza il tempo di attivazione
      SerialBT.println("ALLARME INSERITO");
      Serial.println("Allarme inserito. Stabilizzazione...");
      showMessage("ALLARME INSERITO", ST77XX_RED);
      stabilizzazioneScritta = false; // resetta la scritta stabilizzazione

      // Reset variabili per auto-taratura
      sommaX = 0; sommaY = 0; sommaZ = 0;
      campioni = 0;
      centriCalcolati = false;
    } else if (comando == "0") {
      allarmeInserito = false;
      setLedColor(0, 255, 0); // VERDE acceso
      digitalWrite(releClacson, HIGH); // MODIFICA: Spegne il clacson (HIGH su attivo LOW)
      SerialBT.println("ALLARME DISINSERITO");
      Serial.println("Allarme disinserito.");
      showMessage("ALLARME DISINSERITO", ST77XX_GREEN);
      stabilizzazioneScritta = false;
      delay(500);
    } else if (comando == "T") {
      setLedColor(255, 255, 255); // BIANCO per test clacson
      digitalWrite(releClacson, LOW); // MODIFICA: Attiva clacson (LOW su attivo LOW)
      SerialBT.println("Test clacson attivato");
      Serial.println("Test clacson attivato.");
      showMessage("TEST CLACSON!", ST77XX_CYAN);
      delay(1000);
      digitalWrite(releClacson, HIGH); // MODIFICA: Spegne clacson
      setLedColor(allarmeInserito ? 255 : 0, allarmeInserito ? 0 : 255, 0); // Ripristina stato led
      showMessage(allarmeInserito ? "ALLARME INSERITO" : "ALLARME DISINSERITO", allarmeInserito ? ST77XX_RED : ST77XX_GREEN);
    } else {
      SerialBT.println("Comando non riconosciuto");
      Serial.println("Comando non riconosciuto!");
      showMessage("COMANDO ERRATO", ST77XX_YELLOW);
      delay(1000);
      showMessage(allarmeInserito ? "ALLARME INSERITO" : "ALLARME DISINSERITO", allarmeInserito ? ST77XX_RED : ST77XX_GREEN);
    }
  }

  // --- LOGICA ALLARME ---
  if (allarmeInserito) {
    unsigned long tempoTrascorso = (millis() - allarmeAttivatoMillis) / 1000;

    // Durante stabilizzazione: accumula valori
    if (tempoTrascorso < stabilizzazioneSec) {
      int16_t ax, ay, az;
      mpu.getAcceleration(&ax, &ay, &az);
      sommaX += ax; sommaY += ay; sommaZ += az; campioni++;
      if (!stabilizzazioneScritta) {
        showMessage("STABILIZZAZIONE...", ST77XX_BLUE);
        stabilizzazioneScritta = true;
      }
      // BLU lampeggiante durante stabilizzazione
      setLedColor(0, 0, 255); // Blu acceso
      delay(250);
      setLedColor(0, 0, 0);   // Tutto spento
      delay(250);
      return;
    } else {
      // Dopo stabilizzazione: calcola centro una sola volta
      if (!centriCalcolati && campioni > 0) {
        centroX = sommaX / campioni;
        centroY = sommaY / campioni;
        centroZ = sommaZ / campioni;
        Serial.print("Centri calcolati: ");
        Serial.print("X: "); Serial.print(centroX);
        Serial.print(" Y: "); Serial.print(centroY);
        Serial.print(" Z: "); Serial.println(centroZ);
        centriCalcolati = true;
        showMessage("ALLARME INSERITO", ST77XX_RED);
        stabilizzazioneScritta = false;
      }
    }

    int16_t ax, ay, az;
    mpu.getAcceleration(&ax, &ay, &az);

    int deltaX = abs(ax - centroX);
    int deltaY = abs(ay - centroY);
    int deltaZ = abs(az - centroZ);

    Serial.print("ax: "); Serial.print(ax);
    Serial.print(" ay: "); Serial.print(ay);
    Serial.print(" az: "); Serial.print(az);
    Serial.print(" | DeltaX: "); Serial.print(deltaX);
    Serial.print(" DeltaY: "); Serial.print(deltaY);
    Serial.print(" DeltaZ: "); Serial.println(deltaZ);

    if (deltaX > sogliaMovimento ||
        deltaY > sogliaMovimento ||
        deltaZ > sogliaMovimento) {
      setLedColor(255, 0, 0); // Rosso fisso
      digitalWrite(releClacson, LOW); // MODIFICA: Attiva clacson (LOW su attivo LOW)
      SerialBT.println("Movimento rilevato! Clacson attivo!");
      Serial.println("Movimento rilevato! Clacson attivato!");
      showMessage("MOVIMENTO RILEVATO!", ST77XX_YELLOW);
      delay(2000);
      digitalWrite(releClacson, HIGH); // MODIFICA: Spegne clacson
      setLedColor(255, 0, 0); // Rimane rosso fisso per 1 secondo dopo l'allarme
      showMessage("ALLARME INSERITO", ST77XX_RED);
      delay(1000);
    } else {
      // ALLARME INSERITO: ROSSO LAMPEGGIANTE, blu spento
      setLedColor(255, 0, 0); // Rosso acceso
      delay(400);
      setLedColor(0, 0, 0);   // Tutto spento
      delay(400);
    }
  } else {
    // ALLARME DISINSERITO: VERDE ACCESO, blu spento
    setLedColor(0, 255, 0); // Verde acceso
    digitalWrite(releClacson, HIGH); // MODIFICA: Spegne clacson
    stabilizzazioneScritta = false; // resetta la variabile
  }
}