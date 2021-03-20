/* 

*/

#include <Servo.h>
#define MIN 5
#define MAX 60
#define WIGGLE 10

// twelve servo objects can be created on most boards
Servo filterOnServo;  // create servo object to control a servo
Servo filterOffServo;
Servo robotOnServo;
Servo robotOffServo;
//Servo roofOnServo;
//Servo roofOffServo;

long second1 = 1000;
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

/*
  Serial.println("------");
  lightLevel=analogRead(A0); //(photoRPin);
  Serial.println(minLight);
  Serial.println(lightLevel);
  Serial.println(maxLight);
*/

  filterOnServo.write(MIN);
  filterOffServo.write(MIN);
  robotOnServo.write(MIN);
  robotOffServo.write(MIN);
  
  delay(second5);
  
  press_cleaner_OFF();
  delay(second5);
  press_filter_OFF();

  delay(second5);
  
  Serial.println("Initialization Done");
  
}

void loop() {

/*
  press_filter_OFF();
  delay(second5);
  press_filter_ON();
  delay(second5);
  press_cleaner_OFF();
  delay(second5);
  press_cleaner_ON();
  delay(second5);
  */


  // =====================
  // DETERMINE DAY OR NIGHT
  // =====================

  lightLevel=analogRead(A0); //(photoRPin);
 
  if (lightLevel < 300){
    daytime=true;
    Serial.println("Day");
  } else if (lightLevel > 500) {
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
    press_cleaner_OFF(); // Make sure cleaner is OFF first
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
    //                        delay(minute1);
    press_cleaner_ON();
    delay(minute30);
    delay(minute30);
    //                       delay(minute1);
    press_cleaner_OFF();
  }
  else if (filterOn==false){
    // End of day...reset robot
    robotStarted=false;
  }
  // ===============================


  // 30 minute delay
  delay(minute30);
  
}

void press_filter_ON() {
  Serial.println("Filter ON");
  filterOnServo.write(MAX);
  delay(second1);
  filterOnServo.write(MAX+WIGGLE);
  delay(second1);
  filterOnServo.write(MAX-WIGGLE-WIGGLE);
  delay(second1);
  filterOnServo.write(MIN);
  digitalWrite(filterOnLed, HIGH);
}

void press_filter_OFF() {
  Serial.println("Filter OFF");
  filterOffServo.write(MAX);
  delay(second1);
  filterOffServo.write(MAX+WIGGLE);
  delay(second1);
  filterOffServo.write(MAX-WIGGLE-WIGGLE); 
  delay(second1);
  filterOffServo.write(MIN);
  digitalWrite(filterOnLed, LOW);
}

void press_cleaner_ON() {
  Serial.println("Cleaner ON");
  robotOnServo.write(MAX);
  delay(second1);
  robotOnServo.write(MAX+WIGGLE);
  delay(second1);  
  robotOnServo.write(MAX-WIGGLE-WIGGLE);
  delay(second1);
  robotOnServo.write(MIN);
  digitalWrite(robotOnLed, HIGH);
}

void press_cleaner_OFF() {
  Serial.println("Cleaner OFF");
  robotOffServo.write(MAX);
  delay(second1); 
  robotOffServo.write(MAX+WIGGLE);
  delay(second1); 
  robotOffServo.write(MAX-WIGGLE-WIGGLE);
  delay(second1);
  robotOffServo.write(MIN);
  digitalWrite(robotOnLed, LOW);
}
