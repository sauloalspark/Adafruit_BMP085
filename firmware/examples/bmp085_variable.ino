// This #include statement was automatically added by the Spark IDE.
#include "Adafruit_BMP085/Adafruit_BMP085.h"

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
char          BMP085Info[256];
unsigned long BMP085interval = 2000;
unsigned long BMP085lastTime;
//#ifdef BMP_SERIAL
unsigned long LEDinterval    =  500;
unsigned long LEDlastTime;
int           LEDstate       = LOW;
const int     LEDPin         = D7;
//#endif

void getBMP085Info(){

    if ( (millis()-BMP085lastTime) < BMP085interval ) {
        return;
    }
    
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
    
    sprintf(BMP085Info, "{\"temperature\": %.5f, \"pressure\": %.5f, \"altitude\": %.5f, \"real_altitude\": %.5f, \"time\": %d}", bmp.readTemperature(), bmp.readPressure()/100.0, bmp.readAltitude(), bmp.readAltitude(101500), millis());
    
    //Spark.publish("bmpo85info", szEventInfo);
    BMP085lastTime = millis();
}

// Initialize applicaiton
void InitializeApplication(){
#ifdef BMP_SERIAL
    Serial.begin(9600);
#endif

//#ifdef BMP_SERIAL
    pinMode(LEDPin, OUTPUT);
//#endif

  Spark.variable("BMP085", &BMP085Info, STRING);
}

// Blink LED and wait for some time
void BlinkLED() {
    unsigned long currentMillis = millis();
 
    if((currentMillis - LEDlastTime) > LEDinterval) {
        // save the last time you blinked the LED 
        LEDlastTime = currentMillis;   

        // if the LED is off turn it on and vice-versa:
        if (LEDstate == LOW) {
            LEDstate = HIGH;
        } else {
            LEDstate = LOW;
        }

        // set the LED with the ledState of the variable:
        digitalWrite(LEDPin, LEDstate);
  }
}

void setup() {
    InitializeApplication();
    
    InitializeBMP085();
}

void loop() {
//#ifdef BMP_SERIAL
  BlinkLED();
//#endif
  getBMP085Info();
}
