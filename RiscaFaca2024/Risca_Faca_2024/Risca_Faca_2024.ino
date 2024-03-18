// Code made by Nick
// Contact: nicolas_castrosilva@outlook.com

//Servo Libraries

#include <ESP32Servo.h>
#include <Arduino.h>

//PS4 Controller Libraries
#include <ps4.h>
#include <PS4Controller.h>
#include <ps4_int.h>

//Robot states of operation
enum robotStates {
  LOCKED, AUTO, MANUAL
};
robotStates robotState = LOCKED;
bool right = true;
bool desempate = false;

bool optionPressed = false;

//IR Remote library and variables
#include <IRremote.h>
#define irReceiverPin 23
IRrecv irrecv(irReceiverPin);
decode_results results;

//Presence Sensor Pins and Variables on the board
#define rightInfSensor 5 //17 5
#define leftInfSensor 19 //23 22 
#define middleInfSensor 18
#define leftInfSensorRef 99 //INSERIR O NUMERO NO ESP
#define rightInfSensorRef 98 //INSERIR O NUMERO NO ESP

void sensorTest();
void Status_Verify();
void MotorWrite();
void Controlado();
void IRRead();
void Suicidio();
void Radar();
void fradar();
void Auto();
void ManualControl();
void Teste();

//Auto mode states of operation (1,2,3 TV controll)
enum autoStates {
  READY, RUNNING, STOPPED
};
autoStates autoState = STOPPED;

enum tatics {
   RADAR,
   SUICIDIO,
   MOVIMENTACAO
   //TESTE
};
tatics tatic = RADAR;

//PS4 LED status variables
unsigned long blinkTimer;
bool ledOn = true;
int ledIntensity;

//Define Motors Pins and Instances
#define leftMotorPin 27
#define rightMotorPin 26

Servo MotorEsquerdo;
Servo MotorDireito;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  irrecv.enableIRIn(); //Enable IR Receiver

  //Start Connection between ESP32 and PS4 Controller
  PS4.begin("44:1c:a8:c6:41:80");
  while (!PS4.isConnected()) {
    Serial.println("WatingConnection");
    delay(250);
  }

  pinMode(leftInfSensor, INPUT);
  pinMode(middleInfSensor, INPUT);
  pinMode(rightInfSensor, INPUT);

  PS4.setLed(100, 0, 0);
  PS4.sendToController();

  MotorEsquerdo.attach(leftMotorPin);
  MotorDireito.attach(rightMotorPin);
  MotorEsquerdo.write(90);
  MotorDireito.write(90);

  Serial.println("Ready and LOCKED");
}

void loop() {

  //sensorTest();
  // put your main code here, to run repeatedly:
  if (PS4.isConnected()) {
    Status_Verify();
  } else {
    MotorEsquerdo.write(90);
    MotorDireito.write(90);
  }
}

void sensorTest() {
  //Serial.println("TestintSensors");
  if (digitalRead(leftInfSensor)) {
    Serial.println("Left");
  }
  if (digitalRead(middleInfSensor)) {
    Serial.println("Middle");
  }
  if (digitalRead(rightInfSensor)) {
    Serial.println("Right");
  }

}

void Status_Verify() {
  if (PS4.Options()) {
    if (!optionPressed) {
      optionPressed = true;
      if (robotState == LOCKED) {
        robotState = AUTO;
        PS4.setLed(0, 100, 0);
        PS4.sendToController();
        MotorEsquerdo.write(90);
        MotorDireito.write(90);
        Serial.println("AUTO");

      } else if (robotState == AUTO) {
        robotState = MANUAL;
        PS4.setLed(0, 0, 100);
        PS4.sendToController();
        MotorEsquerdo.write(90);
        MotorDireito.write(90);
        autoState = STOPPED;
        Serial.println("MANUAL");

      } else if (robotState == MANUAL) {
        robotState = LOCKED;
        autoState = STOPPED;
        PS4.setLed(100, 0, 0);
        PS4.sendToController();
        MotorEsquerdo.write(90);
        MotorDireito.write(90);
        Serial.println("LOCKED");
      }
    }
  } else {
    optionPressed = false;
  }

  if (robotState == MANUAL) {
    ManualControl();
  } else if (robotState == AUTO) {
    Auto();
  }
}

void MotorWrite(int ppmDireito, int ppmEsquerdo) {
  MotorDireito.write(ppmDireito);
  MotorEsquerdo.write(ppmEsquerdo);
}
