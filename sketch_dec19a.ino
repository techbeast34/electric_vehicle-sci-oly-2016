#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *rightWheel = AFMS.getMotor(4);
Adafruit_DCMotor *leftWheel = AFMS.getMotor(3);

void setup() {
  Serial.begin(9600);
  AFMS.begin();
  rightWheel->run(FORWARD);
  leftWheel->run(FORWARD);
}


void loop() {
  rightWheel->setSpeed(100);
  leftWheel->setSpeed(100);
}

