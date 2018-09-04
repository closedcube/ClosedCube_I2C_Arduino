/*
 *
 * This is ClosedCube I2C Drivers example shows how to work with an simple HDC1010 and HDC2010 I2C temperature and humidity sensors
 *
 */

#include <Arduino.h> // @@remove@@
#include <ClosedCube_I2C.h>

ClosedCube::Driver::I2CDevice i2c;

void printErrorCode(uint8_t errorCode); // @@remove@@

void setup() {
    Serial.begin(9600);
    Serial.println("ClosedCube I2C Driver example");

    i2c.init();

    Serial.print("Set I2C address 0x40...");
    i2c.address(0x40);
    printErrorCode(i2c.lastErrorCode());

    Serial.print("Writing 0xFC and 0xC9...");
    i2c.writeWord(0xFCC9);
    printErrorCode(i2c.lastErrorCode());

    Serial.print("Reading a byte... 0x");
    Serial.print(i2c.readByte(),HEX);
    printErrorCode(i2c.lastErrorCode());

    Serial.print("Done.");
}

void loop() {
}

void printErrorCode(uint8_t errorCode) {
    if (errorCode == 0) {
        Serial.println(" OK!");
    } else {
        Serial.print(" Error Code #");
        Serial.print(errorCode);
        Serial.print(" (hex: ");
        Serial.print(errorCode, HEX);
        Serial.println(")");
    }
}
