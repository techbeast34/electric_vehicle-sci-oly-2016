#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *rightWheel = AFMS.getMotor(1);
Adafruit_DCMotor *leftWheel = AFMS.getMotor(2);

void setup() {
  AFMS.begin();
  rightWheel->setSpeed(255);
  leftWheel->setSpeed(255);
}

void loop() {
  rightWheel->run(FORWARD);
  leftWheel->run(FORWARD);

}
