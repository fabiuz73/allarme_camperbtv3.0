# allarme_camperbtv3.0

Sistema di allarme per camper basato su tecnologia Bluetooth con supporto per due tipologie di schede ESP32.

## Descrizione

Questo progetto nasce per proteggere il tuo camper, integrando sensori e attuatori gestiti tramite comunicazione Bluetooth. Permette di monitorare lo stato dei sensori, ricevere notifiche di allarme e controllare alcune funzioni direttamente da un'app o da dispositivi remoti.

**Novità v3.0**: Il progetto ora supporta due configurazioni di schede ESP32:
- ESP32 con display integrato ST7789 (configurazione con feedback visivo)
- ESP32 USB-C NodeMCU (CP2102) senza display (configurazione compatta)

## Selezione della scheda

Prima di caricare il firmware sulla scheda ESP32, è necessario selezionare la configurazione corretta modificando il file `allarme_camperv.2.ino`:

### Per ESP32 con display (configurazione predefinita):
```cpp
#define BOARD_DISPLAY     // Per ESP32 con display
// #define BOARD_USB_C    // Per ESP32 USB C NodeMCU (CP2102) senza display
```

### Per ESP32 USB-C NodeMCU senza display:
```cpp
// #define BOARD_DISPLAY     // Per ESP32 con display
#define BOARD_USB_C    // Per ESP32 USB C NodeMCU (CP2102) senza display
```

**Nota importante**: Commentare una definizione e decommentare l'altra a seconda della scheda utilizzata.

## Componenti utilizzati

### Schede ESP32 supportate:

**Opzione 1: ESP32 con display integrato (BOARD_DISPLAY)**
- **Scheda di sviluppo**: diymore per ESP32 Scheda di Sviluppo con Display LCD da 1,14 pollici 2,4 GHz WiFi Bluetooth per ESP32 Nodemcu USB-C Modulo con Chip CH340  
  ![image1](image1)

**Opzione 2: ESP32 USB-C NodeMCU senza display (BOARD_USB_C)**  
- **Scheda di sviluppo**: ESP32 USB-C NodeMCU (CP2102) senza display - configurazione più compatta

### Componenti comuni a entrambe le schede:
- **Sensore di movimento/accelerazione**: MPU6050 Modulo Sensore Accelerometro Giroscopio a 6 Assi Convertitore AD a 16 Bit Uscita Dati Accessori Professionali
- **Relay**: utilizzato per azionare una sirena di allarme
- **LED RGB**: KY-016 FZ0455 Modulo LED RGB a 3 colori per la segnalazione visiva degli stati di allarme

## Collegamenti hardware

### Per ESP32 con display (BOARD_DISPLAY):

| Dispositivo                | Pin ESP32           | Funzione                      |
|----------------------------|---------------------|-------------------------------|
| Display LCD ST7789         | CS: GPIO15          | Chip Select                   |
|                            | DC: GPIO2           | Data/Command                  |
|                            | RST: GPIO4          | Reset                         |
|                            | BL: GPIO32          | Backlight                     |
| LED RGB (KY-016)           | R: GPIO12           | LED Rosso                     |
|                            | G: GPIO13           | LED Verde                     |
|                            | B: GPIO27           | LED Blu                       |
| Relè sirena                | GPIO14              | Attivazione sirena/clacson    |
| MPU6050 (I2C)              | SDA: GPIO21         | Comunicazione I2C             |
|                            | SCL: GPIO22         | Comunicazione I2C             |

### Per ESP32 USB-C NodeMCU senza display (BOARD_USB_C):

| Dispositivo                | Pin ESP32           | Funzione                      |
|----------------------------|---------------------|-------------------------------|
| LED RGB (KY-016)           | R: GPIO12           | LED Rosso                     |
|                            | G: GPIO13           | LED Verde                     |
|                            | B: GPIO27           | LED Blu                       |
| Relè sirena                | GPIO14              | Attivazione sirena/clacson    |
| MPU6050 (I2C)              | SDA: GPIO21         | Comunicazione I2C             |
|                            | SCL: GPIO22         | Comunicazione I2C             |

**Note:**
- Il LED RGB segnala lo stato (verde=disinserito, rosso=allarme, blu=stabilizzazione, bianco=test).
- Il relè è attivo livello LOW (LOW=attivato, HIGH=disattivato).
- **Solo per BOARD_DISPLAY**: Il display LCD ST7789 mostra messaggi di stato e allarme.
- **Per BOARD_USB_C**: I messaggi di stato vengono inviati solo via Serial e Bluetooth.
- Il sensore MPU6050 rileva movimenti e intrusioni tramite accelerometro/giroscopio.

## Funzionalità principali

- Rilevamento di movimenti e intrusioni tramite accelerometro/giroscopio
- Gestione di allarmi sonori tramite relay e sirena
- Segnalazione visiva di stato e allarme tramite LED RGB
- Notifica su dispositivi Bluetooth associati
- Configurazione dei parametri di sicurezza
- Log degli eventi di allarme

## Requisiti

### Requisiti comuni per entrambe le schede:
- Microcontrollore ESP32 (vedi sezione "Componenti utilizzati" per i modelli supportati)
- Sensore MPU6050
- Relay per sirena
- LED RGB KY-016
- Modulo Bluetooth integrato (ESP32)
- Alimentazione 12V

### Requisiti specifici:
- **BOARD_DISPLAY**: Librerie Adafruit_GFX e Adafruit_ST7789 per il display
- **BOARD_USB_C**: Nessuna libreria aggiuntiva per il display (feedback solo via Serial/Bluetooth)

## Installazione

1. Clona il repository:
   ```bash
   git clone https://github.com/fabiuz73/allarme_camperbtv3.0.git
   ```

2. **Selezione della scheda**: Prima di compilare, modifica il file `allarme_camperv.2.ino` per selezionare la scheda corretta:
   - Per ESP32 con display: assicurati che sia attiva `#define BOARD_DISPLAY`
   - Per ESP32 USB-C senza display: attiva `#define BOARD_USB_C` e commenta `BOARD_DISPLAY`

3. **Installazione librerie** (solo per BOARD_DISPLAY):
   - Adafruit_GFX
   - Adafruit_ST7789
   - MPU6050 (comune a entrambe le schede)
   - BluetoothSerial (incluso in ESP32 Core)

4. Carica il firmware sulla scheda ESP32

5. Collega i sensori e gli attuatori come da schema (vedi sezione "Collegamenti hardware")

6. Configura i parametri di connessione Bluetooth

## Utilizzo

- Avvia il dispositivo
- Associa tramite Bluetooth il tuo smartphone o altro dispositivo con nome "ESP32-CAMPER"
- Monitora lo stato attraverso:
  - **BOARD_DISPLAY**: Display LCD per feedback visivo + LED RGB + Serial/Bluetooth
  - **BOARD_USB_C**: LED RGB + Serial/Bluetooth (no display)
- Ricevi notifiche di allarme
- Comandi disponibili via Bluetooth:
  - `1`: Inserisce allarme
  - `0`: Disinserisce allarme  
  - `T`: Test clacson
  - `SOGLIA:xxxx`: Imposta soglia movimento (es. SOGLIA:1500)
  - `GET_SOGLIA`: Visualizza soglia corrente

## Autore

Fabio Z.  
Altri progetti correlati: [allarme-camper-bt](https://github.com/fabiuz73/allarme-camper-bt), [allarme-camper](https://github.com/fabiuz73/allarme-camper)

## Licenza

Questo progetto è distribuito sotto licenza MIT.
