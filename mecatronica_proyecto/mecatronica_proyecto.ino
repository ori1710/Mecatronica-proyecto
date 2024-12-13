#include <Servo.h>

Servo servo_izq, servo_drc;
#define Trigger 5   //Pin digital 7 para el Trigger del sensor
#define Echo 4      //Pin digital 8 para el Echo del sensor
#define Motor_izq 2 //Pin PWM del motor izquierdo
#define Motor_drc 3 //Pin PWM del motor derecho
#define potPin A0   //Pin analógico para el potenciómetro

// Variables
long valor;              // Valor leído del potenciómetro
const int stopThreshold = 10; // Distancia mínima para detenerse (en cm)

void setup() {
  Serial.begin(9600);

  // Ultrasonic
  pinMode(Trigger, OUTPUT);
  pinMode(Echo, INPUT);

  // Motors
  servo_izq.attach(Motor_izq);
  servo_drc.attach(Motor_drc);
}

void loop() {
  // Leer distancia del sensor ultrasónico
  int distance = getUltrasonicDistance();

  // Ajustar la velocidad de los motores en función del potenciómetro
  valor = analogRead(potPin);
  int motorSpeed1 = map(valor, 0, 1000, 180, 90);  // Motor izquierdo (gira hacia adelante)
  int motorSpeed2 = map(valor, 0, 1000, 0, 90);    // Motor derecho (gira hacia adelante)

  servo_izq.write(motorSpeed1);
  servo_drc.write(motorSpeed2);
  // Si hay un obstáculo cerca
  if (distance > 0 && distance <= stopThreshold) {
    stopAndAvoidObstacle();
  }

  delay(50); // Pequeño retardo para estabilidad
}

int getUltrasonicDistance() {
  digitalWrite(Trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trigger, LOW);

  long duration = pulseIn(Echo, HIGH, 30000); // Usar timeout para evitar bloqueos
  if (duration == 0) {
    return -1; // No se detectó eco
  }

  int distance = duration * 0.034 / 2; // Convertir tiempo en distancia (cm)

  return distance;
}

void stopAndAvoidObstacle() {
  // Detener motores
  servo_izq.write(90); // Detener motor izquierdo
  servo_drc.write(90); // Detener motor derecho
  delay(500);

  // Echar hacia atrás
  servo_izq.write(0);  // Motor izquierdo hacia atrás
  servo_drc.write(180); // Motor derecho hacia atrás
  delay(500);

  // Girar
  servo_izq.write(0);  // Motor izquierdo hacia atrás
  servo_drc.write(0);  // Motor derecho hacia adelante
  delay(500);

  // Detener motores antes de continuar
  servo_izq.write(90); // Detener motor izquierdo
  servo_drc.write(90); // Detener motor derecho
  delay(500);
}
