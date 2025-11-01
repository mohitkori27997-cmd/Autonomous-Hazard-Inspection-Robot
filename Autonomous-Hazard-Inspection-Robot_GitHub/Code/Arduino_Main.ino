/*
 * Autonomous Hazard Inspection Robot - Arduino Main
 * Hardware: Arduino UNO R3, L298N, HC-SR04, 2x IR sensors, SG90 servo,
 *           MQ-135 gas sensor, MLX90614 temperature sensor
 * Peripherals: ESP-12F(ESP8266) via SoftwareSerial for cloud telemetry
 */

#include <DFRobot_MLX90614.h>
#include <SoftwareSerial.h>
#include <Adafruit_MLX90614.h>

SoftwareSerial espSerial(2, 3); // RX, TX

#define enA 10
#define in1 9
#define in2 8
#define in3 7
#define in4 6
#define enB 5

#define L_S A0
#define R_S A1

#define echo A2
#define trigger A3

#define servo A5

#define GAS_SENSOR_PIN A4
#define GAS_SENSOR_DIGITAL_PIN 4

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

int Set = 15;
int distance_L, distance_F, distance_R;
int sensorValue;
int digitalValue;

void servoPulse(int pin, int angle) {
  int pwm = (angle * 11) + 500;
  digitalWrite(pin, HIGH);
  delayMicroseconds(pwm);
  digitalWrite(pin, LOW);
  delay(50);
}

long Ultrasonic_read() {
  digitalWrite(trigger, LOW); delayMicroseconds(2);
  digitalWrite(trigger, HIGH); delayMicroseconds(10);
  long time = pulseIn(echo, HIGH);
  return time / 29 / 2;
}

void readAndProcessGasSensor() {
  sensorValue = analogRead(GAS_SENSOR_PIN);
  digitalValue = digitalRead(GAS_SENSOR_DIGITAL_PIN);

  if      (sensorValue >= 10   && sensorValue <= 300)  Serial.println("NH3");
  else if (sensorValue >  300  && sensorValue <= 1000) Serial.println("Benzene");
  else if (sensorValue >  1000 && sensorValue <= 2000) Serial.println("CO2");
  else   Serial.println("Unknown or out of range");

  digitalWrite(13, sensorValue > 400 ? HIGH : LOW);

  Serial.println(sensorValue, DEC);
  Serial.println(digitalValue, DEC);
  delay(1000);
}

void forword() {
  digitalWrite(in1, LOW);  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
}

void backword() {
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);  digitalWrite(in4, HIGH);
}

void turnRight() {
  digitalWrite(in1, LOW);  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);  digitalWrite(in4, HIGH);
}

void turnLeft() {
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
}

void Stop() {
  digitalWrite(in1, LOW);  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);  digitalWrite(in4, LOW);
}

void compareDistance() {
  if (distance_L > distance_R) {
    turnLeft();  delay(500);
    forword();   delay(600);
    turnRight(); delay(500);
    forword();   delay(600);
    turnRight(); delay(400);
  } else {
    turnRight(); delay(500);
    forword();   delay(600);
    turnLeft();  delay(500);
    forword();   delay(600);
    turnLeft();  delay(400);
  }
}

void Check_side() {
  Stop(); delay(100);

  for (int angle = 70; angle <= 140; angle += 5) servoPulse(servo, angle);
  delay(300);
  distance_R = Ultrasonic_read(); Serial.print("D R="); Serial.println(distance_R);
  delay(100);

  for (int angle = 140; angle >= 0; angle -= 5) servoPulse(servo, angle);
  delay(500);
  distance_L = Ultrasonic_read(); Serial.print("D L="); Serial.println(distance_L);
  delay(100);

  for (int angle = 0; angle <= 70; angle += 5) servoPulse(servo, angle);
  delay(300);

  compareDistance();
}

void setup() {
  espSerial.begin(9600);
  Serial.begin(9600);

  pinMode(R_S, INPUT); pinMode(L_S, INPUT);
  pinMode(echo, INPUT); pinMode(trigger, OUTPUT);

  pinMode(enA, OUTPUT); pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT); pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT); pinMode(enB, OUTPUT);

  analogWrite(enA, 200); analogWrite(enB, 200);

  pinMode(13, OUTPUT);
  pinMode(GAS_SENSOR_DIGITAL_PIN, INPUT);
  pinMode(servo, OUTPUT);

  for (int angle = 70; angle <= 140; angle += 5) servoPulse(servo, angle);
  for (int angle = 140; angle >= 0; angle -= 5)  servoPulse(servo, angle);
  for (int angle = 0; angle <= 70; angle += 5)   servoPulse(servo, angle);

  distance_F = Ultrasonic_read(); delay(500);

  if (!mlx.begin()) {
    Serial.println("Error connecting to MLX sensor. Check wiring.");
    while (1);
  }
}

void loop() {
  readAndProcessGasSensor();

  distance_F = Ultrasonic_read();
  Serial.print("D F="); Serial.println(distance_F);

  if ((digitalRead(R_S) == 0) && (digitalRead(L_S) == 0)) {
    if (distance_F > Set) forword();
    else { Stop(); Check_side(); }
  }
  else if ((digitalRead(R_S) == 1) && (digitalRead(L_S) == 0)) turnRight();
  else if ((digitalRead(R_S) == 0) && (digitalRead(L_S) == 1)) turnLeft();
  else if ((digitalRead(R_S) == 1) && (digitalRead(L_S) == 1)) {
    Stop(); delay(10);
    backword(); delay(10);
    Check_side();
    forword();
  }

  float ambC = mlx.readAmbientTempC();
  float objC = mlx.readObjectTempC();

  Serial.print("AmbientC="); Serial.print(ambC);
  Serial.print("  ObjectC="); Serial.println(objC);

  String dataToSend = String(distance_F) + "," + String(sensorValue) + "," + String(ambC) + "," + String(objC);
  espSerial.println(dataToSend);
  delay(500);
}
