#include <SoftwareSerial.h>

//SIM900 TX is connected to Arduino D5
#define SIM800_TX_PIN 11

//SIM900 RX is connected to Arduino D4
#define SIM800_RX_PIN 12 //7

//Create software serial object to communicate with SIM800
SoftwareSerial serialSIM800(SIM800_TX_PIN, SIM800_RX_PIN);

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
int buzzerPin = 3;
// These constants won't change. They're used to give names to the pins used:
const int analogInPin = A3;  // Analog input pin that the potentiometer is attached to
int sensorValue = 0;        // value read from the pot
int sent_Stat = 0;
int red_Led = 10;
int green_Led = 9;
void setup() {
  while (!Serial);
  pinMode(red_Led, OUTPUT);
  pinMode(green_Led, OUTPUT);

  pinMode(3, OUTPUT);
  tone(3, 1000, 2000);

  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  lcd.clear();
  lcd.setCursor (5, 0);
  lcd.print("System");
  lcd.setCursor (1, 1);
  lcd.print("Initializing.!");
  delay(3000);
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);
  // map it to the range of the analog out:
  sensorValue = map(sensorValue, 0, 1023, 0, 100);


  // print the results to the Serial Monitor:
  Serial.print("sensor = ");
  Serial.println(sensorValue);


  if (sensorValue > 60) {
    digitalWrite(red_Led, HIGH);
    digitalWrite(green_Led, LOW);
    lcd.clear();
    lcd.setCursor (0, 0);
    lcd.print("Hazard Detected!");
    tone(3, 250); // A4
    delay(1500);
    lcd.clear();
    tone(3, 1000); // B4
    delay(1500);
    lcd.clear();
    lcd.setCursor (0, 0);
    lcd.print("Hazard Detected!");
    tone(3, 250); // B4
    delay(1500);
    lcd.clear();
    tone(3, 1000); // B4
    delay(1500);
    lcd.clear();
    lcd.setCursor (0, 0);
    lcd.print("Hazard Detected!");
    tone(3, 250); // B4
    delay(1500);
    lcd.clear();
    tone(3, 1000); // B4
    delay(1500);
    lcd.clear();
    lcd.setCursor (0, 0);
    lcd.print("Hazard Detected!");
    tone(3, 250); // B4
    delay(1500);
    noTone(3);

    ///////////////////
    if(sent_Stat == 0){
      send_SMS();
      sent_Stat = 1;
      }
    ////////////////////
  }
  else {
    sent_Stat = 0;
    digitalWrite(red_Led, LOW);
    digitalWrite(green_Led, HIGH);
    lcd.clear();
    lcd.setCursor (1, 0);
    lcd.print("= Sensor Val =");
    lcd.setCursor (6, 1);
    lcd.print(sensorValue);
    lcd.setCursor (9, 1);
    lcd.print("%");

  }


  // wait 300 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(300);
}

void send_SMS() {

  while (!Serial);

  //Being serial communication witj Arduino and SIM800
  serialSIM800.begin(9600);
  delay(1000);

  Serial.println("Setup Complete!");
  Serial.println("Sending SMS...");

  //Set SMS format to ASCII
  serialSIM800.write("AT+CMGF=1\r\n");
  delay(1000);

  //Send new SMS command and message number
  serialSIM800.write("AT+CMGS=\"YOUR MOBILE NUMBER\"\r\n");
  delay(1000);


  //Send SMS content
  serialSIM800.write("CONTENT OF THE SMS");
  delay(1000);

  //Send Ctrl+Z / ESC to denote SMS message is complete
  serialSIM800.write((char)26);
  delay(1000);

  Serial.println("SMS Sent!");
}
