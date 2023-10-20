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
  - soilDry: value for when the moisture sensor is in the air

  - Pins: change the values to the appropriate pins. (Note: the
    moisture sensor analog pin should be in an analog pin and the rest
    should be in digital pins)

  - delays: change this value according to how fast your plant drinks water 
  - miliMetersIncrement: milimeters to add everytime.
  - startingMiliMeters: first time milimeter addition
  - miliMeters: Target milimeters the motor should output

  - miliLiterPerSecond: Calibrate the motor speed calculated in mm/second 
*/

// Tipping point to water the plant
#define soilWet 720   // Define min value to consider the soil to be wet
#define air 950 // Define when the moisture sensor is in the air

// Pins setup
#define motorPin 3 // Motor digital Pin
#define moisturePower 2 // Moisture sensor Digital Pin 
#define moisturePin A0 // Moisture

// Delay
#define delayCycle 86400000 // Delay per cycle, currently 1 day
#define delayInterval 900000 // Delay between intervals
#define miliMetersToOutput 50 // Add 50 milimeters to

// Motor Specifications
#define miliLiterPerSecond 20.15// Motor water speed in milimeters per second
//---------------------------------------------------------------------------------

void setup() {
	pinMode(moisturePower, OUTPUT); // Set the moisture pin to output
  pinMode(motorPin, OUTPUT); // Set the motor pin to output

	Serial.begin(9600);
}

void loop() {
	int moisture = readMoistureSensor(); // Read the moisture value

  // Compare the current moisture to the tipping point and check
  // if the moisture sensor is not in the air Add additional water 
  // if the moisture value still is low
  while(moisture >= soilWet && moisture <= air ) {
    // Power the motor
    powerMotor(miliMetersToOutput);

    // Delay for this interval 
    delay(delayInterval);
    moisture = readMoistureSensor();
  }

  // Delay for next read
  Serial.println("Wait for the next cycle");
	delay(delayCycle);
}

//  This function returns the humidity of the soil
int readMoistureSensor() {
	digitalWrite(moisturePower, HIGH);	// Turn the sensor ON
	delay(10);							// Allow power to settle

	int humidity = analogRead(moisturePin);	// Read the value form the moisture sensor
	digitalWrite(moisturePower, LOW);		// Turn the sensor OFF
	
  Serial.print("Moisture Value:");
  Serial.println(humidity);
  return humidity;							// Return analog moisture value
}

void powerMotor(int input_milimeters) {
  Serial.println("Pumping water...");
  digitalWrite(motorPin,HIGH); // Turn on the motor

  // Calculate the amount of time the motor should be on
  int miliSeconds = (int) ((input_milimeters / miliLiterPerSecond) * 1000); 
  Serial.print("Powering motor and adding: ");
  Serial.print(input_milimeters);
  Serial.println("ml to the plant area.");
  
  delay(miliSeconds); // Wait for the amount of time

  digitalWrite(motorPin,LOW); // Turn off the motor
  Serial.println ("Stopped motor...");
}
