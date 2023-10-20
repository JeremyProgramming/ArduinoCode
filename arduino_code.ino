//---------------------------------------------------------------------------------
/*
Program:      Final Code for Arduino
Author:       Jeremiah Kamidi
Date:         18/10/2023
Version:      3.0

Description:  
  - The program powers a motor connected to the appropriate pin when the
    value of a sensor is greater than the tipping point. It will
    Then it will continute adding water if the moisture value is between
    the soilWet and air. After that sleep for user defined time.

Usage:
  Change the defines underneath to tweak and change the program:
  - soilWet: should be changed if the moisture sensor for any reason
    does not go off
  - air: value for when the moisture sensor is in the air

  - Pins: change the values to the appropriate pins. (Note: the
    moisture sensor analog pin should be in an analog pin and the rest
    should be in digital pins)

  - delays: change these value according to how fast your plant drinks water 
  - miliMetersIncrement: milimeters to add everytime

  - miliMetersToOutput: first time milimeter addition
  - maxWater: Max water before the loop terminates 

  - miliLiterPerSecond: Calibrate the motor speed calculated in mm/second 
*/

// Tipping point to water the plant
#define soilWet 720                           // Define min value to consider the soil to be wet
#define air 950                               // Define when the moisture sensor is in the air

// Pins setup
#define motorPin 3                            // Motor digital Pin
#define moisturePower 2                       // Moisture sensor Digital Pin 
#define moisturePin A0                        // Moisture

// Delay
#define delayCycle 86400000                   // Delay per cycle, currently 1 day
#define delayInterval 900000                  // Delay between intervals

// Water Specifications
#define miliMetersToOutput 50                 // Add 50 milimeters to
#define maxWater 1000                         // Max water before the loop stops

// Motor Specifications
#define miliLiterPerSecond 20.15              // Motor water speed in milimeters per second
//---------------------------------------------------------------------------------

void setup() {
	pinMode(moisturePower, OUTPUT);       // Set the moisture pin to output
  pinMode(motorPin, OUTPUT);                  // Set the motor pin to output

	Serial.begin(9600);
}

void loop() {
	int moisture = readMoistureSensor();  // Read the moisture value
  int totalWater = 0;

  // Compare the current moisture to the tipping point and check
  // if the moisture sensor is not in the air and if the total amount of water
  // does not exceed the max water. Add additional water if the moisture value still is low
  while(moisture >= soilWet && moisture <= air 
        && totalWater <= maxWater) {
    totalWater += miliMetersToOutput;         // Update total water
    powerMotor(miliMetersToOutput);           // Power the motor

    delay(delayInterval);                     // Delay for this interval
    moisture = readMoistureSensor();          // Reread the moisture
  }

  
  Serial.println("Wait for the next cycle");
	delay(delayCycle);                   // Delay for next read              
}

// return the humidity of the soil
int readMoistureSensor() {
	digitalWrite(moisturePower, HIGH);   // Turn the sensor ON
	delay(10);			     // Let the moisture sensor read the input

	int humidity = analogRead(moisturePin);// Read the value form the moisture sensor
	digitalWrite(moisturePower, LOW);    // Turn the sensor off
	
  Serial.print("Moisture Value:");
  Serial.println(humidity);

  return humidity;			    // Return analog moisture value
}


// Takes a desired amount of milimeters of water and powers the motor
void powerMotor(int input_milimeters) {
  Serial.println("Pumping water...");
  digitalWrite(motorPin,HIGH);               // Turn on the motor

  // Calculate the amount of time the motor should be on
  int miliSeconds = (int) ((input_milimeters / miliLiterPerSecond) * 1000); 

  Serial.print("Powering motor and adding: ");
  Serial.print(input_milimeters);
  Serial.println("ml to the plant area.");
  
  delay(miliSeconds);                       // Wait for the amount of time

  digitalWrite(motorPin,LOW);               // Turn off the motor
  Serial.println ("Stopped motor...");
}
