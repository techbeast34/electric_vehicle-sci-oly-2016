//including necessary libraries
#include <PID_v1.h>
#include <RedBot.h>
#include <Adafruit_L3GD20.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_MotorShield.h>

Adafruit_L3GD20 gyro;
Adafruit_MotorShield AFMS = Adafruit_MotorShield();//init motorshield
Adafruit_DCMotor *rightWheel = AFMS.getMotor(2);//init wheels
Adafruit_DCMotor *leftWheel = AFMS.getMotor(1);

RedBotEncoder encoder = RedBotEncoder(A0, A1);//initialize encoder 
                              //(takes in 2 pins one for left and one for right)
//declaring and initializing variables
double numCounts = 0;
double goal = 1;//goal distance in m
double goalCounts = 5000;
double kGyro = 8;//8 normal
double power = 100;
double dir = 0;
unsigned long lastTime;
double kP = 1.2;
double kI = 1.2;
double kD = 1;
double avg =0;//0.00 for 9m,0.5 for 9.5m,  
boolean go = false;

double sum = 0;
double count = 0;

int buttonPin = 5;//pin that reads in button data

PID encoderPID = PID(&numCounts, &power, &goalCounts, kP, kI, kD, DIRECT);//initializing PID
//&input, &output, &setpoint, k, p, d, direction

void setup() {
  AFMS.begin();
  Serial.begin(9600);
  
  encoderPID.SetMode(AUTOMATIC);
  pinMode(buttonPin, INPUT);//sets up button activation
  
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
  
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
  
  encoder.clearEnc(BOTH);//clears encoder counts
  
  Serial.println("pins set up");
   while(!gyro.begin())//if gyro initialization unsuccessful
  {
    Serial.println("Oops ... unable to initialize the L3GD20. Check your wiring!");
  }
 
  Serial.println("gyro set up");
  lastTime = millis();
 }

void loop() {
  Serial.println(digitalRead(buttonPin));
  if (digitalRead(buttonPin) == 1)
  {
    go = true;
  }
  if (!go){
    return;
  }
  driveDistance();
  
  /*rightWheel->setSpeed(0);
  leftWheel->setSpeed(0);
  */
  //delay(10);//wait 10 milliseconds
  Serial.println("-");
}

void driveDistance()
{   
   //encoder.clearEnc(BOTH);
    numCounts = encoder.getTicks(LEFT);//update numCounts
    Serial.println(numCounts);
   if(numCounts < goalCounts)//if the car hasn't driven the right distance yet
   {
    encoderPID.Compute();//compute power
    Serial.print(F("Traveling at: "));
    Serial.println(power);
    gyro.read();//bleh i forgot to do this XD so i wasn't getting any data from the gyro XD
    unsigned long currentTime = millis();
    unsigned long timePassed = currentTime - lastTime;
    lastTime = currentTime;
    dir += (gyro.data.z-avg)*timePassed/1000;//gyro.data.z is an angular speed in seconds
    Serial.println("dir: " + String(dir));
    driveArcade(power, dir);
    //delay(10);
   }  
   else{//once it has driven the right distance, stop
    rightWheel->setSpeed(0);
    leftWheel->setSpeed(0);
    Serial.println(F("Finished"));
   }  
}

void drive(int left, int right){
  left = (left > 255 ? 255 : left);
  right = (right > 255 ? 255 : right);
  if (left < 0){
    leftWheel->run(FORWARD);
    leftWheel->setSpeed(-1*left);
  }
  else{
    leftWheel->run(BACKWARD);
    leftWheel->setSpeed(left);
  }
 if (right < 0){
    rightWheel->run(FORWARD);
    rightWheel->setSpeed(-1*right);
  }
  else{
    rightWheel->run(BACKWARD);
    rightWheel->setSpeed(right);
    
  }
}

void driveArcade(double mag, double dir){
  dir = dir/90*kGyro;
  mag = mag/255.0;
  double left = (mag + dir)*128;
  double right = (mag - dir)*128;
  Serial.println("left: " + String(left));
  Serial.println("right: " + String(right));
  drive(left, right);
}


