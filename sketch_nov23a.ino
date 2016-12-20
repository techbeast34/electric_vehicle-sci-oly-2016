#include <PID_v1.h>
#include <RedBot.h>
#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *rightWheel = AFMS.getMotor(4);
Adafruit_DCMotor *leftWheel = AFMS.getMotor(3);

RedBotEncoder encoder = RedBotEncoder(A4, 10);

double numCounts = 0;
double goal = 2;//goal distance in m
double goalCounts = goal*880/(2*1.25);
double power = 100;

boolean go = false;

int buttonPin = 9;//replace w/actual button pin
PID encoderPID = PID(&numCounts, &power, &goalCounts, 1, 0.9, 0.9, DIRECT);

void setup() {
  Serial.begin(9600);
  AFMS.begin();
  rightWheel->run(FORWARD);
  leftWheel->run(FORWARD);
  
  encoderPID.SetMode(AUTOMATIC);
 // encoderPID.SetOutputLimits(0, 255);
  //encoderPID.SetSampleTime(300);
  pinMode(buttonPin, INPUT_PULLUP);
  
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
   encoder.clearEnc(BOTH);
   
  Serial.println(F("Finished setup"));
  
}


void loop() {
  //Serial.println(encoder.getTicks(LEFT));
  //Serial.println(digitalRead(buttonPin));
  go = true;
  if (digitalRead(buttonPin) == LOW)
    go = true;
  if (!go){
    //Serial.println(F("Waiting for Button"));
    //Serial.println(encoder.getTicks(LEFT));
    return;
  }
  driveDistance(numCounts);
  rightWheel->setSpeed(0);
  leftWheel->setSpeed(0);
  numCounts = encoder.getTicks(LEFT);
  delay(10);
  Serial.println("-");
}

void driveDistance(int numCounts)
{   
   //encoder.clearEnc(BOTH);
   if(encoder.getTicks(LEFT) < goalCounts)
   {
     encoderPID.Compute();
     Serial.print(F("Traveling at: "));
     Serial.println(power);
     rightWheel->run(FORWARD);
     leftWheel->run(FORWARD);
     rightWheel->setSpeed(power);
     leftWheel->setSpeed(power);
     Serial.println(numCounts);
     delay(200);
   }  
   else{
    rightWheel->setSpeed(0);
    leftWheel->setSpeed(0);
    Serial.println(F("Finished"));
   }

   
}

