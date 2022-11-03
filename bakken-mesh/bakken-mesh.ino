#include "painlessMesh.h"

#define   MESH_PREFIX     "username"
#define   MESH_PASSWORD   "password"
#define   MESH_PORT       5555

/* Set the type of device to program */
#define DEVICE            1                 //1-laser, 2-photoresistor, 3-button, 4-testing

/* Laser settings */
#if(DEVICE==1 || DEVICE==4)
  #define NUM_LASERS      15
  int lasers[NUM_LASERS] = {32,33,25,26,27,14,13,23,22,21,19,18,5,17,16};
  
  #define LASER_1           32                       //GPIO32
  #define LASER_2           33
  #define LASER_3           25
  #define LASER_4           26
  #define LASER_5           27
  #define LASER_6           14
  #define LASER_7           13
  
  #define LASER_8           23
  #define LASER_9           22
  #define LASER_10          21
  #define LASER_11          19
  #define LASER_12          18
  #define LASER_13          5
  #define LASER_14          17
  #define LASER_15          16
  
  int buttonState =              1;                  //laser node knows the button as 1 initially
  int laserState =               1;                  //laser node knows the beams as healthy initally
  int laserRecover =             0;                  //0- off track, 1- on track, track being photoresistor: "healthy" and button: "on" 
  int whichNode;
  
  const unsigned long eventInterval = 3000;          //3 second period where lasers are off before turning back on
  unsigned long previousTime =   0;                  //part of laser reset functionality, set this time when lasers turn off
  unsigned long currentTime;
#endif

/* Photoresistor settings */
#if(DEVICE == 2 /*|| DEVICE == 4*/)
  #define NUM_LIGHT_SENSORS      2
  int light_sensors[NUM_LIGHT_SENSORS] = {36, 34};   //GPIO 32-39
  
  #define LIGHT_SENSOR_1         36                  //GPIO36
  #define LIGHT_SENSOR_2         39                  //GPIO39
  #define LIGHT_SENSOR_2         34                  //GPIO34
  #define LIGHT_SENSOR_4         35                  //GPIO35
  #define LIGHT_SENSOR_5         32                  //GPIO32
  #define LIGHT_SENSOR_6         33                  //GPIO33
  
  #define ANALOG_THRESHOLD    3700                   //adjust how might brightness needed for healthy beam
#endif

/* Button settings */
#if(DEVICE == 3)
  #define BUTTON            25                       //GPIO25
  #define BUTTON_LIGHT      26                       //GPIO26
  int buttonPushCounter =   0;                       //counter for the number of button presses
  int buttonVal =           0;                       //current state of the button, 0 means on here. Its confusing
  int lastButtonVal =       0;                       //previous state of the button, 0 means on here. Its confusing
#endif

/* Mesh scheduling and prototypes*/
void sendMessage() ;                                 //sendMessage() function proto., this func. sends node status messages
painlessMesh  mesh;                                  //mesh object
Scheduler userScheduler;                             //called in setup() to control the mesh task
Task taskSendMessage( TASK_SECOND * 0.1 , TASK_FOREVER, &sendMessage );

/*
 * Sends node status messages
 * 
 * Laser node:            Status message, unimportant
 * Photoresistor node:    "healthy" or "broken" depending on if laser is hitting the photoresistor 
 * Button node:           "on" or "off" button state, handled like a toggle
 */
void sendMessage() {
  
  /* Laser node */
  #if(DEVICE == 1)
    String msg = "Hi from laser node";
    mesh.sendBroadcast( msg );
  #endif
  
  /* Photoresistor  node */
  #if(DEVICE == 2)
    String statusPacket = "";
    for(int i=0; i<NUM_LIGHT_SENSORS; i++){
      int analogValue = analogRead(light_sensors[i]); 
      
      if (analogValue > ANALOG_THRESHOLD) {          //if laser is on the photoresistor
        statusPacket+="h";                           //insert bit h-healthy
      } else {
        statusPacket+="b";                           //else, insert bit b-broken 
      }
    }
    mesh.sendBroadcast(statusPacket);                //send status of the photoresistors

  #endif

  
  /* Button node */
  #if(DEVICE == 3)
    buttonVal = digitalRead(BUTTON);                 //check if the pushbutton is pressed. If it is, the buttonVal is HIGH

    if (buttonVal != lastButtonVal) {                //if the state has changed, increment the counter
      if (buttonVal == HIGH) {                       //if the current state is HIGH then the button went from off to on
        buttonPushCounter++;
      } else {                                       //if the current state is LOW then the button went from on to off
        //
      }
      delay(20);                                     //debouncing
    }
    lastButtonVal = buttonVal;                       //save the current state as the last state, for next time through the loop

    if (buttonPushCounter % 2 == 0) {                //check if state changed
      String msg = "on";
      mesh.sendBroadcast(msg);                       //send "on"
      digitalWrite(BUTTON_LIGHT, LOW);               //button light on
    } else {
      String msg = "off";
      mesh.sendBroadcast(msg);                       //send "off"
      digitalWrite(BUTTON_LIGHT, HIGH);              //button light off
    }
  #endif

}

// Needed for painless library
// Laser node: Turns on or off lasers depending on status string
// Photo resistor node: Replies with generic data received printout

/*
 * Needed for the painless mesh library, gets status messages from other nodes and takes action
 * 
 * Laser node:            Get message from photoresistor and button nodes.
 *                        If "healthy"->laserState = 1, and if "on"->buttonState = 1, turn on lasers.
 *                        If lasetState and buttonState are both not 1, then turn off lasers
 *                        and wait 2 seconds before starting re-enable pulse. Laser will sort of 
 *                        pulse until it finds the photoresistor and the button is set to on. 
 * Photoresistor node:    Standard received message, unimportant 
 * Button node:           Standard received message, unimportant 
 */
void receivedCallback( uint32_t from, String &msg ) {
  /* Laser node */
  #if(DEVICE == 1)

    Serial.println((String) "laser recover val:" + laserRecover);
    
    if(laserRecover == 0){

      /* Check with button node */
      if (msg.compareTo("off") == 0) {
        buttonState = 0;                                //turn off lasers, Laser system disabled (button state)
      } else if (msg.compareTo("on") == 0) {
        buttonState = 1;                                //turn on lasers if not already on, Laser system on and armed (button state)
      } 
      
      else {
        /* Check with photoresistor node */
        if(msg.indexOf("b") == -1){                     //search for b in status packet, if -1 then its not there, and healthy signal
          laserRecover = 0;
        }else{
          Serial.printf("Beam broke, going to recovery\n");
          laserRecover = 1;    //beam broke, go into recovery
          whichNode = from;
        }     
      }
    }

    if(buttonState == 0){
      Serial.printf("Laser system disabled, button off\n");
      for(int i = 0; i<NUM_LASERS; i++){
        digitalWrite(lasers[i], HIGH);                  //lasers off
      }
      laserRecover = 0;
    }
    else{
           
      if( (laserRecover == 1)){  
        Serial.printf("Laser system disabled, beam broken\n");
        for(int i = 0; i<NUM_LASERS; i++){
          digitalWrite(lasers[i], HIGH);                //lasers off
        }

        currentTime = millis();
        
        if(previousTime == 0){
          previousTime = millis();                      //if the intitial previous time of 0
        }
                
        if(currentTime - previousTime >= eventInterval){
          Serial.printf("Laser system trying to re-enable, setting recovery to 2\n");
          previousTime = currentTime; 
          laserRecover = 2;          
        } 
         
      }else if( (laserRecover == 2)){
        Serial.printf("Help laser recover\n");
        for(int i=0; i<NUM_LASERS; i++){
          digitalWrite(lasers[i], LOW);                 //lasers on            
        }

        currentTime = millis();
        if(currentTime - previousTime >= eventInterval){
          Serial.printf("Laser system trying to re-enable\n");
          previousTime = currentTime;
          laserRecover = 0;          
        }  
              
      }else if(laserRecover == 0){
        Serial.printf("Laser system running normally\n");
        for(int i=0; i<NUM_LASERS; i++){
            digitalWrite(lasers[i], LOW);               //lasers on            
        }
        previousTime = 0;                               //reset to initial previous time of 0 
      }
    }
  #endif

  /* Photoresistor node */
  #if (DEVICE == 2)
    Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
  #endif
  
  /* Button node */
  #if (DEVICE == 3)
    Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
  #endif
  
  #if(DEVICE == 4)
    for(int i = 0; i<NUM_LIGHT_SENSORS; i++){
      int reading = analogRead(light_sensors[i]);
      Serial.println((String) "photoresistor" + i + (String)" val" + reading);
    }

    if (msg.compareTo("off") == 0) {
      for(int i = 0; i<NUM_LASERS; i++){
        digitalWrite(lasers[i], HIGH);
        Serial.println((String) "laser" + i + (String)"set");
      }
    } else if (msg.compareTo("on") == 0) {
      for(int i = 0; i<NUM_LASERS; i++){
        digitalWrite(lasers[i], LOW);
        Serial.println((String) "laser" + i + (String)"set");
      }
    } 
    
  #endif
}

void newConnectionCallback(uint32_t nodeId) {
  Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
  Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(), offset);
}

void setup() {
  Serial.begin(115200);                             //serial baud rate

  /* Laser node */
  #if (DEVICE == 1 /*|| DEVICE == 4*/)
    for(int i = 0; i<NUM_LASERS; i++){
      pinMode(lasers[i], OUTPUT);                   //set laser to output
      digitalWrite(lasers[i], LOW);                 //turn laser on to start
    }
  #endif

  /* Photoresistor node */
  #if (DEVICE == 3)
    pinMode(BUTTON, INPUT);                         //set button to input
    pinMode(BUTTON_LIGHT, OUTPUT);                  //set button light to output
  #endif


  /* All types: mesh.setDebugMsgTypes(
     ERROR | MESH_STATUS | CONNECTION | SYNC | 
     COMMUNICATION | GENERAL | MSG_TYPES | REMOTE); */
  mesh.setDebugMsgTypes( ERROR | STARTUP );     //set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask(taskSendMessage);
  taskSendMessage.enable();
}

void loop() {
  mesh.update();          //it will run the user scheduler as well
}
