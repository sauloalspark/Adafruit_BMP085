// This #include statement was automatically added by the Spark IDE.
#include "Adafruit_BMP085_noserial/Adafruit_BMP085_noserial.h"

//#define BMP_SERIAL
#define BMP_LED
//#define BMP_FUNCTION
//#define BMP_PUBLISH
#define BMP_VARIABLE

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

unsigned long publishTTL     = 21600;

// Publish Pressure, Altitude
char          BMP085info[256];
char          BMP085infoS[62];
//unsigned long BMP085interval = 30000;
unsigned long BMP085interval = 5000;
unsigned long BMP085altDiff  = 101500;
unsigned long BMP085lastTime;

#ifdef BMP_LED
unsigned long LEDinterval    =  500;
unsigned long LEDlastTime;
int           LEDstate       = LOW;
const int     LEDPin         = D7;
#endif


int tinkerDigitalRead( String pin     );
int tinkerDigitalWrite(String command );
int tinkerAnalogRead(  String pin     );
int tinkerAnalogWrite( String command );


void publishBMP085info(){
    if ( (millis()-BMP085lastTime) < BMP085interval ) {
        return;
    }
    
    //Spark.publish("neoUpdate"   , "publish update", publishTTL, PRIVATE);
    
    getBMP085info("");
    
    //Spark.publish("neoUpdate"   , "publish updated", publishTTL, PRIVATE);
    
    Spark.publish("BMP085infoEv", BMP085infoS, publishTTL, PRIVATE);
    //Spark.publish("BMP085infoEv", "pubishing actual results", publishTTL, PRIVATE);
    
    BMP085lastTime = millis();
}

void updateBMP085info(){
    if ( (millis()-BMP085lastTime) < BMP085interval ) {
        return;
    }
    
    //Spark.publish("neoUpdate", "function", publishTTL, PRIVATE);
    
    getBMP085info("");
    
    BMP085lastTime = millis();
}

int getBMP085info(String command){
    Spark.publish("neoUpdate", "updating", publishTTL, PRIVATE);
    
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
    Serial.print(bmp.readAltitude(BMP085altDiff));
    Serial.println(" meters");
#endif
    
    sprintf(BMP085info , "{\"temperature\": %.5f, \"pressure\": %.5f, \"altitude\": %.5f, \"real_altitude\": %.5f, \"time\": %lu}", bmp.readTemperature(), bmp.readPressure()/100.0, bmp.readAltitude(), bmp.readAltitude(BMP085altDiff), millis());
    sprintf(BMP085infoS, "{\"t\":%.5f,\"p\":%.5f,\"a\":%.5f,\"ra\":%.5f}", bmp.readTemperature(), bmp.readPressure()/100.0, bmp.readAltitude(), bmp.readAltitude(BMP085altDiff));

    Spark.publish("neoUpdate", "updated" , publishTTL, PRIVATE);

    return 1;
}

// Initialize applicaiton
void InitializeApplication(){
#ifdef BMP_SERIAL
    Serial.begin(9600);
#endif

#ifdef BMP_LED
    pinMode(LEDPin, OUTPUT);
#endif

#ifdef BMP_VARIABLE
    Spark.variable( "BMP085"   , &BMP085info   , STRING );
#endif

#ifdef BMP_FUNCTION
    Spark.function( "getBMP085",  getBMP085info         );
#endif

    Spark.function( "digitalread" , tinkerDigitalRead  );
    Spark.function( "digitalwrite", tinkerDigitalWrite );
    Spark.function( "analogread"  , tinkerAnalogRead   );
    Spark.function( "analogwrite" , tinkerAnalogWrite  );
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

int tinkerDigitalRead(String pin) {
    int pinNumber = pin.charAt(1) - '0';
    if (pinNumber< 0 || pinNumber >7) return -1;
    if(pin.startsWith("D")) {
        pinMode(pinNumber, INPUT_PULLDOWN);
        return digitalRead(pinNumber);}
    else if (pin.startsWith("A")){
        pinMode(pinNumber+10, INPUT_PULLDOWN);
        return digitalRead(pinNumber+10);}
    return -2;}

int tinkerDigitalWrite(String command){
    bool value = 0;
    int pinNumber = command.charAt(1) - '0';
    if (pinNumber< 0 || pinNumber >7) return -1;
    if(command.substring(3,7) == "HIGH") value = 1;
    else if(command.substring(3,6) == "LOW") value = 0;
    else return -2;
    if(command.startsWith("D")){
        pinMode(pinNumber, OUTPUT);
        digitalWrite(pinNumber, value);
        return 1;}
    else if(command.startsWith("A")){
        pinMode(pinNumber+10, OUTPUT);
        digitalWrite(pinNumber+10, value);
        return 1;}
    else return -3;}

int tinkerAnalogRead(String pin){
    int pinNumber = pin.charAt(1) - '0';
    if (pinNumber< 0 || pinNumber >7) return -1;
    if(pin.startsWith("D")){
        pinMode(pinNumber, INPUT);
        return analogRead(pinNumber);}
    else if (pin.startsWith("A")){
        pinMode(pinNumber+10, INPUT);
        return analogRead(pinNumber+10);}
    return -2;}

int tinkerAnalogWrite(String command){
    int pinNumber = command.charAt(1) - '0';
    if (pinNumber< 0 || pinNumber >7) return -1;
    String value = command.substring(3);
    if(command.startsWith("D")){
        pinMode(pinNumber, OUTPUT);
        analogWrite(pinNumber, value.toInt());
        return 1;}
    else if(command.startsWith("A")){
        pinMode(pinNumber+10, OUTPUT);
        analogWrite(pinNumber+10, value.toInt());
        return 1;}
    else return -2;}

void setup() {
    InitializeApplication();
    
    InitializeBMP085();
}

void loop() {
#ifdef BMP_LED
  BlinkLED();
#endif



//#ifdef BMP_PUBLISH
  publishBMP085info();
//#else

//#ifdef BMP_VARIABLE
//  updateBMP085info();
//#endif

//#endif
}
