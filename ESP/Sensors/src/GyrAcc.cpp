#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_SPIDevice.h>

#include <Wire.h>

Adafruit_MPU6050 mpu;
Adafruit_BMP085 bmp;

int taster = 2;
int buttonstate = 0;
int ini_pressure = 0;
int ini_altitude = 0;

float altiude_delta = 0;


void setup(){
    Serial.begin(9600);

    pinMode(taster, INPUT);

    if (!mpu.begin()) {
    Serial.println("Sensor init failed");
    while (1)
      yield();
    }

    if (!bmp.begin()) {
	  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	  while (1) {}
    }

    for (int i = 0; i < 10; i++){
      ini_pressure += bmp.readPressure();
    }

    ini_pressure /= 10;

    ini_altitude = bmp.readAltitude();

}

void loop(){
  
  buttonstate = digitalRead(taster);
  if (buttonstate != 1) {
    Serial.println("System turned off... \n\n");
    while (buttonstate != 1) {
      buttonstate = digitalRead(taster);
    }
    Serial.println("System turned on... \n\n");
    ini_pressure = 0;
    for (int i = 0; i < 10; i++){
      ini_pressure += bmp.readPressure();
    }
    ini_pressure /= 10;

    ini_altitude = bmp.readAltitude();
  }

  for (int i = 0; i < 10; i++){
    altiude_delta += bmp.readAltitude(ini_pressure);
  }

  altiude_delta /= 10;
    
  Serial.print("Altitude delta: "); 
  Serial.print(altiude_delta);
  Serial.print("Meter \n"); 

  Serial.print("Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");
  
  Serial.print("Pressure = ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");   

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  Serial.print(a.acceleration.x, 1);
  Serial.print(" ");
  Serial.print(a.acceleration.y, 1);
  Serial.print(" ");
  Serial.println(a.acceleration.z, 1);

  Serial.print(" ");
  Serial.print(g.gyro.x, 1);
  Serial.print(" ");
  Serial.print(g.gyro.y, 1);
  Serial.print(" ");
  Serial.println(g.gyro.z, 1);
  
  Serial.println();
  delay(1000);
}