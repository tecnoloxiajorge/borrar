#include <Servo.h>
Servo motor;
int sensor;

void setup() {
  pinMode(2,INPUT);//entrada sensor infrarrojos
  motor.attach(9);//pin del servomotor
}

void loop() {
 sensor=digitalRead(2);
 if (sensor==0) {motor.write(180);}
 else {motor.write(0);}
 delay(200);
}
