/*********************************************************************
This is an example sketch for our Monochrome Nokia 5110 LCD Displays

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/338

These displays use SPI to communicate, 4 or 5 pins are required to
interface

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <math.h>
#include <DHT.h> //
#include <DHT_U.h> //


#define DHTTYPE DHT11
#define DHTPIN A1 //should be a digital pin
#define LIGHT_PIN A0
#define LASER_PIN 2
//heartrate sensor
#define samp_siz 4
#define rise_threshold 5

// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3); //INDICATES DISPLAY PIN'S ASSIGNMENTS

// Hardware SPI (faster, but must use certain hardware pins):
// SCK is LCD serial clock (SCLK) - this is pin 13 on Arduino Uno
// MOSI is LCD DIN - this is pin 11 on an Arduino Uno
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
// Adafruit_PCD8544 display = Adafruit_PCD8544(5, 4, 3);
// Note with hardware SPI MISO and SS pins aren't used but will still be read
// and written to during SPI transfer.  Be careful sharing these pins!

//initial startup
void setup()   {
  Serial.begin(9600);
  display.begin();
  pinMode(LASER_PIN, OUTPUT);
  // init done
  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(50); //manipulatable
	display.clearDisplay(); //precautionary line

  //display startup logo
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(0,0);
  display.println("Welcome to the Swiss Army Glove");
  display.display(); //?????? causes flower symbol instead of welcome when removed
  delay(2000); //TO BE REDUCED LATER TO MAKE TRANSITION CLEANER
  display.clearDisplay();
}

// Pulse Monitor Test Script
		int sensorPin = 0;
		double alpha = 0.75;
		int period = 100;
		double change = 0.0;
		double minval = 0.0;

//temperature sensor function
DHT dht(DHTPIN, DHTTYPE);
//sensor averaging and timing
float avg = 0; 
long count = 0;
long t=0;
//MODE SELECTION
int mode = 0;
bool mode_trigger = 0;
//heart rate sensor
int heartsensorPin = 0;


//main operations
void loop() {

	long delta_t = millis() - t;  //millis reads milliseconds since program began, holds 50 days of value
	//calculations and initilization etc
	double data = 0;
//MODE SELECTION INPUT CONTROL
	if(mode == 0) //light sensor PIN A0
		data = analogRead(A0); //analog input from pin A0 (light sensor)
	else if(mode == 1) //K PIN A1
		data = dht.readTemperature()+ 273.15; //analog input from pin A1 (temperature sensor)
	else if(mode == 2) //C PIN A1
		data = (dht.readTemperature()) ;
	else if(mode == 3)//F PIN A1
		data = (dht.readTemperature() * 9.0)/ 5.0 + 32.0; //CtoF conversion
	else if(mode == 4) //heartrate PIN A2
		data = analogRead(A2); //analog input from pin A3 (heartrate sensor)
	else if(mode == 5) //sound PIN A3
		data = analogRead(A3);
	else if(mode == 6) //laser PIN 2
		data = 0;
	
	count++;
	//analog setup
	int analog_X = analogRead(A5); 
	//int analog_Y = analogRead(//some pin); //MIGHT BE UNUSED
	//int analog_SW = analogRead(//some pin); //MIGHT BE UNUSED
	if(analog_X == 0)
		Serial.println("Hit 0");



//heartrate test variables
	float reads[samp_siz], sum;
	long int now, ptr;
	float last, reader, start;
	float first, second, third, before, print_value;
	bool rising;
	int rise_count;
	int n;
	long int last_beat;
	for(int i = 0; i < samp_siz; i++)
		reads[i] = 0;
	sum = 0;
	ptr = 0;


	//default startup state = light sensor
	if(mode == 0){
		digitalWrite(LASER_PIN, LOW);
		avg = avg*(count-1)/count + data/count;
		if(delta_t >= 1000){
			// text display lines
			if(avg == 0) //error case
				display.println("Light: N/A");
			else
	 	 		display.println("Light:  " + String(avg)); //current display
	  		display.display();
	  		//delay(100);
	  		display.clearDisplay();
			//calculation etc
			avg = 0;
			count = 0;
			t = millis();
		}
		
		else if(delta_t <= 0)
			t = millis();
	}

		
	// temperature K
	else if(mode == 1){
		digitalWrite(LASER_PIN, LOW);
		avg = avg*(count-1)/count + data/count;
		if(delta_t >= 1000){
			// text display lines
			if(avg == 0) //error case
				display.println("TempK: N/A");
	 	 	else
	 	 		display.println("TempK:  " + String(avg)); //current display
	  		display.display();
	  		//delay(100);
	  		display.clearDisplay();
			//calculation etc
			avg = 0;
			count = 0;
			t = millis();
		}
		
		else if(delta_t <= 0)
			t = millis();
	}

		
	// temperature C
	else if (mode == 2){
		digitalWrite(LASER_PIN, LOW);
		avg = avg*(count-1)/count + data/count;
		if(delta_t >= 1000){
			// text display lines
			if(avg == 0) //error case
				display.println("TempC: N/A");	 	 	
	 	 	else
	 	 		display.println("TempC:  " + String(avg)); //current display
	  		display.display();
	  		//delay(100);
	  		display.clearDisplay();
			//calculation etc
			avg = 0;
			count = 0;
			t = millis();
		}
		
		else if(delta_t <= 0)
			t = millis();	
	}

		
	//temperature F
	else if (mode == 3){
		//TEMP = 2;
		digitalWrite(LASER_PIN, LOW);
		//data = Temp(); //to be used for temperature sensor
		avg = avg*(count-1)/count + data/count;
		if(delta_t >= 1000){
			// text display lines
			if(avg == 0)
				display.println("TempF: N/A"); //error case	 	 	
	 	 	else
	 	 		display.println("TempF:  " + String(avg)); //current display
	  		display.display();
	  		//delay(100);
	  		display.clearDisplay();
			//calculation etc
			avg = 0;
			count = 0;
			t = millis();
		}
		
		else if(delta_t <= 0)
			t = millis();	
	}

		
	//heartrate NEEDS FIXING!
	else if (mode == 4){ //heartrate code uses while(1)


		digitalWrite(LASER_PIN, LOW);
		static double oldValue = 0;
		static double oldChange = 0;
		int rawValue = analogRead(A2);
		double value = alpha * oldValue + (1 - alpha) * rawValue;
		Serial.print (rawValue);
		oldValue = value;
		count++;
		data = value;
		avg = avg*(count-1)/count + data/count;
		if(delta_t >= 1000){
			// text display lines
	 	 	display.println ("Heart  " + String(value));
	  		display.display();
	  		display.clearDisplay();
			//calculation etc
			avg = 0;
			count = 0;
			t = millis();
		}
		
		else if(delta_t <= 0)
			t = millis();	

/* potentially fixable heartrate code
		n = 0;
		start = millis();
		reader = 0.;
		do
		{
			reader += analogRead(A2); //heartrate sensor pin
			n++;
			now = millis();
		}
		while(now < start + 20);
			reader /=n;
			sum -=reads[ptr];
			sum += reader;
			reads[ptr] = reader;
			last = sum/samp_siz;

		if(last > before){
			rise_count++;
			if(!rising && rise_count > rise_threshold){
				rising = true;
				first = millis() - last_beat;
				last_beat = millis();
				print_value = 60000. / (0.4 * first + 0.3 * second + 0.3 * third);
				Serial.print(print_value);
				Serial.print('\n');
				display.println ("Heart  " + String(print_value));
	  			display.display();
	  			display.clearDisplay();
				third = second;
				second = first;
			}	
		}
		else{
			rising = false;
			rise_count = 0;
		}
		before = last;
		ptr++;
		ptr %= samp_siz;
*/
		}



	//Sound NEEDS A SENSITIVITY INCREASE (different data types like floats perhaphs)
	else if(mode == 5){
		digitalWrite(LASER_PIN, LOW);
		avg = avg*(count-1)/count + data/count;

	  	if(delta_t >= 1000){
			// text display lines
	 	 	display.println ("Sound  " + String(avg));
	  		display.display();
	  		//delay(100);
	  		display.clearDisplay();
			//calculation etc
			avg = 0;
			count = 0;
			t = millis();
		}
		
		else if(delta_t <= 0)
			t = millis();	
		}

	else if (mode == 6){

			digitalWrite(LASER_PIN, HIGH);
	 	 	display.println("Laser"); //current display
	  		display.display();
	  		display.clearDisplay();
	}


	//ANY FURTHER SENSORS TO BE ADDED HERE UNDER ANOTHER MODE CONDITION STATEMENT
		
	display.setTextSize(2);
	display.setTextColor(BLACK);
	display.setCursor(0,0);
	
	//analog trigger
	if(analog_X > 300 && mode_trigger == 0){ //if statements possibly combinable
			mode++;
			mode_trigger = 1;			
			if(mode >= 7)
				mode = 0;
	}
	if(analog_X < 20)
		mode_trigger = 0;

		
}
	




