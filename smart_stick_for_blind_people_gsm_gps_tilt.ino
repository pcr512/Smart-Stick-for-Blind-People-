// NIELIT LUCKNOW PIYUSH RASTOGI (SMART STICK FOR BLIND PEOPLE )
//GSM(SIM800L)+GPS(NEO6M)+TILT+ULTRASONIC+BUZZER+LED+JUMPERWIRE+ 1000uF 25 VOLT CAPACITOR+PUSH BUTTONS
#include <SoftwareSerial.h>
SoftwareSerial SIM800L(2, 3); //GSM MODULE SIM900L Piyush
#include <SoftwareSerial.h>
#include <TinyGPS++.h> // CONNECT THE RX OF NEO 6M GPS MODULE TO THE TX OF THE ARDUINO AND TX OF THE GPS MODULE THE THE RX OF THE ARDUINO WHICH ARE (0 AND 1)PIN OF THE ARDUIN0
TinyGPSPlus gps; //GPS MODULE- Piyush RASTOGI NELIT
double latitude, longitude;
String response;
int lastStringLength = response.length();

String link;
const int switchPin1 = 4; // Pin connected to the switch for sending live location
const int switchPin2 = 9; // Pin connected to switch for sending an alert message
const int switchPin3 = 10; // Pin connected to switch for making a call
const int trigPin = 6;//ULTRASONIC SENSOR
const int echoPin = 5;//ULTRASONIC SENSOR
const int buzzer = 11;//ULTRASONIC SENSOR
const int ledPin = 13;//ULTRASONIC SENSOR
const int sensorpin=12;//TILT SENSOR 

long duration;// defines variables
float distance;// defines variables
float safetyDistance;// defines variables
void setup() {

  Serial.begin(9600);
  Serial.println("GPS Piyush");

  SIM800L.begin(9600);
  SIM800L.println("AT+CMGF=1");
  Serial.println("SIM800L started at 9600");
  delay(100);
  Serial.println("Setup Complete! SIM800L is Ready!");
  SIM800L.println("AT+CNMI=2,2,0,0,0");

  pinMode(switchPin1, INPUT_PULLUP);
  pinMode(switchPin2, INPUT_PULLUP);
  pinMode(switchPin3, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(sensorpin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);   // Sets the LED pin as an output
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(buzzer, OUTPUT);// Sets the buzzer as an Output
  pinMode(ledPin, OUTPUT);// Sets the led Pin as Output
  Serial.begin(9600);

}

void loop() {
buttonHandler(); // Call the function to handle button presses

  if (SIM800L.available() > 0) {
    response = SIM800L.readStringUntil('\n');
  }

  if (lastStringLength != response.length()) {
    GPS();
  }
digitalWrite(trigPin, LOW); //ultrasonic sensor
delayMicroseconds(5);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
distance= duration*0.034/2;
safetyDistance = distance;
if (safetyDistance <= 70){
 digitalWrite(buzzer, HIGH);
 digitalWrite(ledPin, HIGH);
}
else{
 digitalWrite(buzzer, LOW);
 digitalWrite(ledPin, LOW);
 
 if (digitalRead(sensorpin) == HIGH) // tilt sensor
 {
 digitalWrite(buzzer, HIGH);
digitalWrite(ledPin, HIGH);
 }
 else
 {
  digitalWrite(buzzer, LOW);
  digitalWrite(ledPin, LOW);
 } 
}
buttonHandler(); // Call the function to handle button presses

  if (SIM800L.available() > 0) {
    response = SIM800L.readStringUntil('\n');
  }

  if (lastStringLength != response.length()) {
    GPS();
  }

}


void buttonHandler() {
  static int lastButtonState1 = HIGH;
  static int lastButtonState2 = HIGH;
  static int lastButtonState3 = HIGH;
  int buttonState1 = digitalRead(switchPin1);
  int buttonState2 = digitalRead(switchPin2);
  int buttonState3 = digitalRead(switchPin3);

  if (buttonState1 != lastButtonState1) {
    if (buttonState1 == LOW) {
      sendLocation();
    }
    delay(50);
  }
    

  if (buttonState2 != lastButtonState2) {
    if (buttonState2 == LOW) {
      SendMessage();
    }
    delay(50);
  }

  if (buttonState3 != lastButtonState3) {
    if (buttonState3 == LOW) {
      CallNumber();
    }
    delay(50);
  }

  lastButtonState1 = buttonState1;
  lastButtonState2 = buttonState2;
  lastButtonState3 = buttonState3;
}

void sendLocation() {
  GPS();
  SIM800L.println("AT+CMGF=1");
  delay(1000);
  SIM800L.println("AT+CMGS=\"+917007556483\"\r"); // Replace with the desired phone number
  delay(1000);
  SIM800L.println(link);
  delay(100);
  SIM800L.println((char)26);
  delay(1000);
}

void GPS() {
  if (Serial.available()) {
    gps.encode(Serial.read());
  }
  if (gps.location.isUpdated()) {
    latitude = gps.location.lat();
    longitude = gps.location.lng();
    link = "I AM IN TROUBLE! PLEASE HELP ME - MY LIVE LOCATION IS www.google.com/maps/place/" + String(latitude, 6) + "," + String(longitude, 6);
    Serial.println(link);
  }
}

void CallNumber() {
  SIM800L.println("ATD+917007XXXXXX;"); // Replace with the number to dial
  //delay(1000);
  //SIM800L.println("ATH"); // Hang up the call after 1 second (adjust as needed)
}

void SendMessage() {
  SIM800L.println("AT+CMGF=1");
  delay(1000);
  SIM800L.println("AT+CMGS=\"+917007XXXXXX\""); // Replace with the desired phone number
  delay(1000);
  SIM800L.println("I AM IN TROUBLE! PLEASE HELP ME"); // Replace with the alert message content
  SIM800L.println((char)26);
  delay(1000);
}