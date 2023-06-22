#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
MPU6050 sensor;
// Valores RAW (sin procesar) del acelerometro y giroscopio en los ejes x,y,z
int ax, ay, az;
int gx, gy, gz;
//variables de tiempo
long tiempo_prev;
float dt;

float ang_x, ang_y,ang_z;
float ang_x_prev, ang_y_prev,ang_z_prev;
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
  
  dt = (millis()-tiempo_prev)/1000;
  tiempo_prev=millis();
  
  //Calcular los ángulos con acelerometro
  float accel_ang_x=atan(ay/sqrt(pow(ax,2) + pow(az,2)))*(180.0/3.14);
  float accel_ang_y=atan(-ax/sqrt(pow(ay,2) + pow(az,2)))*(180.0/3.14);
  
  //Calcular angulo de rotación con giroscopio y filtro complemento  
  ang_x = 0.98*(ang_x_prev+(gx/131)*dt) + 0.02*accel_ang_x;
  ang_y = 0.98*(ang_y_prev+(gy/131)*dt) + 0.02*accel_ang_y;
  
  
  ang_x_prev=ang_x;
  ang_y_prev=ang_y;

  //Mostrar los angulos separadas por un [tab]
// Recordar que los angulos de rotación son considerando la posicion de calibracion 
  Serial.print("Rotacion en X:  ");
  Serial.print(ang_x); 
  Serial.print("tRotacion en Y: ");
  Serial.println(ang_y);

  delay(10);
}
