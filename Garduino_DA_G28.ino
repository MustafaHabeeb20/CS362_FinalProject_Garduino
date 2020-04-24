#include <Time.h>
#include <TimeLib.h>
#include <dht.h>
#define dht_apin A0 //Analog pin sensor that is connected to the tem/humidity sensor
#include <SPI.h>
#include <SD.h>

dht DHT;
File myFile;

//time_t t = now();
int hr;
int mint;
int state = 0; //Bluetooth
int uv_ain=A1;
int ad_value;

void setup() {
  pinMode(uv_ain, INPUT);
  Serial.begin(9600);
  delay(500);         //A small delay to let the temp/humidity system to boot
  //Serial.println("DHT11 Humidity & Temperature Sensor & UV Sensor");
  delay(1000);
  while (!Serial) {
    ;     //Wait for serial port to connect. Needed for native USB port only (For the SD card reader)
  }
  Serial.print("Initializing SD card...");
  
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }

  Serial.println("initialization done.");
  
  // open the file. Only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("garden.txt", FILE_WRITE);

  delay(2000);
}

void loop() {

    if(Serial.available() > 0){        
      state = Serial.read();                         
    }

    //t = now();
    DHT.read11(dht_apin);
    ad_value=analogRead(uv_ain);
    
//    Serial.print("Current humidity = ");
//    Serial.print(DHT.humidity);
//    Serial.print("%  ");
//    Serial.print("temperature = ");
//    Serial.print(DHT.temperature); 
//    Serial.println("C  ");

//    if(ad_value>20){
//      Serial.println("UV up the standard");
//    }
//    else{
//      Serial.println("UV down the standard");
//    }

    // if the file opened okay, write to it:
    if (SD.exists("garden.txt")) {
      //Open the file
      myFile = SD.open("garden.txt", FILE_WRITE);

      hr = hour();
      mint = minute();

      
      myFile.print(hr);
      myFile.print(":");
      myFile.print(mint);
      myFile.print(",");
      myFile.print(DHT.humidity);
      myFile.print(",");
      myFile.print(DHT.temperature*1.8+32);
      myFile.print(",");
      myFile.print(ad_value);
      myFile.println("");

      //Close the file:
      myFile.close();
      Serial.println("done.");
      
      Serial.print("Current humidity = ");
      Serial.print(DHT.humidity);
      Serial.print("%  ");
      Serial.print("Temperature = ");
      Serial.print(DHT.temperature*1.8+32); 
      Serial.println("F  ");
      Serial.print("UV Light = ");
      Serial.println(ad_value);
    }
//    else {
//      // if the file didn't open, print an error:
//      Serial.println("ERROR");
//    }

    if(state == '1'){
      Serial.print("Current humidity = ");
      Serial.print(DHT.humidity);
      Serial.print("%  ");
      Serial.print("Temperature = ");
      Serial.print(DHT.temperature*1.8+32); 
      Serial.println("F  ");
      Serial.print("UV Light = ");
      Serial.println(ad_value);
      state = 0;
    }
    
    delay(1000);//Wait 20 seconds before accessing sensor again.
 
}
