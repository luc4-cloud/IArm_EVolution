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

// start
int angoloBase = 90;
int angoloGomito = 90;
int angoloPinza = 90;

// pulsante
bool pinzaAperta = true;
bool statoPrecedente = true;

void setup() {
  //attacchi
  servoBase.attach(pinServoBase);
  servoGomito.attach(pinServoGomito);
  servoPinza.attach(pinServoPinza);
  //pulsante chiusura pinza
  pinMode(pinSW, INPUT_PULLUP);

  // posizione start
  servoBase.write(angoloBase);
  servoGomito.write(angoloGomito);
  servoPinza.write(angoloPinza);

  Serial.begin(9600); 
}

void loop() {
  // valori joystick
  int valX = analogRead(pinX);
  int valY = analogRead(pinY);

  // zona morta: +/- 50 attorno al centro (512)
  int deadZone = 50;

  // base senza zona morta
  if (abs(valX - 512) > deadZone) {
    angoloBase += map(valX, 0, 1023, -2, 2);
  }

  // gomito senza zona morta
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

  if (statoPrecedente == HIGH && statoAttuale == LOW) {
    // se premuto cambia
    pinzaAperta = !pinzaAperta;
    angoloPinza = pinzaAperta ? 90 : 30; // gradatura 90 = aperto      30 = chiuso
    servoPinza.write(angoloPinza);
    delay(200); 
  }

  statoPrecedente = statoAttuale; //aggiorna lo stato della pinza

  delay(20); 
}
