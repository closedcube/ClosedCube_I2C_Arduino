/*

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

#if defined(CC_ARDUINO)
ClosedCube::Driver::I2CDevice::I2CDevice(TwoWire *wire) : _wire(wire)
{
}
#endif

ClosedCube::Driver::I2CDevice::I2CDevice(uint8_t address) : _address(address)
{
}

void ClosedCube::Driver::I2CDevice::init()
{
    _wire->begin();
}

void ClosedCube::Driver::I2CDevice::printI2CSettings()
{
    Serial.print("ClosedCube I2C driver - Version:0x");
    Serial.print(CC_I2C_DRIVER_VERSION, HEX);
    Serial.print(" Pins:(SDA=");
    Serial.print(SDA);
    Serial.print(", SCL=");
    Serial.print(SCL);
    Serial.println(")");
}

uint8_t ClosedCube::Driver::I2CDevice::readByteFromReg(uint8_t reg)
{
    return readByteFromReg(reg, CC_I2C_RW_DELAY_MS);
}

uint8_t ClosedCube::Driver::I2CDevice::readByteFromReg(uint8_t reg, uint16_t delay_ms)
{
    clearError();
    writeByte(reg, false);
    if (delay_ms > 0)
    {
        delay(delay_ms);
    }
    return readByte();
}

void ClosedCube::Driver::I2CDevice::writeByteToReg(uint8_t reg, uint8_t value)
{
    clearError();

    _wire->beginTransmission(_address);
    _wire->write(reg);
    _wire->write(value);
    _errorCode = _wire->endTransmission();
}

uint16_t ClosedCube::Driver::I2CDevice::readWordFromReg(uint8_t reg)
{
    return readWordFromReg(reg, CC_I2C_RW_DELAY_MS);
}

uint16_t ClosedCube::Driver::I2CDevice::readWordFromReg(uint8_t reg, uint16_t delay_ms)
{
    clearError();
    writeByte(reg, false);
    if (delay_ms > 0)
    {
        delay(delay_ms);
    }
    return readWord();
}

void ClosedCube::Driver::I2CDevice::writeWordToReg(uint8_t reg, uint16_t value)
{
    clearError();

#if defined(CC_ARDUINO)
    _wire->beginTransmission(_address);
    _wire->write(reg);
    _wire->write((value >> 8) & 0xFF);
    _wire->write((value) && 0xFF);
    _errorCode = _wire->endTransmission();
#else
    _errorCode = CC_I2C_NOT_DEFINED_ERROR;
#endif
}

uint8_t ClosedCube::Driver::I2CDevice::lastErrorCode()
{
    return _errorCode;
}

uint8_t ClosedCube::Driver::I2CDevice::readByte()
{
    clearError();

    uint8_t result;

    _wire->requestFrom(_address, (uint8_t)1);
    
    if (_wire->available())
    {
        result = _wire->read();
    }
    else
    {
        _errorCode = CC_I2C_ERROR_REQ_INCORRECT;
    }

    return result;
}

uint16_t ClosedCube::Driver::I2CDevice::readWord()
{
    clearError();

    uint8_t msb, lsb;

    _wire->requestFrom(_address, (uint8_t)2);
    if( _wire->available())
    {
        msb = _wire->read();
        lsb = _wire->read();
    }
    else
    {
        _errorCode = CC_I2C_ERROR_REQ_INCORRECT;
    }

    return (uint16_t)(msb << 8 | (lsb & 0xFF));
}

uint32_t ClosedCube::Driver::I2CDevice::readInt()
{
    clearError();

    uint8_t buf[4];

    uint8_t len = (uint8_t)4;
    _wire->requestFrom(_address, len);
    if( _wire->available())
    {
        _wire->readBytes(buf, len);
    }
    else
    {
        _errorCode = CC_I2C_ERROR_REQ_INCORRECT;
    }

    return (uint32_t)buf[0] << 24 | ((uint32_t)buf[1] & 0xFF) << 16 | (buf[2] & 0xFF) << 8 | (buf[3] & 0xFF);
}

void ClosedCube::Driver::I2CDevice::writeByte(uint8_t value)
{
    writeByte(value, true);
}

void ClosedCube::Driver::I2CDevice::writeByte(uint8_t value, bool stop)
{
    clearError();
    _wire->beginTransmission(_address);
    _wire->write(value);
    _errorCode = _wire->endTransmission(stop);
}

void ClosedCube::Driver::I2CDevice::writeWord(uint16_t value)
{
    writeWord(value, true);
}

void ClosedCube::Driver::I2CDevice::writeWord(uint16_t value, bool stop)
{
    clearError();
    _wire->beginTransmission(_address);
    _wire->write((value >> 8) & 0xFF);
    _wire->write((value) & 0xFF);
    _errorCode = _wire->endTransmission(stop);
}

void ClosedCube::Driver::I2CDevice::writeInt(uint32_t value)
{
    writeInt(value, true);
}

void ClosedCube::Driver::I2CDevice::writeInt(uint32_t value, bool stop)
{
    clearError();
    _wire->beginTransmission(_address);
    _wire->write((value >> 24) & 0xFF);
    _wire->write((value >> 16) & 0xFF);
    _wire->write((value >> 8) & 0xFF);
    _wire->write((value)&0xFF);
    _errorCode = _wire->endTransmission(stop);
}
          
            
void ClosedCube::Driver::I2CDevice::writeS8ToReg(uint8_t reg, int8_t value) {
    writeByteToReg(reg,value);
}

int8_t ClosedCube::Driver::I2CDevice::readS8FromReg(uint8_t reg) {
    return (int8_t)readByteFromReg(reg);
}

int8_t ClosedCube::Driver::I2CDevice::readS8FromReg(uint8_t reg, uint16_t delay_ms) {
    return (int8_t)readByteFromReg(reg,delay_ms);
}

void ClosedCube::Driver::I2CDevice::readBytes(byte *buf, uint8_t size)
{
    readBytes(buf, size, true);
}

void ClosedCube::Driver::I2CDevice::readBytes(byte *buf, uint8_t size, bool stop)
{
    _wire->requestFrom(_address, size, stop);
    _wire->readBytes(buf, size);
}

void ClosedCube::Driver::I2CDevice::writeBytes(byte *buf, uint8_t size, bool stop)
{
    _wire->beginTransmission(_address);
    uint8_t i = 0;
    for (i = 0; i < size; i++)
    {
        _wire->write(buf[i]);
    }
    _errorCode = _wire->endTransmission(stop);
}

void ClosedCube::Driver::I2CDevice::writeBytes(byte *buf, uint8_t size)
{
    writeBytes(buf, size, true);
}

void ClosedCube::Driver::I2CDevice::clearError()
{
    _errorCode = CC_I2C_OK;
}
