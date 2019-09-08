
#include "LedControl.h"
#include <SimpleDHT.h>
#include <SPI.h>
#include <SD.h>

File archivo;
byte valPI;

LedControl lc = LedControl(9, 8, 7, 1); // 9=DIN, 8=CLK, 7=CS, 1=una sola matrix

int displayData[8];

int pinRele1 = 6;
int pinRele2 = 5;
int pinRele3 = 4;
int pinRele4 = 3;

int pinDHT11 = 2;
SimpleDHT11 dht11(pinDHT11);

int brillo;

int valRele;
int delayRele = 30; //humedad = delay entre los diferentes rele
int delayoff = 10; //temperatura = delay entre encendido y apagado del mismo rele
int n;

unsigned long tRef = millis();
int timeOn = 1000;

void setup() {

  /*
    The MAX72XX is in power-saving mode on startup,
    we have to do a wakeup call
  */
  lc.shutdown(0, false);
  /* Set the brightness 0-15 values */
  lc.setIntensity(0, 0);
  /* and clear the display */
  lc.clearDisplay(0);

  pinMode(pinRele1, OUTPUT);
  pinMode(pinRele2, OUTPUT);
  pinMode(pinRele3, OUTPUT);
  pinMode(pinRele4, OUTPUT);
  digitalWrite (pinRele1, HIGH);
  digitalWrite (pinRele2, HIGH);
  digitalWrite (pinRele3, HIGH);
  digitalWrite (pinRele4, HIGH);

  /*
    SD PinOUT:
    1=10
    2=11
    5=13
    7=12
  */
  SD.begin();
  archivo = SD.open("pimillon.txt");
  archivo.seek(0);

  Serial.begin(9600);

  for (int i = 7; i < 0; i--) {
    displayData[i] = 0;
  }

}

void loop() {
  
  if (archivo){
    while (archivo.available()) {
      shift();
      dataIn();
      matrix();
      rele();
      //sensado();
     
    }
    lc.clearDisplay(0);
    delay(500);
    for (int i = 0; i < 3; i++) {
    lc.setLed(0, 2, 0, true);
    lc.setLed(0, 5, 0, true);
    lc.setLed(0, 2, 1, true);
    lc.setLed(0, 5, 1, true);
    lc.setLed(0, 2, 2, true);
    lc.setLed(0, 5, 2, true);
    lc.setLed(0, 2, 3, true);
    lc.setLed(0, 5, 3, true);
    lc.setLed(0, 1, 4, true);
    lc.setLed(0, 2, 5, true);
    lc.setLed(0, 3, 5, true);
    lc.setLed(0, 4, 5, true);
    lc.setLed(0, 5, 5, true);
    lc.setLed(0, 6, 6, true);
    delay(500);
    lc.clearDisplay(0);
    delay(500);
    archivo.seek(0);
  }

  }else {
    archivo.close();
    }
}


void shift() {

for (int i = 0; i < 8; i++) {
    displayData[i] = displayData[i + 1];
  }
}


void dataIn() {

 valPI = archivo.parseInt();
 displayData[7] = 4 * (valPI);

 Serial.print(valPI);
 delay(timeOn);
  
}


void matrix() {
  for (int i = 0; i < 8; i++) {
    lc.setColumn(0, i, displayData[i]);
  }

}

void rele() {

  valRele = displayData[0] / 4;

  switch (valRele) {
    case 1:
      digitalWrite(pinRele1, LOW);
      delay(delayoff);
      digitalWrite(pinRele1, HIGH);
      break;
    case 2:
      digitalWrite(pinRele2, LOW);
      delay(delayoff);
      digitalWrite(pinRele2, HIGH);
      break;
    case 3:
      digitalWrite(pinRele1, LOW);
      delay(delayRele);
      digitalWrite(pinRele2, LOW);
      delay(delayoff);
      digitalWrite(pinRele1, HIGH);
      delay(delayRele);
      digitalWrite(pinRele2, HIGH);
      break;
    case 4:
      digitalWrite(pinRele3, LOW);
      delay(delayoff);
      digitalWrite(pinRele3, HIGH);
      break;
    case 5:
      digitalWrite(pinRele1, LOW);
      delay(delayRele);
      digitalWrite(pinRele3, LOW);
      delay(delayoff);
      digitalWrite(pinRele1, HIGH);
      delay(delayRele);
      digitalWrite(pinRele3, HIGH);
      break;
    case 6:
      digitalWrite(pinRele2, LOW);
      delay(delayRele);
      digitalWrite(pinRele3, LOW);
      delay(delayoff);
      digitalWrite(pinRele2, HIGH);
      delay(delayRele);
      digitalWrite(pinRele3, HIGH);
      break;
    case 7:
      digitalWrite(pinRele1, LOW);
      delay(delayRele);
      digitalWrite(pinRele2, LOW);
      delay(delayRele);
      digitalWrite(pinRele3, LOW);
      delay(delayoff);
      digitalWrite(pinRele1, HIGH);
      delay(delayRele);
      digitalWrite(pinRele2, HIGH);
      delay(delayRele);
      digitalWrite(pinRele3, HIGH);
      break;
    case 8:
      digitalWrite(pinRele4, LOW);
      delay(delayoff);
      digitalWrite(pinRele4, HIGH);
      break;
    case 9:
      digitalWrite(pinRele1, LOW);
      delay(delayRele);
      digitalWrite(pinRele4, LOW);
      delay(delayoff);
      digitalWrite(pinRele1, HIGH);
      delay(delayRele);
      digitalWrite(pinRele4, HIGH);
      break;
  }
  
}

void sensado() {

  if (millis() - tRef > 2000) {
    tRef = millis();
    byte temperature = 0;
    byte humidity = 0;
    int err = SimpleDHTErrSuccess;
    if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
      return;
    }
  brillo=(humidity/temperature)*2;  
  lc.setIntensity(0, brillo);
  
  delayRele = humidity;
  delayoff = temperature;

  Serial.println(temperature);
  Serial.print("");
  Serial.println(humidity);
  }
}
