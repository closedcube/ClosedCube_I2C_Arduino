/*

Example: i2cdemo

Arduino library for ClosedCube I2C Driver (Wrapper)
version 2020.9.8

---

Copyright (c) 2018-2020, ClosedCube
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, 
   this list of conditions and the following disclaimer in the documentation and/or 
   other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors 
   may be used to endorse or promote products derived from this software
   without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "ClosedCube_I2C.h"

ClosedCube::Driver::I2CDevice i2c;

void setup() {
    Serial.begin(9600);
    Serial.println("ClosedCube I2C Driver Demo");

    i2c.init();
    i2c.printI2CSettings();

    Serial.print("Setting I2C address 0x40...");
    i2c.address(0x40);
    printErrorCode(i2c.lastErrorCode());

    Serial.print("Writing 0xFCC9...");
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

