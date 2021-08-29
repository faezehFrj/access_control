//library

//***wifi and MQTT***//

#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

//***RFID***//
#include <SPI.h>//https://www.arduino.cc/en/reference/SPI
#include <MFRC522.h>//https://github.com/miguelbalboa/rfid

//***fingerprint***//
#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>

//***oled***//
#include "SSD1306.h"
#include "Fonts.h"
#include "fingerprint_image.h"

//***neopixel***//
#include "Freenove_WS2812_Lib_for_ESP32.h"

//************Constants************//
#define SS_PIN 5
#define RST_PIN 4
#define touch_pin 32

//** neopixel **//
#define LEDS_COUNT  8
#define LEDS_PIN  25
#define CHANNEL   0

//** relay **//
#define relay_door_pin 27

//**buzzer**//
#define buzzer_pin 26

//**Button for ringing**//
#define button_ringing_pin 15


//************set Parameters************//

//**wifi**//

const char* ssid = "QSTP-Wifi";
const char* password = "";


//**mqtt**//

const char* mqtt_server = "172.16.16.114";
float temperature = 0;
float humidity = 0;
const int ledPin = 2;
char publishMessage[20];
String employee_name = "";
String employee_time = "";


//**fingerprint**//
String status_button = "normal";
String messege_break = "coun";
String result_enroll = " ";
char result_enrollString[2];
int id_new_fingerprint;
String getFingerprint1 = "stop";
String getFingerprint2 = "stop";
String moveToTable1 = "stop";
String moveToTable2 = "stop";
int p;
int read_touch_button = 1;

//************config************//

//**wifi**//
WiFiClient espClient1;

//**mqtt**//
PubSubClient client(espClient1);


//**RFID**//
byte nuidPICC[4] = {0, 0, 0, 0};
MFRC522::MIFARE_Key key;
MFRC522 rfid = MFRC522(SS_PIN, RST_PIN);


//***fingerprint***//
HardwareSerial mySerial(2);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

//***neopixel***//
Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL, TYPE_GRB);


//****oled***//
SSD1306 display(0x3C, 22, 21);



//************setup************//

void setup() {

  Serial.begin(115200);

  //**wifi**//
  setup_wifi();

  //**mqtt**//
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  //**RFID**//
  SPI.begin();
  rfid.PCD_Init();

  Serial.print(F("Reader :"));
  rfid.PCD_DumpVersionToSerial();

  //**fingerprint88//
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }
  pinMode(touch_pin, INPUT);

  //*********

  //*** oled ***//
  display.init();
  display.setFont(Dialog_bold_17);
  display.flipScreenVertically();//tasaviro bar migardon

  display.drawString(40, 0, "ARTA");
  display.display();
  delay(1000);
  display.drawString(1, 26, "Loading...");
  display.display();
  delay(2000);
  display.drawString(1, 45, "Connecting");
  display.display();
  delay(2000);
  //*** oled ***//



  //***neopixel***//
  strip.begin();

  //*********
  pinMode(ledPin, OUTPUT);

  //***setuip relay***//
  pinMode(relay_door_pin, OUTPUT);
  digitalWrite(relay_door_pin, LOW);

  //***buzzer***//
  pinMode(buzzer_pin, OUTPUT);
  digitalWrite(buzzer_pin, LOW);

  //***button for ringing***//
  pinMode(button_ringing_pin, INPUT);
  digitalWrite(button_ringing_pin, LOW);

}


//************loop************//

void loop() {

  //**mqtt**//
  connerct_broker();

  //**rfid**//
  readRFID();

  //**fingerprint find**//
  if (status_button == "fingerprintRun") {
    getFingerprintIDez();
  }

  //**config button**//

  read_touch_button = digitalRead(touch_pin);
  if (read_touch_button == 0) {
    client.publish("button/click", "0");
  }

  //***button for ringing***//

  if (digitalRead(button_ringing_pin) == 0) {
    client.publish("door/ringed", "0");
  }

  //**add new fingerprint**//
  if (getFingerprint1 == "run") {
    finger_getImage(id_new_fingerprint);
  }

  if (getFingerprint2 == "run") {
    same_finger_getImage(id_new_fingerprint);
  }


}
