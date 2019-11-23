
//ESP8266 Stuff
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ConfigManager.h>
//Time Stuff
#include <EasyNTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
//DFPlayer Stuff
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>
//Flickering Stuff
#include <WS2812FX.h>
#define LED_COUNT 20
#define LED_PIN 12
// Pin D1 for button and D2 for busy-checker
#define BUTTON_PIN 5
#define DFP_BUSY_PIN 4


ConfigManager configManager;
struct Config {
    char name[20];
    bool enabled;
    int8_t hour;
    char password[20];
} config;
struct Metadata {
    int8_t version;
} meta;

int buttonState = 0;
int lastButtonState = 0;
int busyState;
int currentDay = 0;

WiFiUDP ntpUDP;
EasyNTPClient ntpClient(ntpUDP, "de.pool.ntp.org");

WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

//Connecton to DFPlayer
SoftwareSerial playerConn(13, 15); //RX, TX
DFRobotDFPlayerMini mp3;


void setup() {
  
  Serial.begin(115200);
  Serial.println();

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  configManager.setAPName("@ventskalender");
  configManager.setAPFilename("/index.html");
  configManager.addParameter("name", config.name, 20);
  configManager.addParameter("enabled", &config.enabled);
  configManager.addParameter("hour", &config.hour);
  configManager.addParameter("password", config.password, 20, set);
  configManager.addParameter("version", &meta.version, get);
  configManager.begin(config);
  
  //Establishing Serial-Connection to DFPlayer
  Serial.println("Initializing DFPlayer ... (May take 3~5 seconds)");
  playerConn.begin(9600);
  if (!mp3.begin(playerConn, false)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  mp3.volume(30);
  Serial.println("DFPlayer Mini online.");
  uint16_t vol = mp3.readVolume();
  Serial.print("Volume is: ");
  Serial.print(vol);
  Serial.println();

  //initializing the LEDs
  ws2812fx.init();
  
}

//Functions for returning just the Month and Day, because we don't need a year for our @ventskalender


void loop() {
  configManager.loop();
  mp3.pause();
  // if-case for opening the chest, so its a "if the cachest is open"
  buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == LOW){
    Serial.println("Chest was opened");
    //Checking if its december
    Serial.println("Checking month...");
    if (month(ntpClient.getUnixTime() + 3600) == 11 ){
      Serial.println("Hooray, it's december \\O/");
      
      currentDay = day(ntpClient.getUnixTime() + 3600);
      
      mp3.play(currentDay);
      
      Serial.print("Now playing Track ");
      Serial.print(currentDay);
      Serial.println();

      
      //ws2812fx.setBrightness(80);
      ws2812fx.setSpeed(200);
      ws2812fx.setMode(FX_MODE_FIRE_FLICKER_INTENSE);
      ws2812fx.start();
      
      busyState = digitalRead(DFP_BUSY_PIN);
      while (buttonState == LOW){
        buttonState = digitalRead(BUTTON_PIN);
        ws2812fx.service();
      }
      ws2812fx.stop();
      
    }
  }
  
  lastButtonState = buttonState;
  
}
