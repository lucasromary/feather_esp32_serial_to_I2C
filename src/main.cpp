#include <Arduino.h>
#include <Wire.h>

int x = 0;

const unsigned int MAX_MESSAGE_LENGTH = 12;
byte array4Bytes[4] = {0, 0, 0, 0};

void setup()
{
  Serial.begin(115200);
  // Start the I2C Bus as Master

  while (Wire.begin(23, 22) == 0)
  {
    Serial.println("I2C cant begin");
    delay(500);
  }
  Serial.println("I2C begin sucess");
}
void loop()
{

  // send data only when you receive data:
  // Check to see if anything is available in the serial receive buffer
  while (Serial.available() > 0)
  {
    // Create a place to hold the incoming message
    static char message[MAX_MESSAGE_LENGTH];
    static unsigned int message_pos = 0;
    // Read the next available byte in the serial receive buffer
    char inByte = Serial.read();
    // Message coming in (check not terminating character) and guard for over message size
    if (inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1))
    {
      // Add the incoming byte to our message
      message[message_pos] = inByte;
      message_pos++;
    }
    // Full message received...
    else
    {
      // Add null character to string
      message[message_pos] = '\0';
      // Print the message (or do other things)
      // Serial.println(message);
      // Or convert to integer and print
      long number = atoi(message);
      Serial.println(number);

      // Reset for the next message
      message_pos = 0;

      // number will be 4 bytes : -2,147,483,648 to 2,147,483,647 ou 0 to 4,294,967,295
      int push = 24;
      for (int i = 0; i < 4; i++)
      {
        array4Bytes[i] = (number >> push) & 0xFF;
        push = push - 8;
        //Serial.println(array4Bytes[i]);
      }

      Wire.beginTransmission(9); // transmit to device #9
      Wire.write(array4Bytes,4);        // sends x
      Wire.endTransmission();    // stop transmitting
    }
  }
  /*
    Wire.beginTransmission(9); // transmit to device #9
    Wire.write(x);              // sends x
    Wire.endTransmission();    // stop transmitting
    x++; // Increment x
    if (x > 500) x = 0; // `reset x once it gets 6
    delay(500);
    */
}