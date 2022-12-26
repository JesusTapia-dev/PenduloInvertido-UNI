#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
MPU6050 sensor;
// Valores RAW (sin procesar) del acelerometro y giroscopio en los ejes x,y,z
int ax, ay, az;
int gx, gy, gz;
long tiempo_prev;
float dt;
//float ang_x, ang_y,ang_z;
//float ang_x_prev, ang_y_prev,ang_z_prev;
float girosc_ang_x, girosc_ang_y,girosc_ang_z;

float girosc_ang_x_prev, girosc_ang_y_prev,girosc_ang_z_prev;

void setup() {
  Serial.begin(57600);
  Wire.begin();
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

//Serial.print("Rotacion:");
Serial.println(girosc_ang_z);
  delay(100);

}
