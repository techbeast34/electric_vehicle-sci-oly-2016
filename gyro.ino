//including neccessary libraries
#include <Adafruit_L3GD20.h>
#include <Adafruit_L3GD20_U.h>
#include <Adafruit_MotorShield.h>

//initialize gyro, motorshield, and wheels
Adafruit_L3GD20 gyro;
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *rightWheel = AFMS.getMotor(4);//refers to M4 on the motorshield
Adafruit_DCMotor *leftWheel = AFMS.getMotor(3);//refers to M3 on the motorshield

//initial values
float mag; //power that goes to the motor
           //(will be calculated by the PID)
float dir; //gyro angle
float xDir, yDir; //calculated power for left and 
                  //right wheels, might need to be reversed later
int count;//counter for loop to make the car stop after set time

void setup() {
  // put your setup code here, to run once:
  AFMS.begin();
  Serial.begin(9600);
  
  rightWheel->run(FORWARD);
  leftWheel->run(FORWARD);
  
  leftWheel->setSpeed(255);
  rightWheel->setSpeed(255);

  pinMode(11, OUTPUT);//ground for gyro
  digitalWrite(11, LOW);
  
  pinMode(10, OUTPUT);//power for gyro
  digitalWrite(10, HIGH);
  
  if (!gyro.begin())//if gyro initialization unsuccessful
  {
    Serial.println("Oops ... unable to initialize the L3GD20. Check your wiring!");
    while (1);//stop forever
  }
  count = 0;//initializes loop counter to 0
  }

void loop() {
  //this loop will run forever
  if(count <=2)//adds a conditional so loop can run
  {
    // put your main code here, to run repeatedly:
    int startTime = millis();
    gyro.read();
    int timePassed = millis() - startTime;
    mag = 255;//change to PID value once integrated w/distance code
    dir = gyro.data.z*timePassed/1000;//gyro.data.z is an angular speed in seconds
    Serial.println(dir);
  
    xDir = mag*cos(dir);//arcade drive
    Serial.println("xDir: " + String(xDir));
    yDir = mag*sin(dir);//arcade drive

    //scale y values so they are within 0 to 255
    if(yDir>=0) yDir=255-yDir;
    else yDir+=255;
    Serial.println("yDir: " + String(yDir));
    
   if(yDir>0)
    {
      rightWheel->run(FORWARD);
      rightWheel->setSpeed(yDir);
    }
    else
    {
      rightWheel->run(BACKWARD);//if motor speed negative, reverse direction
      rightWheel->setSpeed(abs(yDir));
    }
    
    if(xDir>=0)
    {
      leftWheel->run(FORWARD);
      leftWheel->setSpeed(xDir);
      
    }
    else
    {
      leftWheel->run(BACKWARD);
      leftWheel->setSpeed(abs(xDir));
    }
    
    delay(2000);//wait 2 seconds, allowing car enough time to correct orientation
    count++;//increment count
    }
    else//once the car has gone through the loop 3 times, stop
    {
      rightWheel->setSpeed(0);
      leftWheel->setSpeed(0);
      return 1;
    }
}

