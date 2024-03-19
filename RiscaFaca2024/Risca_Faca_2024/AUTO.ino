void IRRead() {
  String value;
  if (irrecv.decode(&results))
  {
    Serial.println(results.value);
    value = String(results.value, HEX);
    Serial.println(value);
    irrecv.resume();
  }
  if (value == "10") {
    if (autoState == STOPPED) {
      //Serial.println("ReadyToGo");
      autoState = READY;
      PS4.setLed(0, 100, 100);
      PS4.sendToController();
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
      PS4.setLed(100, 100, 0);
      PS4.sendToController();
      MotorWrite(90, 90);
    }
  }
}

// void Teste(){
//   Serial.println("TesteStart");
//   if (!desempate) {
//     unsigned int timerStart = millis() + 300;
//     while (timerStart > millis()) {
//       MotorWrite(120, 120);
//     }
//   }

//   while (autoState == RUNNING) {
//     while ((digitalRead(middleInfSensor) && digitalRead(leftInfSensor) && digitalRead(rightInfSensor) ) && autoState == RUNNING) {
//       //Serial.println("NotFind");
//       IRRead();
//       //Status_Verify();
//       if (right) {
//         MotorWrite(95, 110);
//       } else {
//         MotorWrite(110, 95);
//       }
//     }
//     right = !right;
//     while ((!digitalRead(middleInfSensor) && !digitalRead(!leftInfSensor) && !digitalRead(!rightInfSensor)) && autoState == RUNNING) {

//       if (!digitalRead(middleInfSensor)){
//       //Serial.println("Find");
//       IRRead();
//       //Status_Verify();
//       MotorWrite(150, 150);
//       } else if (!digitalRead(!leftInfSensor)){
//         while (digitalRead(middleInfSensor))
//         {
//           IRRead();
//           MotorWrite(95, 110);
//         }
        
//       } else if (!digitalRead(!rightInfSensor)){
//           while (digitalRead(middleInfSensor))
//         {
//           IRRead();
//           MotorWrite(110, 95);
//         }
//       }
//     }
//     while (autoState == STOPPED) {   
//       IRRead();
//       MotorWrite(0, 0);
//       }
//   }
// }
//
void Movimentacao() {
  static int movimento = 0;
  static bool direcao = true; 
  int cont=0;
  rightInfSensor = digitalRead(rightInfSensor);
  leftInfSensor = digitalRead(leftInfSensor);
  middleInfSensor = digitalRead(middleInfSensor);

  if (digitalRead(middleInfSensor)==true) {
    MotorWrite(130, 130);
  } else if (digitalRead(leftInfSensor)==true) {
    MotorWrite(80, 95);
    delay(100);
  } else if (digitalRead(rightInfSensor)==true) {
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
  } else if(digitalRead(middleInfSensor)) {

    
    do{
      if (movimento < 2) {
      MotorWrite(100,100);
      direcao = true;
    } else if (movimento < 1) {
      MotorWrite(80, 80);
      direcao = false;
    } else {
      movimento = 0; 
    }
    movimento++;
    cont++;
    
    } while(cont<=50 || digitalRead(middleInfSensor)==false );
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
  Serial.println("RadarStart");
  if (!desempate) {
    unsigned int timerStart = millis() + 300;
    while (timerStart > millis()) {
      MotorWrite(120, 120);
    }
  }

  while (autoState == RUNNING) {
    while ((digitalRead(rightInfSensor) && digitalRead(leftInfSensor) && digitalRead(middleInfSensor)) && autoState == RUNNING) {
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
    while (!digitalRead(middleInfSensor) && autoState == RUNNING) {
      //Serial.println("Find");
      IRRead();
      //Status_Verify();
      MotorWrite(150, 150);
    }
    while (autoState == STOPPED) {   
      IRRead();
      MotorWrite(0, 0);
      }
  }
}
void RadarInverso() {
  Serial.println("RadarInversoStart");
  if (!desempate) {
    unsigned int timerStart = millis() + 300;
    while (timerStart > millis()) {
      MotorWrite(120, 120);
    }
  }

  while (autoState == RUNNING) {
    // Verifica se os sensores identificam um robô adversário
    if (digitalRead(rightInfSensor) == LOW || digitalRead(leftInfSensor) == LOW || digitalRead(middleInfSensor) == LOW) {
      // Realiza uma volta de ré
      while (digitalRead(middleInfSensor) == LOW && autoState == RUNNING) {
        IRRead();
        MotorWrite(-120, -120); // Marcha ré
      }
      delay(200); // Delay para garantir que a volta de ré foi completada
    }
    
    // Avança para frente
    while ((digitalRead(rightInfSensor) && digitalRead(leftInfSensor) && digitalRead(middleInfSensor)) && autoState == RUNNING) {
      IRRead();
      if (right) {
        MotorWrite(95, 110);
      } else {
        MotorWrite(110, 95);
      }
    }
    right = !right;
    while (!digitalRead(middleInfSensor) && autoState == RUNNING) {
      IRRead();
      MotorWrite(150, 150);
    }
    while (autoState == STOPPED) {   
      IRRead();
      MotorWrite(0, 0);
    }
  }
}

void Suicidio() {
  Serial.println("SuicidioStart");
  if (!desempate) {
    unsigned int timerStart = millis() + 300;
    while (timerStart > millis()) {
      MotorWrite(120, 120);
    }
  }

  while (autoState == RUNNING) {  
      IRRead();
      while(!digitalRead(middleInfSensor)){
        IRRead();
      MotorWrite(150, 150);
      PS4.setLed(100, 0, 0);
      PS4.sendToController();
      }
      }
  while (autoState == STOPPED) {   
      IRRead();
      MotorWrite(0, 0);
      }
}

void Auto() {
  IRRead();
  if (PS4.Square()) {
    Serial.println("RadarInversoMode");
    tatic = RADAR_INVERSO;
  }
  if (PS4.Circle()) {
    Serial.println("RadarMode");
    tatic = RADAR;
  }
  if (PS4.Triangle()) {
    Serial.println("MovimentacaoMode");
    tatic = MOVIMENTACAO;
  }
  if (PS4.Cross()) {
    Serial.println("SuicideMode");
    tatic = SUICIDIO;
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
    if (tatic == SUICIDIO) {
      Suicidio();
    } else if (tatic == MOVIMENTACAO){
      Movimentacao();
    } else if (tatic == RADAR) {
      Radar();
    } else if (tatic == RADAR_INVERSO) {
      RadarInverso();
 
  // Verify leds controll
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
}
