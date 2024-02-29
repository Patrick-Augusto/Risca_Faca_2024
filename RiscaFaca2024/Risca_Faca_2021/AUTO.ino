void IRRead() {
  String value;
  if (irrecv.decode(&results))
  {
    value = String(results.value, HEX);
    Serial.println(value);
    irrecv.resume();
  }
  if (value == "10") {
    if (autoState == STOPPED) {
      //Serial.println("ReadyToGo");
      autoState = READY;
      MotorWrite(90, 90);
      //CalibrateSensors();
    }
  } else if (value == "810") {
    if (autoState == READY) {
      autoState = RUNNING;
      PS4.setLed(0, 100, 0);
      PS4.sendToController();
      //Serial.println("LET'S GO!!!");
    }
  } else if ( value == "410") {
    if (autoState == RUNNING || autoState == READY) {
      //Serial.println("STOP");
      autoState = STOPPED;
      MotorWrite(90, 90);
    }
  }
}
/*
void Star() {

  rightSensor = analogRead(rightSensorPin);
  leftSensor = analogRead(leftSensorPin);

  if (digitalRead(middleInfSensor)) {
    MotorWrite(130, 130);
  } else if (digitalRead(leftInfSensor)) {
    MotorWrite(80, 95);
    delay(100);
  } else if (digitalRead(rightInfSensor)) {
    MotorWrite(100, 80);
    delay(100);
  }

  if (leftSensor < leftSensorRef && rightSensor < rightSensorRef) {

    MotorWrite(80, 80);
    delay(200);
    MotorWrite(100, 80);
    delay(200);
  } else if (leftSensor < leftSensorRef) {
    MotorWrite(100, 80);
  } else  if (rightSensor < rightSensorRef) {
    MotorWrite(80, 95);
  } else {
    MotorWrite(110, 110);
  }
}*/

void Movimentacao() {
  static int movimento = 0;
  static bool direcao = true; 
  int cont=0;
  //rightInfSensor = digitalRead(rightInfSensor);
  //leftInfSensor = digitalRead(leftInfSensor);
  //middleInfSensor = digitalRead(middleInfSensor);

  if (digitalRead(middleInfSensor)) {
    MotorWrite(130, 130);
  } else if (digitalRead(leftInfSensor)) {
    MotorWrite(80, 95);
    delay(100);
  } else if (digitalRead(rightInfSensor)) {
    MotorWrite(100, 80);
    delay(100);
  }else if (digitalRead(leftInfSensorRef)) {
    MotorWrite(80, 95);
    delay(100);
  } else if (digitalRead(rightInfSensorRef)) {
    MotorWrite(100, 80);
    delay(100);
  }
   if (direcao) {
      //MotorWrite(-80, 100);
    //  delay(100);
    //} else {
     // MotorWrite(100, -80);
    //  delay(100);
    //}
    direcao = !direcao;
  } else if(digitalRead(rightInfSensor)==false) {

    
    do{
      if (movimento < 2) {
      MotorWrite(100,100);
      direcao = true;
    } else if (movimento < 1) {
      MotorWrite(100, 100);
      direcao = false;
    } else {
      movimento = 0; 
    }
    movimento++;
    cont++;
    
    } while(cont<=50 || digitalRead(rightInfSensor)==false );
    /*if (movimento < 2) {
      MotorWrite(100,100);
      direcao = true;
    } else if (movimento < 1) {
      MotorWrite(100, 100);
      direcao = false;
    } else {
      movimento = 0; 
    }
    movimento++;*/
  }
}

void Radar() {
  //Serial.println("StarStart");
  if (!desempate) {
    unsigned int timerStart = millis() + 300;
    while (timerStart > millis()) {
      MotorWrite(120, 120);
    }
  }

 
  while (autoState == RUNNING) {
    while ((!digitalRead(rightInfSensor) && !digitalRead(leftInfSensor)) && autoState == RUNNING) {
      //Serial.println("NotFind");
      IRRead();
      //Status_Verify();
      if (right) {
        MotorWrite(95, 110);
      } else {
        MotorWrite(110, 95);
      }
    }
    right = !right;
    while ((digitalRead(rightInfSensor) || digitalRead(leftInfSensor)) && autoState == RUNNING) {
      //Serial.println("Find");
      IRRead();
      //Status_Verify();
      MotorWrite(150, 150);
    }
  }
}

void Auto() {
  IRRead();
  if (PS4.Circle()) {
    Serial.println("RadarMode");
    tatic = RADAR;
  }
  if (PS4.Triangle()) {
    Serial.println("Movimentacao");
    tatic2 = MOVIMENTACAO;
  }
  if (PS4.Right()) {
    Serial.println("Right");
    right = true;
  }
  if (PS4.Left()) {
    Serial.println("Left");
    right = false;
  }
  if (PS4.Up()) {
    Serial.println("Up");
    desempate = false;
  }
  if (PS4.Down()) {
    Serial.println("Down");
    desempate = true;
  }

  if (autoState == RUNNING) {
    //if (tatic == STAR) {
  //    Star();
   // } else if (tatic == RADAR) {
      Radar();
      Movimentacao();
 //   }

  } else if (autoState == READY) {
    MotorWrite(90, 90);
    if (blinkTimer < millis()) {
      if (ledOn) {
        PS4.setLed(0, 0, 0);
        PS4.sendToController();
      } else {
        PS4.setLed(0, 100, 0);
        PS4.sendToController();
      }
      ledOn = !ledOn;
      blinkTimer = millis() + 200;
    }
  } else if (autoState == STOPPED) {
    MotorWrite(90, 90);
    if (blinkTimer < millis()) {
      if (ledOn) {
        PS4.setLed(0, ledIntensity++, 0);
        PS4.sendToController();
      } else {
        PS4.setLed(0, ledIntensity--, 0);
        PS4.sendToController();
      }
      if (ledIntensity == 0 || ledIntensity == 100) {
        ledOn = !ledOn;
      }
      blinkTimer = millis() + 10;
    }
  }
}
/*
void CalibrateSensors() {
  leftSensorRef = analogRead(leftSensorPin) - leftSensorTolerance;
  rightSensorRef = analogRead(rightSensorPin) - rightSensorTolerance;
}*/
