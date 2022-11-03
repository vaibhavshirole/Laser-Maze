// The below are constants, which cannot be changed

#define LED_PIN           22  // ESP32 pin GPIO22 connected to LED

#define LASER_PIN        33  // ESP32 pin GPIO33 connected to first LASER diode
#define ANALOG_THRESHOLD  4000

#define BUTTON            25  //GPIO25

#define NUM_LIGHT_SENSORS      6
int light_sensors[NUM_LIGHT_SENSORS] = {36, 39, 34, 35, 32, 33};   //GPIO 32-39

#define LIGHT_SENSOR_1         36                  //GPIO36
#define LIGHT_SENSOR_2         39                  //GPIO39
#define LIGHT_SENSOR_3         34                  //GPIO34
#define LIGHT_SENSOR_4         35                  //GPIO35
#define LIGHT_SENSOR_5         32                  //GPIO32
#define LIGHT_SENSOR_6         33                  //GPIO33


  const unsigned long eventInterval = 2000;   //2 second period where lasers are off before turning back on
  unsigned long previousTime = 0;             //part of laser reset functionality, set this time when lasers turn off
  unsigned long currentTime;
void setup() {
  Serial.begin(115200);                           //start the serial monitor
  pinMode(LED_PIN, OUTPUT);                       //set led gpio to output mode
  pinMode(LASER_PIN, OUTPUT);                    //set laser gpio to output mode
  digitalWrite(LASER_PIN, LOW);              //start the laser on

  pinMode(BUTTON, INPUT);

  delay(100);
  Serial.println("Setup done");

}

void loop() {

    for(int i=0; i<NUM_LIGHT_SENSORS; i++){
      int analogValue = analogRead(light_sensors[i]);
      Serial.println((String) "#: " + i + (String) " val: " + analogValue);
      delay(100);
    }
    delay(2000);

//  String msg;
//  int laserRecover = 0;
//  int buttonState = 1;
//  int analogValue = analogRead(light_sensors[1]);
//  if (analogValue >= 4000) {
//    msg += "h";
//  } else {
//    msg += "b";
//  }
//
//  if (msg.indexOf("b") == -1) {
//    laserRecover = 0;
//  } else {
//    Serial.printf("Beam broke, going into recovery\n");
//    laserRecover = 1;
//  }
//
//  if (buttonState == 0) {
//    //
//  }
//  else {
//    if (laserRecover == 1) {
//      Serial.printf("Laser system disabled, beam broken\n");
//      for (int i = 0; i < 10; i++) {
//        digitalWrite(LASER_PIN, HIGH);                   //lasers off
//      }
//      currentTime = millis();
//      if (currentTime - previousTime >= eventInterval) {
//        Serial.printf("Laser system trying to re-enable, setting recovery to 2\n");
//        previousTime = currentTime;
//        laserRecover = 2;
//      }
//
//    } else if (laserRecover == 2) {
//      Serial.printf("Help laser recover\n");
//
//      currentTime = millis();
//      if (currentTime - previousTime >= eventInterval) {
//        Serial.printf("Laser system trying to re-enable\n");
//        for (int i = 0; i < 10; i++) {
//          digitalWrite(LASER_PIN, LOW);   //lasers on
//        }
//        previousTime = currentTime;
//        laserRecover = 0;
//      }
//
//    } else if (laserRecover == 0) {
//      Serial.printf("Laser system running normally\n");
//      for (int i = 0; i < 10; i++) {
//        digitalWrite(LASER_PIN, LOW);   //lasers on
//      }
//    }
//  }
//
//  delay(100);




  //    delay(1000);
  //    digitalWrite(LASER_PIN, LOW);
  //    Serial.println("set laser on");
  //    delay(1000);
  //    digitalWrite(LASER_PIN, HIGH);
  //    Serial.println("set laser off");

}





/*  #include <Arduino.h>

int counter = 0;

int lasers[15] = {32,33,25,26,27,14,13,23,22,21,19,18,5,17,16};

void setup() {
  // put your setup code here, to run once:
  for(int i = 0; i<15; i++){

    pinMode(lasers[i], OUTPUT);

  }

}

void loop() {
  // put your main code here, to run repeatedly:
    for(int i = 0; i<15; i++){

    if(counter%lasers[i]==0){

      digitalWrite(lasers[i], LOW);

    } else {

      digitalWrite(lasers[i], HIGH);


    }
    }
    delay(70);
    counter++;


  }
 */