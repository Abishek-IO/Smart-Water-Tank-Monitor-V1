// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID           "xxxxxxxxxxxxxxx"
#define BLYNK_DEVICE_NAME           "xxxxxxxxxxxxxxx"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_WROVER_BOARD
//#define USE_TTGO_T7
//#define USE_ESP32C3_DEV_MODULE
//#define USE_ESP32S2_DEV_KIT

#include "BlynkEdgent.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SOUND_SPEED 0.034
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int trigPin = 18;
const int echoPin = 19;
const int redled = 2;
const int blueled = 4;
const int min_value = 10;
const int max_value = 57;
long duration;
float distanceCm;
float percentage;
int inverse_percentage;
int full;
int empty;
const int buzzer = 13;
const int Auto = 27;
const int manual = 26;
const int buzzer_mode = 23;
int buttonstate1 = 0;
int buttonstate2 = 0;
int buttonstate3 = 0;
const int autoled = 33;
const int manualled = 32;
String operation_mode;
String tank_status;
String buzzer_status;
String motor_status;
void setup()
{
  Serial.begin(115200);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(1000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(13, 0);
  display.println("Smart Water Level");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(45, 10);
  display.println("Monitor");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 30);
  display.println("Connecting to Wi-Fi");
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 40);
  display.println("and Server... Please wait");
  display.display(); 

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redled, OUTPUT);
  pinMode(blueled, OUTPUT);
  pinMode(Auto, INPUT);
  pinMode(manual, INPUT);
  pinMode(manual, INPUT);
  pinMode(autoled, OUTPUT);
  pinMode(manualled, OUTPUT);
  pinMode(buzzer, OUTPUT);
  delay(5000);

  BlynkEdgent.begin();
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * SOUND_SPEED/2;
  Serial.print("Distance(cm): ");
  Serial.println(distanceCm);
  if (distanceCm < min_value)
  {
    percentage = 0;
    } 
  else if (distanceCm > max_value)
  {
    percentage = 100; 
    }
   else
   {
    percentage = ((distanceCm - min_value)/(max_value - min_value))*100;
    }   
  Serial.print("Percentage: ");
  Serial.println(percentage);
  inverse_percentage = 100 - percentage;
  Blynk.virtualWrite(V0, inverse_percentage);
  Serial.print(" Inverse Percentage: ");
  Serial.println(inverse_percentage);
  buttonstate3 = digitalRead(buzzer_mode);
  if (inverse_percentage == 100)
  {
    full = 1;
    empty = 0;
    digitalWrite(redled, HIGH);
    digitalWrite(blueled, LOW);
    tank_status = "Full";
    if (buttonstate3 == HIGH)
    {
    digitalWrite(buzzer, HIGH);
    }
    else
    {
      digitalWrite(buzzer, LOW);
      }
  }
  else if (inverse_percentage == 0)
  {
    full = 0;
    empty = 1;
    digitalWrite(redled, LOW);
    digitalWrite(blueled, HIGH);
    tank_status = "Empty";
    if (buttonstate3 == HIGH)
    {    
    digitalWrite(buzzer, HIGH);
    delay(1000);
    digitalWrite(buzzer, LOW);
    }
    else
    {
      digitalWrite(buzzer, LOW);
      }
  }
  else
  {
    full = 0;
    empty = 0;
    tank_status = "Half Full";
    digitalWrite(buzzer, LOW);
    }
    
buttonstate1 = digitalRead(Auto);
buttonstate2 = digitalRead(manual);
  if (buttonstate1 == HIGH)
  {
    Serial.println("Auto mode enabled");
    digitalWrite(autoled, HIGH);
    digitalWrite(manualled, LOW);
    operation_mode = "Auto";
    }
    
if(buttonstate2 == HIGH)
    {
      Serial.println("Manual mode enabled");
      digitalWrite(manualled, HIGH);
      digitalWrite(autoled, LOW);
      operation_mode = "Manual";
      }
      
if (buttonstate3 == HIGH)
{
  buzzer_status = "On";
  }
else
{
  buzzer_status = "Off";
  }
   
if (operation_mode == "Manual")
{
  motor_status = "NA";
  }
  else
  {
    motor_status = "";
    }
                 
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(9, 0);
display.println("Water Level Monitor");
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0, 10);
display.println("Water Level: "+String(inverse_percentage)+"%");
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0, 20);
display.println("Tank Status:"+tank_status);
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0, 30);
display.println("Operation Mode:"+operation_mode);
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0, 40);
display.println("Buzzer Status: "+buzzer_status);
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(0, 50);
display.println("Motor Status: "+motor_status);
display.display(); 

Blynk.virtualWrite(V1, full);
Blynk.virtualWrite(V2, empty);  
BlynkEdgent.run();
delay(1000);
}
