#include <Wire.h>
#include "MAX30105.h"
#include "SSD1306Wire.h"
#include "heartRate.h" // Include the heart rate algorithm header

MAX30105 particleSensor;

SSD1306Wire display(0x3c, SDA, SCL);

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred
long lastPrintTime = 0; //Time of the last print
long lastmoistTime = 0;
long lastPrintbeatavgTime =0;
int beatcount=0;
bool bpmstate =true;

float beatsPerMinute;
int beatAvg;
#define butt1 15
#define butt2 16
#define butt3 17
#define butt4 18
#define WIRE_PIN_1 12 // Pin for wire 1
#define WIRE_PIN_2 14 // Pin for wire 2
#define WIRE_PIN_3 27 // Pin for wire 3
#define START_BUTTON_PIN 32
#define STOP_BUTTON_PIN 33
#define buzz 13
float temperature=0;
bool started = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing...");

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0);
  particleSensor.enableDIETEMPRDY();

  // Initialize OLED display
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);

  pinMode(WIRE_PIN_1, INPUT_PULLUP);
  pinMode(WIRE_PIN_2, INPUT_PULLUP);
  pinMode(WIRE_PIN_3, INPUT_PULLUP);
  pinMode(butt1, INPUT_PULLUP);
  pinMode(butt2, INPUT_PULLUP);
  pinMode(butt3, INPUT_PULLUP);
  pinMode(butt4, INPUT_PULLUP);
  pinMode(START_BUTTON_PIN, INPUT_PULLUP);
  pinMode(STOP_BUTTON_PIN, INPUT_PULLUP);
  pinMode(25,OUTPUT);
  pinMode(26,OUTPUT);
  pinMode(13,OUTPUT);
  digitalWrite(26, HIGH);
  digitalWrite(25, HIGH);
  digitalWrite(buzz, HIGH);

}

long lastAreawireTime =0;
bool wirestate = true;
long deb1=0;
long deb2=0;
long deb3=0;
long deb4=0;
int debtime=500;
bool moiststate=true;
bool tempstate = true;

void loop() {
  HR();
  // Call printsd() function every second
  if (millis() - lastPrintTime >= 500) {
    printsd();
    lastPrintTime = millis();
  }
  if (millis() - lastmoistTime >= 5000) {
  Areawire();
  Bodytemp();
    lastAreawireTime = millis();
  }
  if (millis() - lastmoistTime >= 5000) {
  moist_res1();
  moist_res2();
  checkForDifference();
  lastmoistTime = millis();
  }
  checkforstart();
  if (millis() - deb1 >= debtime) {
    if (digitalRead(butt2) == LOW) {
    if(wirestate==true){
      wirestate=false;
      Serial.println("wire Disable warnig button pressed.");
    }
    else{
      wirestate=true;
      Serial.println("wire enable warnig button pressed.");
    }
  }
  deb1 = millis();
  }
  if (millis() - deb3 >= debtime) {
    if (digitalRead(butt3) == LOW) {
    if(tempstate==true){
      tempstate=false;
      Serial.println("Body Temp Disable warnig button pressed.");
    }
    else{
      tempstate=true;
      Serial.println("Body Temp enable warnig button pressed.");
    }
  }
  deb3 = millis();
  }
  if (millis() - deb4 >= debtime) {
    if (digitalRead(butt4) == LOW) {
    if(moiststate==true){
      moiststate=false;
      Serial.println("moisture Disable warnig button pressed.");
    }
    else{
      moiststate=true;
      Serial.println("mositure enable warnig button pressed.");
    }
  }
  deb4 = millis();
  }
  if (millis() - deb2 >= debtime) {
  if (digitalRead(butt1) == LOW) {
    if(bpmstate==true){
      bpmstate=false;
      Serial.println("bpm Disable warnig button pressed.");
    }
    else{
      bpmstate=true;
      Serial.println("bpm enable warnig button pressed.");
    }
  }
  deb2 = millis();
}
}

void HR() {
  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true) {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20) {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }
}



void printsd() {
  // Get IR value
  long irValue = particleSensor.getIR();
  
  // Print to Serial monitor
  Serial.print("IR=");
  Serial.print(irValue);
  Serial.print(", BPM=");
  Serial.print(beatsPerMinute);
  Serial.print(", Avg BPM=");
  Serial.println(beatAvg);
  
  if (beatAvg>160||beatAvg<40){
    Serial.println("UNSTABLE HEART BEAT");
     if (millis() - lastPrintbeatavgTime >= 1000) {
    beatcount++;
    lastPrintbeatavgTime = millis();
    if (beatcount==300){
    checkstate(bpmstate);
    }
  }
}
display.clear();
display.drawString(0, 0, "Avg BPM: " + String(beatAvg));
display.drawString(0, 20, "Body temp "+String(temperature));
display.drawString(0, 40, "MORE INFO");
display.display();
}

void Areawire() {
  bool wire1Connected = digitalRead(WIRE_PIN_1) == LOW;
  bool wire2Connected = digitalRead(WIRE_PIN_2) == LOW;
  bool wire3Connected = digitalRead(WIRE_PIN_3) == LOW;
  printResult(wire1Connected, "Wire 1");
  printResult(wire2Connected, "Wire 2");
  printResult(wire3Connected, "Wire 3");
  

}

void printResult(bool connected, const char* wireName) {
  if (connected) {
    Serial.print(wireName);
    Serial.println(" is connected!");
  } else {
    Serial.print(wireName);
    Serial.println(" is broken or not connected!");
    checkstate(wirestate);
}
}

void checkstate(bool bol){
  if(bol){
    started = true;
    checkforstart();
     Serial.println(" Activating Device");
  }
  else{
    Serial.println("Disabled");
  }
}

#define moisturePin1 35
int moistureValue1;
int moisturePercentage1;
void moist_res1() {
  moistureValue1 = analogRead(moisturePin1);
  int moisturePercentage1 = map(moistureValue1, 0, 4095, 2000, 0);
  Serial.print("Moisture Percentage: ");
  Serial.print(moisturePercentage1);
  Serial.println("%");
}
#define moisturePin2 34
int moistureValue2;
int moisturePercentage2;
void moist_res2() {
  moistureValue2 = analogRead(moisturePin2);
  int moisturePercentage2 = map(moistureValue2, 0, 4095, 2000, 0);
  Serial.print("Moisture Percentage: ");
  Serial.print(moisturePercentage2);
  Serial.println("%");
}


void Bodytemp()
{
  temperature = particleSensor.readTemperature();
  Serial.print("temperatureC=");
  Serial.print(temperature, 4);
  float temperatureF = particleSensor.readTemperatureF(); //Because I am a bad global citizen
  Serial.print(" temperatureF=");
  Serial.print(temperatureF, 4);
  Serial.println();

}

long lastbuzzTime=0;
long laststarterTime=0;

void checkforstart() {
  // Check if the start button is pressed
  if (digitalRead(START_BUTTON_PIN) == LOW) {
    started = true;
    Serial.println("Start button pressed. Loop started.");
  }

  digitalWrite(buzz, LOW);
  // Run the while loop if started
  while (started) {
    // Check if the stop button is pressed
    while(millis() - laststarterTime >= 150000) {
   digitalWrite(buzz,LOW);
    laststarterTime = millis();
  }
    if (digitalRead(STOP_BUTTON_PIN) == LOW) {
      started = false;
      Serial.println("Stop button pressed. Loop stopped.");
    }
     if (millis() - lastbuzzTime >= 30000) {
   digitalWrite(buzz,HIGH);
    lastbuzzTime = millis();
  }
  if (millis() - lastbuzzTime >= 150000) {
   digitalWrite(buzz,LOW);
    lastbuzzTime = millis();
  }

    // Your code to run while the loop is active goes here
    // For example, you can blink an LED
  digitalWrite(25, LOW);
  digitalWrite(26, LOW);
  digitalWrite(buzz,LOW);
  }
  digitalWrite(buzz, HIGH);
  digitalWrite(25, HIGH);
  digitalWrite(26, HIGH);
}

#define CHECK_INTERVAL 1000 // Check interval in milliseconds
//#define TIME_THRESHOLD 120000 // Time threshold for 2 minutes in milliseconds
//#define DIFFERENCE_THRESHOLD 0.20 // 20% difference threshold
int potentialMeter1Value;
int potentialMeter2Value;
unsigned long lastCheckTime1 = 0;
unsigned long lastCheckTime2 = 0;
//unsigned long lastCheckTime = 0;
float difference;
float difference1;
float difference2;
void checkForDifference() {
    // Read the values of potential meters
    potentialMeter1Value = analogRead(34);
    potentialMeter2Value = analogRead(35);
    // Calculate the difference between the potential meter values
   difference1 = abs(potentialMeter1Value) / 4095.0;
   difference2 = abs(potentialMeter2Value) / 4095.0;
    Serial.print("difference ");
    Serial.println(difference);
    difference = difference1-difference2;
    if (difference>=0.40){
    Serial.println(" difference exceed the threshold");
    started = true;
    checkforstart();
    }}
