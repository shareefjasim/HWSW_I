#include <Arduino.h>
 
// Define pin numbers for the voltage output signals
const int VOUT1_PIN = 1; // analog input pin 0
const int VOUT2_PIN = 2; // analog input pin 1
 
void setup() {
    // Initialize serial communication at a baud rate of 115200
    Serial.begin(115200); 
    
    // Attach the analog pins to the ADC (Analog-to-Digital Converter) for input readings
    adcAttachPin(VOUT1_PIN); // Attach VOUT1_PIN for ADC input
    adcAttachPin(VOUT2_PIN); // Attach VOUT2_PIN for ADC input
    
    // Set the ADC attenuation level to 11dB, allowing the ADC to measure voltages up to 3.3V
    analogSetAttenuation(ADC_11db);
}
 
void loop() {
    // Read the ADC values from both VOUT1_PIN and VOUT2_PIN
    int adc1 = analogRead(VOUT1_PIN); // Read ADC value from VOUT1_PIN
    int adc2 = analogRead(VOUT2_PIN); // Read ADC value from VOUT2_PIN
    
    // Convert the ADC readings to voltages (assuming 3.3V reference voltage)
    float voltage1 = (adc1 / 4095.0) * 3.3; // Convert ADC value to voltage for VOUT1_PIN
    float voltage2 = (adc2 / 4095.0) * 3.3; // Convert ADC value to voltage for VOUT2_PIN
    
    // Output the voltage readings to the Serial Monitor
    Serial.print("VOUT1: ");
    Serial.print(voltage1); // Print the voltage of VOUT1_PIN
    Serial.println(" V (Voltage)"); // Print unit label for VOUT1
    
    Serial.print("VOUT2: ");
    Serial.print(voltage2); // Print the voltage of VOUT2_PIN
    Serial.println(" V (Voltage)"); // Print unit label for VOUT2
    
    Serial.println(""); // Add an empty line for readability
    
    // Delay for 1 second before repeating the loop
    delay(1000); 
}
