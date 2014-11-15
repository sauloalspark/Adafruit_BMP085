// This #include statement was automatically added by the Spark IDE.
#include "Adafruit_BMP085_noserial/Adafruit_BMP085_noserial.h"

//#define BMP_SERIAL

/*
	Wiring
	------
	BMP085 Vcc to 3.3V
	BMP085 GND to GND
	BMP085 SCL to D1
	BMP085 SDA to D0
*/

Adafruit_BMP085 bmp;

// Initialize BMP085

void InitializeBMP085(){
	if (!bmp.begin()) {
		Serial.println("Could not find a valid BMP085 sensor, check wiring!");
		while (1) {}
	}
}

// Publish Pressure, Altitude
char BMP085Info[128];

void getBMP085Info(){
#ifdef BMP_SERIAL
    Serial.print("Temperature = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");
    
    Serial.print("Pressure = ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");
    
    // Calculate altitude assuming 'standard' barometric
    // pressure of 1013.25 millibar = 101325 Pascal
    Serial.print("Altitude = ");
    Serial.print(bmp.readAltitude());
    Serial.println(" meters");

  // you can get a more precise measurement of altitude
  // if you know the current sea level pressure which will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
    Serial.print("Real altitude = ");
    Serial.print(bmp.readAltitude(101500));
    Serial.println(" meters");
#endif
    
    sprintf(BMP085Info, "Temperature=%.2f °C, Pressure=%.2f hPa Altitude %.2f meters, Real Altitude %.2f meters", bmp.readTemperature(), bmp.readPressure()/100.0, bmp.readAltitude(), bmp.readAltitude(101500));
    
    //Spark.publish("bmpo85info", szEventInfo);
}

// Initialize applicaiton
void InitializeApplication(){
#ifdef BMP_SERIAL
    Serial.begin(9600);
#endif

#ifdef BMP_SERIAL
    pinMode(D7, OUTPUT);
#endif

  Spark.variable("BMP085", &BMP085Info, STRING);
}

// Blink LED and wait for some time
void BlinkLED(){
    digitalWrite(D7, HIGH);   
    delay(500);
    digitalWrite(D7, LOW);   
    delay(500);
}

void setup() {
    InitializeApplication();
    
    InitializeBMP085();
}

void loop() {
//#ifdef BMP_SERIAL
  BlinkLED();
  getBMP085Info();
//#endif
}
