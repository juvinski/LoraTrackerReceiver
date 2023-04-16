//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 433E6

//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

String LoRaData;

String new_gpsdata[5];
String old_gpsdata[5];

void pingBola(int _value) 
{
  float circ_radios = 2.5;
  display.fillCircle(display.width() - circ_radios, display.height() - circ_radios, circ_radios, _value);
}

void separaLora(String _param)
{
  int x = 0;
  int y = 0;
  _param.concat("...");
  for(int i=0;i < _param.length(); i++)
  {
    if(_param.charAt(i) == ':')
    {
      new_gpsdata[y] = _param.substring(x,i);
      Serial.printf("DADO [%d]: %s\n", i, _param.substring(x,i));
      y++;
      x = i + 1;
    }
  }
}

void setup() { 
  //initialize Serial Monitor
  Serial.begin(115200);
  
  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("DRONE TRACKER");
  display.display();
  Serial.println("LoRa Drone Tracker");
  
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initializing OK!");
  display.setCursor(0,10);
  display.println("LoRa Initializing OK!");
  display.display();
}

void loop() {

  int packetSize = LoRa.parsePacket();
  if (packetSize) 
  {
    //received a packet
    Serial.print("Received packet ");
    //read packet
    while (LoRa.available()) 
    {
      LoRaData = LoRa.readString();
      Serial.println(LoRaData);
    }
    separaLora(LoRaData);
  }

  //print RSSI of packet
  int rssi = LoRa.packetRssi();
  //Serial.print(" with RSSI ");    
  //Serial.println(rssi);

  // Display information
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("DRONE TRACKER");
  display.setCursor(0,10);
  
  //display.print(LoRaData);
  //separaLora(LoRaData);
  //Serial.printf("D1: %s\nD2: %s\n D3: %s\n",new_gpsdata[0], new_gpsdata[1],new_gpsdata[2] );
  if(new_gpsdata[5] != old_gpsdata[5])
  {
    //Serial.println("New Data!");
    display.setCursor(0,20);
    display.print("LAT:");
    display.setCursor(25,20);
    display.print(new_gpsdata[0]);
    display.setCursor(0,30);
    display.print("LNG:");
    display.setCursor(25,30);
    display.print(new_gpsdata[1]);
    display.setCursor(0,40);
    display.print("SAT:");
    display.setCursor(25,40);
    display.print(new_gpsdata[2]);
    display.setCursor(50,40);
    display.print("ALT:");
    display.setCursor(75,40);
    display.print(new_gpsdata[3]);
    old_gpsdata[0] = new_gpsdata[0];
    old_gpsdata[1] = new_gpsdata[1];
    old_gpsdata[2] = new_gpsdata[2];
    old_gpsdata[3] = new_gpsdata[3];
    old_gpsdata[4] = new_gpsdata[4];
    old_gpsdata[5] = new_gpsdata[5];
    pingBola(1);
  }
  else
  {
    //Serial.println("OLD DATA!");
    display.setCursor(0,20);
    display.print("LAT:");
    display.setCursor(25,20);
    display.print(old_gpsdata[0]);
    display.setCursor(0,30);
    display.print("LNG:");
    display.setCursor(25,30);
    display.print(old_gpsdata[1]);
    display.setCursor(0,40);
    display.print("SAT:");
    display.setCursor(25,40);
    display.print(old_gpsdata[2]);
    display.setCursor(50,40);
    display.print("ALT:");
    display.setCursor(75,40);
    display.print(old_gpsdata[3]);
    pingBola(0);
  }
  display.setCursor(0,50);
  display.print("RSI:");
  display.setCursor(25,50);
  display.print(rssi);
  display.display();
  //delay(1000);
}

