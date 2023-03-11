template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include <Servo.h>

Servo ESC; //Objeto para aumentar la velocidad de la rueda

MPU6050 sensor;

// Valores RAW (sin procesar) del acelerometro y giroscopio en los ejes x,y,z
int ax, ay, az;
int gx, gy, gz;
long tiempo_prev;
float dt;
float girosc_ang_z,girosc_ang_z_prev;

float pwmOut;
const int pinPWM=6;//pin que enviara la señal al driver 
float m=0.15,g=9.81;//consideramos una masa de 150g

//CONSTANTES PID
const double Kp = 0.25;  // Proportional gain
const double Ki = 0.001;  // Integral gain
const double Kd = 0.001; // Derivative gain

double Setpoint=21, Input, Output;
double lastInput, lastError, integral;


void setup() {
  Serial.begin(57600);
  ESC.attach(pinPWM, 1000, 1600);
  ESC.writeMicroseconds(1000);
  Wire.begin();           //Iniciando I2C  
  sensor.initialize(); 
  if (sensor.testConnection()) Serial.println("Sensor iniciado correctamente");
  else Serial.println("Error al iniciar el sensor");
  delay(5000);
  tiempo_prev=millis();
}

void loop() {
  // Leer las aceleraciones y velocidades angulares
  sensor.getAcceleration(&ax, &ay, &az);
  sensor.getRotation(&gx, &gy, &gz);
  dt = millis()-tiempo_prev;
  tiempo_prev=millis();

  girosc_ang_z = (gz/131)*dt/1000.0 + girosc_ang_z_prev;
  girosc_ang_z_prev = girosc_ang_z;
  Input = girosc_ang_z;

  //Trabajamos con la ley de control
  double error = Setpoint - Input;
  integral += error;
  double derivative = error - lastError;
  Output = (Kp * error) + (Ki * integral) + Kd * derivative;
  lastError = error;

  //if(Output > 1600) Output = 1600;
  //if(Output < 1000) Output = 1000;
  ESC.write(Output);
  delay(50);
  
  Serial << "Angulo: " << girosc_ang_z << " Control: " << Output <<  << " Error: " << error << '\n' ;
  
  return;
}
