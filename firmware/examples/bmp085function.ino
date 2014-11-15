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
char BMP085Info[128];
    
// Initialize BMP085

void InitializeBMP085(String command){
	if (!bmp.begin()) {
	    RGB.control(true);
	    RGB.color(255, 255, 255);
#ifdef BMP_SERIAL
		Serial.println("Could not find a valid BMP085 sensor, check wiring!");
#endif
		while (1) {}
	}
	RGB.control(false);
}

// Publish Pressure, Altitude
int getBMP085Info(){
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
    
    sprintf(BMP085Info, "{\"temperature\": %.5f, \"pressure\": %.5f, \"altitude\": %.5f, \"real_altitude\": %.5f}", bmp.readTemperature(), bmp.readPressure()/100.0, bmp.readAltitude(), bmp.readAltitude(101500));
    
    //Spark.publish("bmpo85info", szEventInfo);
    return 0;
}

// Initialize applicaiton
void InitializeApplication(){
#ifdef BMP_SERIAL
    Serial.begin(9600);
#endif

//#ifdef BMP_SERIAL
    pinMode(D7, OUTPUT);
//#endif

  Spark.variable( "BMP085"   , &BMP085Info   , STRING );
  Spark.function( "getBMP085", &getBMP085Info         );
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
//#endif
}
