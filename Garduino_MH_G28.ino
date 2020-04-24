/* 
 *  
 *  Mustafa Habeeb and Daniel Aguilar - Group 28 
 *  Title: Garduino
 *  Water Pump
 *  Soil Sensor
 *  Bluetooth module using serial
 *  Ultasonic Sensor to show if the bottle is filled 
 *          
 *          
*/

//SETUP SENSORS 
int soilSensor = A0;          //analog pin
int waterPump = 12;           //digital pin
int trigPin = 9;              //digital pin for UltraSonic Sensor - emits UltraSound
int echoPin = 10;             //digital pin for UltraSonic Sensor - recieves UltraSound

//OTHER VARIABLES
int state = 0;                //initialize at 0, assists in taking input and reading via blutooth
int soilSensorVal = 0;        //initialize at 0, but will be read from analog
int readCounter = 0;          //initialize at 0, used to count the number of reads when the soil is moist
int waterPumpCounter = 0;     //initialize at 0, used to count the number of times the water has been pumped 
int USSensorVal = 0;          //initialize at 0, but will be read from after we caclulated distance
long duration = 0;            //initialize at 0, used to record soundwave time traveled in microseconds     
int distance = 0;             //initialize at 0, calculates the distance using duration, velocity = 340 m/s,
                              //  and divinding by 2 becuase of the sound travels forward and backward 

void setup(){
  
  Serial.begin(9600);         //Starts the serial communication
  pinMode(soilSensor, INPUT); //will take in input of how the soil is (either wet or dry)
  pinMode(waterPump, OUTPUT); //if the soil is dry, the waterPump will send water.
                              //  When the soild is moist (wet), the water pump will do nothing.
                              //UltraSonic Sensor... 
  pinMode(trigPin, OUTPUT);   //Sets the trigPin as an Output
  pinMode(echoPin, INPUT);    //Sets the echoPin as an Input
  delay(500);
}//End of setup

void loop(){

  if(Serial.available() > 0){                      //Checks whether data is comming from the serial port
    state = Serial.read();                         //Reads the data from the serial port
  }
  
  soilSensorVal = analogRead(soilSensor);      
  soilSensorVal = map(soilSensorVal,1023,0,0,100); //The soil Sensor reads values from 0 to 1023, cetain ranges mean different things...
                                                   //source: https://www.instructables.com/id/Soil-Moisture-Measurement-With-Arduino/
                                                   //    Ranges: around 1000 - 1023 -> not in soil.
                                                   //            around 600 - 1000  -> in dry soil.
                                                   //            around 370 - 600   -> in humid soil.
                                                   //            around 0 - 370     -> in water. 
                                                   //With with this knowlege we were able to map the values to show percentages,
                                                   //  and after running a few different trials, 55% of more, shows the soild will 
                                                   //  be humid, otherwise, the soil is considered dry and will need to be watered.  

                                                             
 
  Serial.print(readCounter);                       //Output should look like ...
  Serial.print(". ");                              // "0. Moisture : (some percentage value)% 
  Serial.print("Mositure: ");
  Serial.print(soilSensorVal);
  Serial.println("%");

  int dryVal = 55;                                 //We know 55% is the cutoff between humid and dry soil,
                                                   // for thsi reason we will set the dry value at.                                       
  
  if(soilSensorVal <= dryVal){
   Serial.println("watering");
   digitalWrite(waterPump, HIGH);                  //Pump water 
   delay(3000);                                    //3 second delay after every time it needs to be watered till the soild sensor reads above 55% 
   digitalWrite(waterPump, LOW);                   //Stop pumping water 
   delay(5000);                                    //5 second delay after watering. 
   readCounter = 0;                                //set readCounter to 0 to see where it breaks in Serial monitor.  
   waterPumpCounter++;                             //Count every time the water was pumped
  }//End of if-statement  
  readCounter++;                                   //Add to the counter as long as the soilSensorVal reads higher than the dryVal    

                                                   //source: https://howtomechatronics.com/tutorials/arduino/ultrasonic-sensor-hc-sr04/
  digitalWrite(trigPin, LOW);                      //Clears the trigPin
  delayMicroseconds(2);                            //Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);               //Reads the echoPin, returns the sound wave travel time in microseconds
  distance= duration*0.034/2.0;                    //Calculating the distance
  Serial.print("   Distance: ");                   //Prints the distance on the Serial Monitor
  Serial.println(distance);

  USSensorVal = map(distance,14,2,0,100);          //Map distance from 2cm to 14cm to a range from 0 to 100 representing percentages
                                                   // where 14cm maps 0% where the cup is empty and 2cm maps to 100% cause the cup is filled


  //STATES used to show blueTooth communication Inputs are 1 and 2 
  if(state == '1'){                                //If input for State is equal to 1 
    Serial.print("water level is at ");            //Give percentage of water-level from the cup the water pump is reciving from
    Serial.print(USSensorVal); 
    Serial.println("%");
 
    if(USSensorVal <= 40){                         //If the USSensorVal is less than equal to 40% than the USER will know to 
                                                   //  fill up the cup so they never run out of water
      Serial.println("Fill up water cup");
    }
    state = 0; 
  }//End of if-statement

  if(state == '2'){                                //If input for State is equal to 2 
    Serial.print("Number of pumps: ");             //Give number of pumps 
    Serial.println(waterPumpCounter);
    state = 0; 
  }//End of if-statement

  
  delay(5000);                                     //5-second delay 
}//End of loop 
