#include <SevSeg.h>

SevSeg sevseg;  // Instanța obiectului pentru display-ul 7-segment

const int micPin = A2;      // Pinul analogic la care este conectat KY-037
const int ledPin = A3;      // Pinul digital pentru LED
const int redLedPin = A4;   // Pinul digital pentru LED
const int buzzerPin = A5;   // Pinul pentru buzzer
const int threshold = 620;  // Pragul pentru detectarea sunetului

bool ledState = false;            // Starea curentă a LED-ului
bool stopwatchRunning = false;    // Starea cronometrei (pornit/oprire)
unsigned long startMillis = 0;    // Momentul în care cronometru a început
unsigned long currentMillis = 0;  // Timpul curent
unsigned long elapsedMillis = 0;  // Timpul scurs, pentru continuare
int seconds = 0;                  // Secunde pentru cronometru
int minutes = 0;                  // Minute pentru cronometru
const int buzzerFreq = 690;

const unsigned long beepInterval = 1000;  // Intervalul de timp între bipuri (1 secundă)
unsigned long lastBeepMillis = 0;         // Ultimul moment când am făcut un bip
unsigned long lastSecondMillis = 0;       // Ultima secundă în care cronometru a fost actualizat
unsigned long lastChangeMillis = 0;       // Ultima schimbare de secundă

// Variabile pentru detectarea dublei bătăi
unsigned long lastClapTime = 0;          // Timpul ultimei bătăi detectate
unsigned long doubleClapInterval = 500;  // Intervalul maxim între două bătăi (500ms)
int clapCount = 0;                       // Numără bătăile detectate

void setup() {
  pinMode(ledPin, OUTPUT);     // Configurează pinul LED-ului ca ieșire
  pinMode(redLedPin, OUTPUT);  // Configurează pinul LED-ului ca ieșire
  pinMode(buzzerPin, OUTPUT);  // Configurează pinul buzzer-ului ca ieșire
  Serial.begin(115200);        // Inițializează comunicația serială

  // Setup pentru display-ul 7-segment
  byte numDigits = 4;                                    // Display-ul are 4 cifre
  byte digitPins[] = { 5, 4, 3, 2 };                     // Pinii pentru cifre
  byte segmentPins[] = { 7, 8, 9, 10, 11, 12, 13, A0 };  // Pinii pentru segmente (A-G + DP)
  sevseg.begin(COMMON_CATHODE, numDigits, digitPins, segmentPins);
  sevseg.setBrightness(50);  // Setează luminozitatea display-ului

  digitalWrite(ledPin, ledState ? HIGH : LOW);
  digitalWrite(redLedPin, ledState ? LOW : HIGH);
}

void loop() {
  int soundLevel = analogRead(micPin);  // Citește valoarea de la microfon
  currentMillis = millis();             // Timpul curent în milisecunde

  // Dacă valoarea de la microfon depășește pragul și nu am detectat deja o bătaie
  if (soundLevel > threshold) {  // Dacă valoarea depășește pragul
    if (currentMillis - lastClapTime < doubleClapInterval) {
      clapCount++;  // Incrementăm numărul de bătăi detectate
    } else {
      clapCount = 1;  // Resetăm numărătoarea dacă timpul între bătăi e prea mare
    }
    lastClapTime = currentMillis;

    if (clapCount == 2) {
      // Resetăm timerul
      stopwatchRunning = false;
      elapsedMillis = 0;
      startMillis = currentMillis;
      seconds = 0;
      minutes = 0;
      Serial.println("Timer reset by double clap!");
      sevseg.setNumber(0, 2);  // Resetăm afișajul

      ledState =false;
      digitalWrite(ledPin, ledState ? HIGH : LOW);     // Actualizează starea LED-ului
      digitalWrite(redLedPin, ledState ? LOW : HIGH);  // Actualizează starea LED-ului

      tone(buzzerPin, buzzerFreq + 1000);     // Pornește buzzer-ul
      lastBeepMillis = currentMillis;  // Salvează timpul pentru următorul bip
      lastChangeMillis = seconds;      // Salvează secunda curentă

    } else {
      ledState = !ledState;                            // Inversează starea LED-ului
      digitalWrite(ledPin, ledState ? HIGH : LOW);     // Actualizează starea LED-ului
      digitalWrite(redLedPin, ledState ? LOW : HIGH);  // Actualizează starea LED-ului

      if (!stopwatchRunning) {
        stopwatchRunning = true;      // Începe cronometru
        startMillis = currentMillis;  // Salvează momentul de start
        Serial.println("Stopwatch Started");
      } else {
        stopwatchRunning = false;                      // Oprește cronometru
        elapsedMillis += currentMillis - startMillis;  // Salvează timpul scurs până în acest moment
        Serial.println("Stopwatch Paused");
      }
    }
    delay(100);  // Evită detectarea multiplă pentru aceeași bătaie
  }

  // Dacă cronometru este activat, actualizăm timpul
  if (stopwatchRunning) {
    unsigned long elapsedTime = currentMillis - startMillis + elapsedMillis;  // Timpul total scurs
    seconds = elapsedTime / 1000;                                             // Calculăm secunde
    minutes = seconds / 60;                                                   // Calculăm minute
    seconds = seconds % 60;                                                   // Rămânem cu restul secundelor

    // Afișăm timpul pe display-ul 7-segment
    int displayTime = (minutes * 100) + seconds;  // Combinăm minute și secunde într-un număr întreg
    sevseg.setNumber(displayTime, 2);             // Afișează timpul pe display

    // Emitere bip exact când secunda se schimbă
    if (seconds != lastChangeMillis) {
      tone(buzzerPin, buzzerFreq);     // Pornește buzzer-ul
      lastBeepMillis = currentMillis;  // Salvează timpul pentru următorul bip
      lastChangeMillis = seconds;      // Salvează secunda curentă
    }

    // Oprește buzzer-ul după 100 ms
    if (currentMillis - lastBeepMillis >= 100) {
      noTone(buzzerPin);  // Oprește buzzer-ul
    }

  } else {
    // Dacă cronometru este pe pauză, afișează timpul acum
    if (currentMillis - lastBeepMillis >= 100) {
      noTone(buzzerPin);  // Oprește buzzer-ul
    }

    int displayTime = (minutes * 100) + seconds;  // Afișează timpul (minute.secunde)
    sevseg.setNumber(displayTime, 2);             // Afișează timpul pe display
  }
  sevseg.refreshDisplay();  // Actualizează display-ul
}
