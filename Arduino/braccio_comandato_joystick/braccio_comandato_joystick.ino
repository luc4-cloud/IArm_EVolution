/*
obbiettivo:
  realizzare un braccio robotico comandato sia da joystick sia da ai per il posizionamento dei dadi colorati nelle apposite posizioni
autore:
  Uboldi Luca
progetto:
  progetto interdisciplinare classe 4^
variabili:
  servoBase;  dichiarazione servo base
  servoGomito;  dichiarazione servo gomito
  servoPinza; dichiarazione servo pinza

  pinX; dichiarazione asse x joystick
  pinY; dichiarazione asse y joystick
  pinSW; dichiarazione pulsante joystick(per chiusura o apertura pinza)

  pinServoBase; dichiarazione pin Arduino servomotore base
  pinServoGomito; dichiarazione pin Arduino servomotore gomito
  pinServoPinza; dichiarazione pin Arduino servomotore pinza

  angoloBase; angolo servomotore base
  angoloGomito; angolo servomotore gomito
  angoloPinza; angolo servomotore pinza
  
  angoloBaseInizio; posizione servomotore base base
  angoloGomitoInizio; posizione servomotore gomito base
  angoloPinzaInizio; posizione servomotore pinza base

  ledVerde; dichiarazione pin Arduino per il led verde, rappresentante l'entrata nella modalità automatica
  ledRosso; dichiarazione pin Arduino per il led rosso, rappresentante l'entrata nella modalità manuale

  valvola; dichiarazione pin Arduino per la valvola, gestisce in che modalità si è attualmente(automatica o manuale)

  pinzaAperta; variabile boleana per indicare se la pinza è aperta o chiusa
  statoPrecedente; variabile boleana per indicare in che stato era precedentemente la pinza
*/
#include <Servo.h>

// dichiarazione servo
Servo servoBase;
Servo servoGomito;
Servo servoPinza;

// dichiarazione pin joystick
const int pinX = A0;
const int pinY = A1;
const int pinSW = 7;

// pin servo
const int pinServoBase = 9;
const int pinServoGomito = 10;
const int pinServoPinza = 11;

// angoli
int angoloBase = 90;
int angoloGomito = 180;
int angoloPinza = 90;

// posizioni base
int angoloBaseInizio = 90;
int angoloGomitoInizio = 180;
int angoloPinzaInizio = 90;

// led
const int ledVerde = 2;
const int ledRosso = 3;

// valvola
const int valvola = 6;

// pulsante
bool pinzaAperta = true;
bool statoPrecedente = true;

void setup() {
/*
  // posizione start
  servoBase.write(angoloBaseInizio);
  servoGomito.write(angoloGomitoInizio);
  servoPinza.write(angoloPinzaInizio);
*/
  // pulsante chiusura pinza
  pinMode(pinSW, INPUT_PULLUP);

  // led
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRosso, OUTPUT);
  
  // valvola
  pinMode(valvola, INPUT_PULLUP);

  delay(1000);
    // attacchi
  servoBase.attach(pinServoBase);
  servoGomito.attach(pinServoGomito);
  servoPinza.attach(pinServoPinza);


  // inizializzazione
  Serial.begin(9600); 
  Serial.println("pronto");
}

void loop() {
  // valori joystick
  int valX = analogRead(pinX);
  int valY = analogRead(pinY);

  // valori digitali valvola
  int stato = digitalRead(valvola);

  // zona morta: +/- 50 attorno al centro (512)
  int deadZone = 50;
  
  

  // se valvola attivata(led rosso, manuale)
  if(stato == LOW){
    Serial.println("sono nel loop manuale");
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledRosso, HIGH);

    // base senza zona morta, mappa i valori in base a questi limiti
    if (abs(valX - 512) > deadZone) {
      angoloBase += map(valX, 0, 1023, -2, 2);
    }

    // gomito senza zona morta, mappa i valori in base a questi limiti
    if (abs(valY - 512) > deadZone) {
      angoloGomito += map(valY, 0, 1023, -2, 2);
    }

    // limiti angoli(180)
    angoloBase = constrain(angoloBase, 0, 180);
    angoloGomito = constrain(angoloGomito, 0, 180);

    // movimento
    servoBase.write(angoloBase);
    servoGomito.write(angoloGomito);

    // aperto/chiuso pulsante pinza
    bool statoAttuale = digitalRead(pinSW);

    // per la pinza
    if (statoPrecedente == HIGH && statoAttuale == LOW) {
      // se premuto cambia
      pinzaAperta = !pinzaAperta;
      angoloPinza = pinzaAperta ? 90 : 30; // gradatura 90 = aperto      30 = chiuso
      servoPinza.write(angoloPinza);
      delay(20); 
    }

    statoPrecedente = statoAttuale; //aggiorna lo stato della pinza
  }else{  // se valvola non attivata(led rosso, manuale)
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledRosso, LOW);
    Serial.println("sono nel loop automatico");

    // leggo il valore di tipo string passato da seriale fino a capo
    String data = Serial.readStringUntil("\n");

    // rimuovo eventuali spazi o altri tipo di \...
    data.trim();
    
    // converte data in intero in modo da poter gestiro coi numeri 1, 2, 3, 4...
    int valoreRicevuto = data.toInt();

    servoBase.write(60);
    servoGomito.write(130);
    servoPinza.write(90);
    delay(800);
    

    // nel caso colore... allora ...
    switch(valoreRicevuto){
      case 1: // rosso
        // apertura pinza
        // apri la pinza solo se è chiusa
        servoBase.write(120);
        delay(800);
        servoGomito.write(140);
        delay(800);
        servoPinza.write(90);
        delay(800);
        servoPinza.write(30);
        delay(500);
        //oggetto preso e sollevato
        servoBase.write(40);
        delay(800);
        servoPinza.write(90);
        delay(800);
        // Solleva braccio
        
        // movimento effettivo braccio
        /**********
        inserire i valori dei movimento
        eseguo i movimenti del braccio:
        -prendo dado
        -chiudo pinza
        -sposto nel apposito posto
        -apro pinza
        -torno in posizione base
        **********/
        delay(10000);
        break;
      case 2: // verde
        // apertura pinza
        // apri la pinza solo se è chiusa
        servoBase.write(120);
        delay(800);
        servoGomito.write(140);
        delay(800);
        servoPinza.write(90);
        delay(800);
        servoPinza.write(30);
        delay(500);
        //oggetto preso e sollevato
        servoBase.write(140);
        delay(800);
        servoPinza.write(90);
        delay(800);
        // movimento effettivo braccio
        /**********
        inserire i valori dei movimento
        eseguo i movimenti del braccio:
        -prendo dado
        -chiudo pinza
        -sposto nel apposito posto
        -apro pinza
        -torno in posizione base
        **********/
        delay(10000);
        break;

      case 3: // blu
        servoBase.write(120);
        delay(800);
        servoGomito.write(140);
        delay(800);
        servoPinza.write(90);
        delay(800);
        servoPinza.write(30);
        delay(500);
        //oggetto preso e sollevato
        servoBase.write(90);
        delay(800);
        servoPinza.write(90);
        delay(800);
        // movimento effettivo braccio
        /**********
        inserire i valori dei movimento
        eseguo i movimenti del braccio:
        -prendo dado
        -chiudo pinza
        -sposto nel apposito posto
        -apro pinza
        -torno in posizione base
        **********/
        delay(10000);
        break;
    }
  }
  delay(20); 
}
