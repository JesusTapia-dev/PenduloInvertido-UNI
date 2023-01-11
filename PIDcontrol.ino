#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include <Servo.h>

Servo ESC;//Objeto para aumentar la velocidad de la rueda

//$$$$$$$$$$$$$$$$$$$ MPU 6050 $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
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
const double Kp = 0.5;  // Proportional gain
const double Ki = 0.1;  // Integral gain
const double Kd = 0.01; // Derivative gain

double Setpoint=21, Input, Output;
double lastInput, lastError, integral;


void setup() {
  Serial.begin(57600);
  ESC.attach(pinPWM, 1000, 1600);
  ESC.write(1000);
  Wire.begin();           //Iniciando I2C  
  sensor.initialize(); 
  if (sensor.testConnection()) Serial.println("Sensor iniciado correctamente");
  else Serial.println("Error al iniciar el sensor");
  tiempo_prev=millis();
}

void loop() {
  // Leer las aceleraciones y velocidades angulares
  sensor.getAcceleration(&ax, &ay, &az);
  sensor.getRotation(&gx, &gy, &gz);
  dt = millis()-tiempo_prev;
  tiempo_prev=millis();
  girosc_ang_z=(gz/131)*dt/1000.0 + girosc_ang_z_prev;
  girosc_ang_z_prev=girosc_ang_z;
  Input=girosc_ang_z;
  //Trabajamos con la ley de control
  double error = Setpoint - Input;
  integral += error*dt;
  double derivative = (error - lastError)/dt;
  Output = (Kp * error) + (Ki * integral) + (Kd * derivative);
  lastError = error;

  unsigned long startTime=millis();
  if(Input<30){
      while(millis() - startTime < 3000){ESC.write(0);}
      ESC.write(Output); 
    }
   if(Input>30){
    while(millis() - startTime < 3000){ESC.write(Output);}
    ESC.write(0);
    }
  delay(50);
}
