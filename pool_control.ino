/* 

*/

#include <Servo.h>

// twelve servo objects can be created on most boards
Servo filterOnServo;  // create servo object to control a servo
Servo filterOffServo;
Servo robotOnServo;
Servo robotOffServo;
//Servo roofOnServo;
//Servo roofOffServo;

long second5 = 5000;
long minute1 = (second5 * 12); // 60,000
long minute5 = (minute1 * 5);  // 300,000
long minute10 = (minute1 * 10); // 600,000
long minute30 = (minute1 * 30); // 1,800,000

int pos = 0;    // variable to store the servo position

int filterOnLed = 3;
int robotOnLed  = 4;

int minLight;          //Used to calibrate the readings
int maxLight;          //Used to calibrate the readings
int lightLevel;
int adjustedLightLevel;
bool daytime;
bool summertime;
bool filterOn;
bool pool_warmer_than_air;
bool robotStarted;

void setup() {
  Serial.begin(9600);

  Serial.println("Initialization Starting...Please wait");
  filterOnServo.attach(9);  // attaches the servo on pin 9 to the servo object
  filterOffServo.attach(10);
  robotOnServo.attach(11);
  robotOffServo.attach(12);
  //roofOnServo.attach(?);
  //roofOffServo.attach(?);

  pinMode(filterOnLed, OUTPUT);
  pinMode(robotOnLed, OUTPUT);
    
  daytime    = false;
  summertime = true;
  filterOn   = false;
  pool_warmer_than_air = false;
  robotStarted = false;

  minLight = 500;
  maxLight = 400;


  //Serial.println("1min test start");
  //delay(minute1);
  //Serial.println("1min test end");

  filterOnServo.write(0);
  filterOffServo.write(0);
  robotOnServo.write(0);
  robotOffServo.write(0);
  
  press_cleaner_OFF();
  press_filter_OFF();
  
  Serial.println("Initialization Done");
}

void loop() {
  // =====================
  // DETERMINE DAY OR NIGHT
  // =====================
   //auto-adjust the minimum and maximum limits in real time
  lightLevel=analogRead(A0); //(photoRPin);
  if(lightLevel<minLight){
    minLight=lightLevel;
  }
  if(lightLevel>maxLight){
    maxLight=lightLevel;
  }
    
  //Adjust the light level to produce a result between 0 and 100.
  adjustedLightLevel = map(lightLevel, minLight, maxLight, 0, 100);

/*
  Serial.println("------");
  Serial.println(minLight);
  Serial.println(lightLevel);
  Serial.println(maxLight);
*/
  Serial.println(adjustedLightLevel);
 
  if (adjustedLightLevel < 50){
    daytime=true;
    Serial.println("Day");
  } else{
    daytime=false;
    Serial.println("Night");
  }
  // =====================

  // ===============================
  // DETERMINE SUMMER OR WINTER MODE
  // ===============================
    summertime=true;
  // =============================== 

  // ===============================
  // DETERMINE POOL WARMER THAN AIR
  // ===============================
    pool_warmer_than_air=false;
  // =============================== 


  // ===============================
  // PRESS BUTTONS ON REMOTE
  // Depends on day/night and summer/winter and pool/air temperature
  // On a summer day, turn ON
  // On a summer night, if pool temp is warmer than air then turn ON
  // If freezing, pump will turn on automatically
  // ===============================
  if (summertime==true && daytime==true && filterOn==false){
    // Summer Day
    // Press Filter ON
    // Rely on pool controller to decide to send to roof or not
    filterOn=true;
    press_filter_ON();
  }

//  else if (summertime==true && daytime==false && pool_warmer_than_air==true && filterOn==false){
//    // Cool Summer Night
//    // Press Filter ON
//    // Press AUX to send to roof
//    filterOn=true;
//    press_filter_ON();
//    //press_aux_ON();
//  }

  else if (daytime==false && filterOn==true){
    // Night
    // Press Filter OFF
    // Turn AUX OFF
    filterOn=false;
    press_filter_OFF();
  }
  // ===============================


  // ===============================
  // TURN ON ROBOT ONCE PER DAY
  // ===============================
  if (filterOn==true && robotStarted==false){
    robotStarted=true;
    // start robot after 5 minutes
    Serial.println("Waiting 5min to start robot");
    delay(minute5);
    press_cleaner_ON();
    // run robot for 30 minutes
    delay(minute30);
    press_cleaner_OFF();
  }
  else if (filterOn==false){
    // End of day...reset robot
    robotStarted=false;
  }
  // ===============================


  // 10 minute delay
  delay(minute1);
}

void press_filter_ON() {
  Serial.println("Filter ON");
  for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 90 degrees
    // in steps of 1 degree
    filterOnServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  delay(second5);
  for (pos = 90; pos >= 0; pos -= 1) { // goes from 90 degrees to 0 degrees
    filterOnServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  digitalWrite(filterOnLed, HIGH);
}

void press_filter_OFF() {
  Serial.println("Filter OFF");
  for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    filterOffServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  delay(second5);
  for (pos = 90; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    filterOffServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  digitalWrite(filterOnLed, LOW);
}

void press_cleaner_ON() {
  Serial.println("Cleaner ON");
  for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 90 degrees
    // in steps of 1 degree
    robotOnServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  delay(second5);
  for (pos = 90; pos >= 0; pos -= 1) { // goes from 90 degrees to 0 degrees
    robotOnServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  digitalWrite(robotOnLed, HIGH);
}

void press_cleaner_OFF() {
  Serial.println("Cleaner OFF");
  for (pos = 0; pos <= 90; pos += 1) { // goes from 0 degrees to 90 degrees
    // in steps of 1 degree
    robotOffServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  delay(second5);
  for (pos = 90; pos >= 0; pos -= 1) { // goes from 90 degrees to 0 degrees
    robotOffServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  digitalWrite(robotOnLed, LOW);
}