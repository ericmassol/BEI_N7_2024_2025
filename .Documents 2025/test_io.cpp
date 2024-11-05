//                                            SKETCH FOR THE BOARD INSIDE FISHER


/**
 * This sketch is written to work permanently 
 * It collects data from GPS, IMU and BMP
 * It storages this data on arrays
 * It makes the mean each second
 * It sends the results thrwo LoRA chanel
 * It archives the result on SD card
 * */ 

//--------------------------------------------------------------------------------------------------------------------------
// REFERENCES

// BOARD ESP32 LOLIN32 Lite

// GPS receiver BN 220
// This sample sketch demonstrates the normal use of a TinyGPSPlus (TinyGPSPlus) object. 
// From a predefined example in library TinyGPSPlus.
// It requires the use of SoftwareSerial, and assumes that working with a
// 9600-baud serial GPS device hooked up on pins 17(GPSRX) and 16(GPSTX).

// IMU 9DoF LSM9DS1
// For the Inertial Measurement Unit I used the basic example for reading the measures from LSM9DS1.
// The built-in basic example for reading using I2C communication is implemented with 
// guidelines found at https://lowpowerlab.com/guide/moteino/m0-sensor-shields/ 

// LoRa receiver SX1278
// For the LoRa communication protocol, I used a basic example of sender which is built-in in the library
// With the help of this tutorial https://randomnerdtutorials.com/esp32-lora-sensor-web-server/ 

// Temperature Sensor BMP280
// From a basic example in the library.
// Used to control the temperature of the electronics during operation, 
// and to activate a heater through a MOSFET switch if needed
//--------------------------------------------------------------------------------------------------------------------------



// LIBRARIES
#include <Arduino.h>
#include <math.h>


// GPS Libraries
#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>


// I2C Communication Library
#include <Wire.h>
// I2C addresses 0x6B and 0x1E for the IMU


// IMU library
#include <SPI.h>
#include <SparkFunLSM9DS1.h>


// LoRa Library
#include <LoRa.h>

// SD card Library
#include <SD.h>
#include <FS.h>

// BMP280
#include "driver/rtc_io.h"
#include <Adafruit_BMP280.h>

#include "config.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



// USED PINS


// For the GPS receiver
static const int RX_GPS_Pin = 16;
static const int TX_GPS_Pin = 17;


// For the IMU and the Differential Pressure sensor (I2C communication protocol)
#define SDA_pin 0
#define SCL_pin 4


// For the LoRa modem
const int csLORA = 5;            // LoRa radio chip select
const int resetPin = 14;        // LoRa radio reset
const int irqPin = 2;           // LoRa hardware interrupt pin

#define  LoRa_SCK   18
#define  LoRa_MISO  19
#define  LoRa_MOSI  23
#define  LoRa_CS     5
#define  LoRa_RST   14    //  LoRa_Reset
#define  DI0         2    //  LoRa_IRQ


// For SD card
// WARNING: put a 4.7kohm resistor on MISO-output of the SD!!!
// SD card is wired on same LoRa SPI but the csPin is different
// https://forum.arduino.cc/t/two-spi-devices-not-working-together/858472
// STD pins for SPI on LOLIN32 are
// 19 MISO
// 23 MOSI
// 18 SCK
//  5 SS
#define  SD_CLK     18
#define  SD_MISO    19
#define  SD_MOSI    23
#define  SD_CS      33

const int csSD = 33;            /* SD chip select */
bool SD_OK = true;

// For the MOSFET that controls the heater 
gpio_num_t TempControlPIN = GPIO_NUM_26;



//--------------------------------------------------------------------------------------------------------------------------
// IMPORTANT NOTES
// The GPS receiver, the Lora modem and the IMU are powered through the 3.3V supply from the ESP32 LOLIN32 Lite
// The IMU can be set for both I2C or SPI communication. This sketch uses I2C.
// The same is valid for the BMP280 sensor.
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define S_TO_uS_FACTOR 1000000ULL      /* Conversion factor from seconds to micro seconds */
#define MINS_TO_MILLIS 60*1000         /* Conversion factor from minutes to milli seconds */
#define S_TO_MILLIS 1*1000                /* Conversiont factor from sec to milli seconds*/

#define LORA_BAND 433E6                /* LoRa frequency allowed depending on the regulations of the specific location */
//433E6 for Asia (and nominally for this specific modem)
//866E6 for Europe
//915E6 for North America
#define LORA_SEND_INTERVAL_TIME 5 //in the mail loop, lora message is sent each 5s
unsigned long loraCheckTime = 0;
unsigned int loraMessageCounter = 0; //to validate all sendings during testing



// Time management variables
RTC_DATA_ATTR int EpochTime2Work = 0;       /* Variable to storage time to work */
RTC_DATA_ATTR double minAltON = 0; /*Variable to keep the min Altitude to work */
static const uint32_t GPSBaud = 9600;


String LoRaMessage = "";

// For GPS readings
String GPS_Info = "";

// For IMU readings
String IMU_Info = "";

// For BMP reading
String TMP_Info = "";


//a matrix of floats where rows are:
// lat, lon, alt, mx, my, mz
const int Nrows = 6;
const int Ncols = 60;
int pointer = 0; //because we are just doing the mean, the order of samples is not important
float data[Nrows][Ncols];


// For temperature control
RTC_DATA_ATTR float temp;
#define TEMP_REGULATION 0
#define TEMP_CHECK_INTERVAL_TIME 120 //in the mail loop, temp control is done each 120s
unsigned long tempCheckTime = 0;

#ifdef MOTEINO_ZERO
  #ifdef SERIAL_PORT_USBVIRTUAL
    #define Serial SERIAL_PORT_USBVIRTUAL // Required for Serial on Zero based boards
  #endif
#endif



// I2C SETUP
// Tutorial https://randomnerdtutorials.com/esp32-i2c-communication-arduino-ide/ explains how to properly use this communication
// SDO_XM and SDO_G are both pulled high, so our addresses are:
#define LSM9DS1_M 0x1E // Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG  0x6B // Would be 0x6A if SDO_AG is LOW

// OBJECTS

// For the GPS receiver
// The TinyGPSPlus object
TinyGPSPlus gps;


// The serial connection to the GPS device
SoftwareSerial ssGPS(RX_GPS_Pin, TX_GPS_Pin);


// Use the LSM9DS1 class to create an object
LSM9DS1 imu;


// For the BMP280
Adafruit_BMP280 bmp; // I2C
//Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
//Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


//set type of job
bool debug = false; //if true gps is simulated, otherwise is for flight

//this struct allow to send bytes over LoRA
struct Data_Package{
  unsigned int yearTimeStamp;
  unsigned long espTime;
  bool gpsValidity;
  float altitude;
  float latitude;
  float longitude;
  float temperature;
  float mx, my, mz;
  unsigned int counter;
};

Data_Package myData;


// FUNCTIONS


// DELAY
// This custom version of delay() ensures that the gps object is being "fed". Source: https://dronebotworkshop.com/using-gps-modules/
// With a normal delay, some information from the gps are lost.
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ssGPS.available())
      gps.encode(ssGPS.read());
  } while (millis() - start < ms);
}


// IMU SETUP
// Before initializing the IMU, there are a few settings we may need to adjust.
byte imuSetup() {
  // Use the settings struct to set the device's communication mode and addresses:
  imu.settings.device.commInterface = IMU_MODE_I2C;
  imu.settings.device.mAddress = LSM9DS1_M;
  imu.settings.device.agAddress = LSM9DS1_AG;
  // The above lines will only take effect AFTER calling
  // imu.begin(), which verifies communication with the IMU
  // and turns it on.
  if (!imu.begin())
  {
    Serial.println("Failed to communicate with LSM9DS1.");
    Serial.println("Double-check wiring.");
    Serial.println("Default settings in this sketch will " \
                  "work for an out of the box LSM9DS1 " \
                  "Breakout, but may need to be modified " \
                  "if the board jumpers are.");
    return false;
  }
  return true;
}

// WAKEUP REASON
/* Method to print the reason by which ESP32 has been awaken from sleep */
void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();
  Serial.print("INFO: ");
  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}

void activeSPIfor(String device){
  if (device == "LORA"){
    digitalWrite(LoRa_CS, LOW);    //  SELECT LORA
    digitalWrite(SD_CS, HIGH);      //  DESELECT SD
  }
  else if (device == "SD")
  {
    digitalWrite(LoRa_CS, HIGH);    //  SELECT LORA
    digitalWrite(SD_CS, LOW);      //  DESELECT SD
  }
  else{
    digitalWrite(LoRa_CS, HIGH);    //  SELECT LORA
    digitalWrite(SD_CS, HIGH);      //  DESELECT SD
  }
}


/**
 * READ the SD configuration file named config.txt
 * */

void readConfigFromSD(){
  Serial.println("ACTION: Reading configuration file from SD: /config.txt");
  activeSPIfor("SD");
  File confFile = SD.open("/config.txt");

  if (!confFile) {
    Serial.println("ERROR: The config file cannot be opened... check it!!");
    return;
  }
  Serial.println("INFO: /config.txt is available on SD");
  while (confFile.available()) {
    String buffer = confFile.readStringUntil('\n');
    Serial.println("INFO: configuration line contains: " + buffer); //Printing for debugging purpose
    unsigned int lineLenght = buffer.length();
    unsigned int pos = buffer.indexOf("=") + 1;
    if (buffer.startsWith("StartEpochTime4Fisher=")){
      EpochTime2Work = buffer.substring(pos, lineLenght).toInt();
      Serial.println("INFO: OK line set!!"); //Printing for debugging purpose
      Serial.println(buffer.substring(pos, lineLenght));
    }
    else if (buffer.startsWith("MinAltitude="))
    {
      /* code */
      minAltON = buffer.substring(pos, lineLenght).toDouble();
      Serial.println("INFO: OK line set!!");
    }
    else
      Serial.println("INFO: line not used!");
    
    //do some action here
  }
  confFile.close();   
  activeSPIfor("NONE");
}

// SD Card and LORA INITIALIZATION
bool Init_SD(){
  Serial.println("ACTION: Initialize SD card communication");
  if(!SD.begin(SD_CS)){
    Serial.println("ERROR: Card Mount Failed");
    return false;
  }

  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
    Serial.println("ERROR: No SD card attached");
    return false;
  }
  Serial.println("INFO: Card Mount succeeded");
  Serial.print("INFO: SD Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("INFO: SD Card Size: %lluMB\n", cardSize);
  return true;
}

void Init_LoRa(){
  
  Serial.println("ACTION: Initialize LoRa communication");

  // override the default CS, reset, and IRQ pins (optional)
  LoRa.setPins(LoRa_CS, LoRa_RST, DI0);// set CS, reset, IRQ pin

  if (!LoRa.begin(LORA_BAND)) {             // initialize ratio at 915 MHz
    Serial.println("FATAL ERROR: LoRa init failed. Check your connections.");
    //while (true);                       // if failed, do nothing
    return;
  }
  
  
  // Sync word (0xF3) to match the nacelle
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // Available range: 0-0xFF
  LoRa.setSyncWord(0xF3);

  Serial.println("INFO: LoRa init succeeded.");
}




// LORA SENDER for String type data
void LoRa_sender(String outgoing_message, int workingSecs){
  
  Serial.println("INFO: Transmiting by LORA for " + String(workingSecs) + " seconds");
  
  unsigned long maxMillis = millis() + workingSecs * S_TO_MILLIS;
  Serial.println("Sending " + outgoing_message);
  do{
    LoRa.beginPacket();
    LoRa.println(outgoing_message);
    LoRa.endPacket();
    Serial.print(".");
    smartDelay(500);
  }while ( millis() < maxMillis );
  Serial.println();
}

// LORA SENDER for Struct type data
void LoRa_Struct_Sender(Data_Package newData, int workingSecs){
  Serial.println("INFO: Transmiting by LORA for " + String(workingSecs) + " seconds");
  
  unsigned long maxMillis = millis() + workingSecs * S_TO_MILLIS;
  Serial.println("Sending data struct");
  do{
    LoRa.beginPacket();
    LoRa.write((uint8_t*) &newData, sizeof(newData));
    LoRa.endPacket();
    Serial.print(".");
    smartDelay(500);
  }while ( millis() < maxMillis );
  Serial.println();
}



// LORA RECEIVER
String LoRa_readings(){
  int packetSize = LoRa.parsePacket();
  String LoRaData = "NO_DATA";
  if (packetSize) {
    // read packet
    Serial.print("Received packet: ");
    //message from gondola is: NEXT_MEASUREMENT_EPOCHTIME;REMAINING_CYCLES
    while (LoRa.available()) {
      LoRaData = LoRa.readString();
      Serial.print(LoRaData);
      // print RSSI of packet
      Serial.print(" with RSSI ");
      Serial.println(LoRa.packetRssi());
      Serial.println();
    }
    smartDelay(2000);
  }
  return LoRaData;
}

// PRINTING FUNCTIONS AND DATE EPOCH MANAGEMENT
int yisleap(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
int get_yday(int mon, int day, int year)
{
    static const int days[2][13] = {
        {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334},
        {0, 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335}
    };
    int leap = yisleap(year);

    return days[leap][mon] + day;
}
int secsFromEpoch(int year, int month, int day, int hour, int min, int secs){
    int yday = get_yday(month, day, year);

    //int seconds = secs + min*60 + hour*3600 + yday*86400 +
    //(year-70)*31536000 + ((year-69)/4)*86400 -
    //((year-1)/100)*86400 + ((year+299)/400)*86400;

    // just use secs from 1st January of the current year!! 
    int seconds = secs + min*60 + hour*3600 + yday*86400;

    return seconds;
}

String printTime(int hours, int mins, int secs){
  String myTime = "";
  if (hours < 10) myTime = "0" + String(hours);
  else myTime = String(hours);

  if (mins < 10) myTime += ":0" + String(mins);
  else myTime += ":" + String(mins);

  if (secs < 10) myTime += ":0" + String(secs);
  else myTime += ":" + String(secs);
  return myTime;
}

String printDate(int day, int month, int year){
  String myDate = "";
  if(day < 10) myDate = "0" + String(day);
  else myDate = String(day);

  if(month < 10) myDate += "/0" + String(month);
  else myDate += "/" + String(month);

  myDate += "/" + String(year);
  return myDate;
}

static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1)
      Serial.print('*');
    Serial.print(' ');
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(' ');
  }
  smartDelay(0);
}

static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  smartDelay(0);
}

// to print GPS date/time in good format for python
String sprintDateTime(TinyGPSDate &d, TinyGPSTime &t){
  String tmp;

  if (!d.isValid())
  {
    tmp = F("********** ");
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
    tmp = String(sz);
  }

  if (!t.isValid())
  {
    tmp = tmp + F("********");
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d", t.hour(), t.minute(), t.second());
    tmp = tmp + String(sz);
  }

  return tmp;
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
  if (!d.isValid())
  {
    Serial.print(F("********** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
    Serial.print(sz);
  }
  
  if (!t.isValid())
  {
    Serial.print(F("******** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
    Serial.print(sz);
  }

  printInt(d.age(), d.isValid(), 5);
  smartDelay(0);
}

static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  smartDelay(0);
}


//I/F read/write with SD card

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if(!root){
    Serial.println("Failed to open directory");
    return;
  }
  if(!root.isDirectory()){
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while(file){
    if(file.isDirectory()){
        Serial.print("  DIR : ");
        Serial.println(file.name());
        if(levels){
            listDir(fs, file.name(), levels -1);
        }
    } else {
        Serial.print("  FILE: ");
        Serial.print(file.name());
        Serial.print("  SIZE: ");
        Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void createDir(fs::FS &fs, const char * path){
  Serial.printf("Creating Dir: %s\n", path);
  if(fs.mkdir(path)){
      Serial.println("Dir created");
  } else {
      Serial.println("mkdir failed");
  }
}

void removeDir(fs::FS &fs, const char * path){
  Serial.printf("Removing Dir: %s\n", path);
  if(fs.rmdir(path)){
      Serial.println("Dir removed");
  } else {
      Serial.println("rmdir failed");
  }
}

void readFile(fs::FS &fs, const char * path){
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if(!file){
      Serial.println("Failed to open file for reading");
      return;
  }

  Serial.print("Read from file: ");
  while(file.available()){
      Serial.write(file.read());
  }
  file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if(!file){
      Serial.println("Failed to open file for writing");
      return;
  }
  if(file.print(message)){
      Serial.println("File written");
  } else {
      Serial.println("Write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if(!file){
      Serial.println("Failed to open file for appending");
      return;
  }
  if(file.print(message)){
      Serial.println("Message appended");
  } else {
      Serial.println("Append failed");
  }
  file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
      Serial.println("File renamed");
  } else {
      Serial.println("Rename failed");
  }
}

void deleteFile(fs::FS &fs, const char * path){
  Serial.printf("Deleting file: %s\n", path);
  if(fs.remove(path)){
      Serial.println("File deleted");
  } else {
      Serial.println("Delete failed");
  }
}

void testFileIO(fs::FS &fs, const char * path){
  File file = fs.open(path);
  static uint8_t buf[512];
  size_t len = 0;
  uint32_t start = millis();
  uint32_t end = start;
  if(file){
    len = file.size();
    size_t flen = len;
    start = millis();
    while(len){
        size_t toRead = len;
        if(toRead > 512){
            toRead = 512;
        }
        file.read(buf, toRead);
        len -= toRead;
    }
    end = millis() - start;
    Serial.printf("%u bytes read for %u ms\n", flen, end);
    file.close();
  } else {
    Serial.println("Failed to open file for reading");
  }
  file = fs.open(path, FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }

  size_t i;
  start = millis();
  for(i=0; i<2048; i++){
    file.write(buf, 512);
  }
  end = millis() - start;
  Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
  file.close();
}


// SETUP
void setup(){
  Serial.begin(115200);
  smartDelay(1000); //Take some time to open up the Serial Monitor
  while(!Serial);


  //getting the time to work if first time
  if (EpochTime2Work == 0)
    EpochTime2Work = secsFromEpoch(yON, mON, dON, hON, minON, sON);

  // Initialize I2C communication
  Wire.begin(SDA_pin, SCL_pin);

  //Print the wakeup reason for ESP32
  print_wakeup_reason();
  
  //define pins modes for cs LORA and SD
  pinMode(LoRa_CS ,OUTPUT);
  pinMode(SD_CS,OUTPUT);

  activeSPIfor("NONE");

  // Initialize LoRa communication
  activeSPIfor("LORA");
  Init_LoRa();
  

  // Ini SD card
  activeSPIfor("SD");
  SD_OK = Init_SD();
  // clear existing file and write headlines
  deleteFile(SD, outputFile.c_str());
  String headline = "espTime;timeStamp;gpsValidity;latitude;longitude;altitude;mx;my;mz;temperature;counter\n";
  writeFile(SD,outputFile.c_str(), headline.c_str());
  // by default all SPI slaves down
  activeSPIfor("NONE");

  // Initialize GPS receiver
  ssGPS.begin(GPSBaud);

  // Initialize IMU 
  pinMode(LED_BUILTIN,OUTPUT);
  imu.begin();


  /*// Calibrate IMU
  unsigned long current_time = millis();
  while(millis()-current_time < 60*1000){
    imu.calibrate(true);
    imu.calibrateMag(true);
  }*/
  

  // Initialize BMP280
  bmp.begin(0x77);


  // Setting to keep the MOSFET pin's output as HIGH even during deep sleep
  rtc_gpio_init(TempControlPIN);
  rtc_gpio_set_direction(TempControlPIN,RTC_GPIO_MODE_OUTPUT_ONLY);

  pinMode(TempControlPIN, OUTPUT);


  // Read the temperature as the board wakes up
  temp = bmp.readTemperature();
  Serial.println("INFO: Initial temperature is: " + String(temp));
  // Change the temperature value for the final version (for instance, -10°C)
  if (temp > TEMP_REGULATION + 5){                                              /* Set coherent values of temperature once the thermal analysis is complete */
    rtc_gpio_set_level(TempControlPIN,0);                         /* MOSFET closed, heater turned off */
    digitalWrite(TempControlPIN, LOW);
  }
  else if (temp < TEMP_REGULATION - 5){
    rtc_gpio_set_level(TempControlPIN,1);                          /* MOSFET open, heater turned on */
    digitalWrite(TempControlPIN, HIGH);
  }


  //read flight config from SD
  //it does nothing if file config.txt does not exist on SD card root folder
  if (SD_OK) readConfigFromSD();

  

  //initialize the data array
  float ini_lat = 45.;
  float ini_lon = -11.;
  float ini_alt = 100.;

  float ini_mx = 0, ini_my = 0, ini_mz = 0;

  if (imuSetup()){
    imu.readMag();
    ini_mx = imu.mx;
    ini_my = imu.my;
    ini_mz = imu.mz;
  }

  for(int ii=0; ii<Ncols; ii++){
    data[0][ii] = ini_lat;
    data[1][ii] = ini_lon;
    data[2][ii] = ini_alt;
    data[3][ii] = ini_mx;
    data[4][ii] = ini_my;
    data[5][ii] = ini_mz;
  }

  tempCheckTime = millis();
  loraCheckTime = millis();
  loraMessageCounter = 0;

  randomSeed(0);

  //TO TEST THE MAGNET
  bool test_magnet = false;
  unsigned long maxTime = millis() + 300 * 1000;
  while (millis() < maxTime and test_magnet){
    imu.readMag();
    ini_mx = imu.mx;
    ini_my = imu.my;
    ini_mz = imu.mz;
    Serial.println(String(ini_mx) + ";" + String(ini_my) + ";" + String(ini_mz)  );
    delay(1000);
  }

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


// LOOP

void loop(){
  //get new infos
  smartDelay(1000);
  bool gpsValidity;
  
  if (debug){
    gpsValidity = true;
    data[0][pointer] = 44. + random(0, 10);
    data[1][pointer] = -11. + random(0, 10);
    data[2][pointer] = 100. + random(0, 10);
    imu.readMag();
    data[3][pointer] = imu.mx;
    data[4][pointer] = imu.my;
    data[5][pointer] = imu.mz;
    pointer ++;
    pointer %= Ncols;
  }
  else{
    gpsValidity = gps.location.isValid() && gps.date.isValid() && gps.altitude.isValid() && gps.satellites.value() >= 4;
    if(gpsValidity){
      data[0][pointer] = gps.location.lat();
      data[1][pointer] = gps.location.lng();
      data[2][pointer] = gps.altitude.meters();
      imu.readMag();
      data[3][pointer] = imu.mx;
      data[4][pointer] = imu.my;
      data[5][pointer] = imu.mz;
      pointer ++;
      pointer %= Ncols;
    }
  }
  

  //apply means
  float tmp[Nrows];
  for (int ii = 0; ii<Nrows; ii++)
    tmp[ii] = 0;
  
  for (int ii = 0; ii<Nrows; ii++){
    for(int jj=0; jj<Ncols; jj++)
      tmp[ii] += data[ii][jj];

    tmp[ii] /= Ncols;
  }

  // Create string to send via LoRa connection in CSV format
  //ESPTime;GPSTimeStamp;GPSValidity;Lat;Lon;Alt;Temperature;Mx;My;Mz;Counter
  String strValidity = "False"; //because String(gpsValidity) gets "1" or "0"
  if (gpsValidity) strValidity = "True";

  unsigned int my_secs = secsFromEpoch(
    gps.date.year(), gps.date.month(), gps.date.day(),
    gps.time.hour(), gps.time.minute(), gps.time.second()
  );

  //LoRaMessage = String(millis()) + ";" + sprintDateTime(gps.date, gps.time) + ";" + strValidity + ";";
  LoRaMessage = String(millis()) + ";" + String(my_secs) + ";" + strValidity + ";";
 
  for (int ii=0; ii<6; ii++)
    LoRaMessage += String(tmp[ii], 4) + ";";
  LoRaMessage += String(bmp.readTemperature()) + ";";
  LoRaMessage += String(loraMessageCounter);


  //update data struct
  myData.yearTimeStamp = secsFromEpoch(
    gps.date.year(), gps.date.month(), gps.date.day(),
    gps.time.hour(), gps.time.minute(), gps.time.second()
  );

  myData.espTime = millis();
  myData.gpsValidity = gpsValidity;
  myData.latitude = tmp[0];
  myData.longitude = tmp[1];
  myData.altitude = tmp[2];
  myData.mx = tmp[3];
  myData.my = tmp[4];
  myData.mz = tmp[5];
  myData.temperature = bmp.readTemperature();
  myData.counter = loraMessageCounter;


  /* On this state the probe will send the LoRaMessage durint 2min to ensure it is well received by the gondola */
  // It seems that LoRA saturates if send data each second
  // Try to send it each 5s for example
  if (millis() > loraCheckTime){
    Serial.println("\n\n");
    Serial.println("INFO: Sending message " + LoRaMessage);
    activeSPIfor("LORA");
    //LoRa_sender(LoRaMessage, -1); //value -1 indicates that the message is sent once
    LoRa_Struct_Sender(myData, -1);
    activeSPIfor("NONE");
    loraMessageCounter ++;
    loraCheckTime = millis() + LORA_SEND_INTERVAL_TIME * 1000;

    //saving data to SD
    if (SD_OK){
      Serial.println("ACTION: Saving data on SD Card file: " + outputFile);
      activeSPIfor("SD");
      String data2save = LoRaMessage + "\n";
      const char* line2write = data2save.c_str();
      appendFile(SD, outputFile.c_str(), line2write);    
      activeSPIfor("NONE");
    }
  }

  

  //temperature regulation
  //this shall be done each 120s for example
  if (millis() > tempCheckTime){
    Serial.println("\n\n");
    tempCheckTime = millis() + TEMP_CHECK_INTERVAL_TIME * 1000;
    // Read the temperature as the board wakes up
    temp = bmp.readTemperature();
    Serial.println("INFO: Current temperature is: " + String(temp));
    // Change the temperature value for the final version (for instance, -10°C)
    if (temp > TEMP_REGULATION + 5){                                              /* Set coherent values of temperature once the thermal analysis is complete */
      rtc_gpio_set_level(TempControlPIN,0);                         /* MOSFET closed, heater turned off */
      digitalWrite(TempControlPIN, LOW);
      Serial.println("  --> Heater OFF");
    }
    else if (temp < TEMP_REGULATION - 5){
      rtc_gpio_set_level(TempControlPIN,1);                          /* MOSFET open, heater turned on */
      digitalWrite(TempControlPIN, HIGH);
      Serial.println("  --> Heater ON");
    }
    Serial.println("\n\n");
  }
} 