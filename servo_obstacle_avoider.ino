#include <Arduino.h>
#include <ESP32Servo.h>

#define TRIG_PIN D2
#define ECHO_PIN D3
#define SERVO_PIN D5
#define LED_PIN D6
#define BUZZER_PIN D7

Servo servo;

float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2; // convert to cm
  return distance;
}

void beepBuzzer(int times) {
  for (int i = 0; i < times; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    delay(100);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  servo.attach(SERVO_PIN);
  servo.write(0);
}

void loop() {
  // Sweep from 0° to 180°
  for (int angle = 0; angle <= 180; angle += 5) {
    servo.write(angle);
    float distance = getDistance();
    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print("°  Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    if (distance < 15 && distance > 0) {
      digitalWrite(LED_PIN, HIGH);
      beepBuzzer(1); // short beep for detection
    } else {
      digitalWrite(LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
    }

    delay(150);
  }

  // Sweep back from 180° to 0°
  for (int angle = 180; angle >= 0; angle -= 5) {
    servo.write(angle);
    float distance = getDistance();

    if (distance < 15 && distance > 0) {
      digitalWrite(LED_PIN, HIGH);
      beepBuzzer(1);
    } else {
      digitalWrite(LED_PIN, LOW);
      digitalWrite(BUZZER_PIN, LOW);
    }

    delay(150);
  }
}
