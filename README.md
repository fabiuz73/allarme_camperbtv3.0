# allarme_camperbtv3.0

Sistema di allarme per camper basato su tecnologia Bluetooth.

## Descrizione

Questo progetto nasce per proteggere il tuo camper, integrando sensori e attuatori gestiti tramite comunicazione Bluetooth. Permette di monitorare lo stato dei sensori, ricevere notifiche di allarme e controllare alcune funzioni direttamente da un'app o da dispositivi remoti.

## Componenti utilizzati

- **Scheda di sviluppo**: diymore per ESP32 Scheda di Sviluppo con Display LCD da 1,14 pollici 2,4 GHz WiFi Bluetooth per ESP32 Nodemcu USB-C Modulo con Chip CH340  
  ![image1](image1)
- **Sensore di movimento/accelerazione**: MPU6050 Modulo Sensore Accelerometro Giroscopio a 6 Assi Convertitore AD a 16 Bit Uscita Dati Accessori Professionali
- **Relay**: utilizzato per azionare una sirena di allarme
- **LED RGB**: KY-016 FZ0455 Modulo LED RGB a 3 colori per la segnalazione visiva degli stati di allarme

## Funzionalità principali

- Rilevamento di movimenti e intrusioni tramite accelerometro/giroscopio
- Gestione di allarmi sonori tramite relay e sirena
- Segnalazione visiva di stato e allarme tramite LED RGB
- Notifica su dispositivi Bluetooth associati
- Configurazione dei parametri di sicurezza
- Log degli eventi di allarme

## Requisiti

- Microcontrollore compatibile (ESP32 come descritto sopra)
- Sensore MPU6050
- Relay per sirena
- LED RGB KY-016
- Modulo Bluetooth integrato (ESP32)
- Alimentazione 12V

## Installazione

1. Clona il repository:
   ```bash
   git clone https://github.com/fabiuz73/allarme_camperbtv3.0.git
   ```
2. Carica il firmware sulla scheda ESP32
3. Collega i sensori e gli attuatori come da schema (vedi documentazione)
4. Configura i parametri di connessione Bluetooth

## Utilizzo

- Avvia il dispositivo
- Associa tramite Bluetooth il tuo smartphone o altro dispositivo
- Monitora lo stato e ricevi notifiche di allarme
- Osserva la segnalazione visiva tramite LED RGB

## Autore

Fabio Z.  
Altri progetti correlati: [allarme-camper-bt](https://github.com/fabiuz73/allarme-camper-bt), [allarme-camper](https://github.com/fabiuz73/allarme-camper)

## Licenza

Questo progetto è distribuito sotto licenza MIT.