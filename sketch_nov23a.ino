//including necessary libraries
#include <PID_v1.h>
#include <RedBot.h>
#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();//init motorshield
Adafruit_DCMotor *rightWheel = AFMS.getMotor(4);//init wheels
Adafruit_DCMotor *leftWheel = AFMS.getMotor(3);

RedBotEncoder encoder = RedBotEncoder(A4, 10);//initialize encoder 
                              //(takes in 2 pins one for left and one for right)
//declaring and initializing variables
double numCounts = 0;
double goal = 2;//goal distance in m
double goalCounts = goal*880/(2*1.25);//880 was the number of encoder counts we saw by manually turning the wheel 1 meter
//I don't recall why it's divided by 2 and multiplied by 1.25, but this will be changed when we test distance

double power = 100;

boolean go = false;

int buttonPin = 9;//pin that reads in button data

PID encoderPID = PID(&numCounts, &power, &goalCounts, 1, 0.9, 0.9, DIRECT);//initializing PID
//&input, &output, &setpoint, k, p, d, direction

void setup() {
  Serial.begin(9600);
  
  AFMS.begin();
  rightWheel->run(FORWARD);
  leftWheel->run(FORWARD);
  
  encoderPID.SetMode(AUTOMATIC);
  pinMode(buttonPin, INPUT_PULLUP);//sets up button activation
  
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);//power for encoders

  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);//ground for encoders
  
  encoder.clearEnc(BOTH);//clears encoder counts
   
  Serial.println(F("Finished setup"));
  
}


void loop() {
  //Serial.println(digitalRead(buttonPin)); not using button code right now
  go = true;
  if (digitalRead(buttonPin) == LOW)
    go = true;
  if (!go){
    //Serial.println(F("Waiting for Button"));
    //Serial.println(encoder.getTicks(LEFT));
    return;
  }
  driveDistance(numCounts);//calls drive distance
  
  rightWheel->setSpeed(0);
  leftWheel->setSpeed(0);
  
  numCounts = encoder.getTicks(LEFT);//update numCounts
  delay(10);//wait 10 milliseconds
  Serial.println("-");
}

void driveDistance(int numCounts)
{   
   //encoder.clearEnc(BOTH);
   if(encoder.getTicks(LEFT) < goalCounts)//if the car hasn't driven the right distance yet
   {
     encoderPID.Compute();//compute power
     Serial.print(F("Traveling at: "));
     Serial.println(power);
     
     rightWheel->run(FORWARD);
     leftWheel->run(FORWARD);
     
     rightWheel->setSpeed(power);
     leftWheel->setSpeed(power);
     Serial.println(numCounts);
     delay(200);
   }  
   else{//once it has driven the right distance, stop
    rightWheel->setSpeed(0);
    leftWheel->setSpeed(0);
    Serial.println(F("Finished"));
   }

   
}

