/*

Arduino library for ClosedCube I2C Driver (Wrapper)
version 2020.5.7

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
#if defined(CC_ARDUINO)
    _wire->begin();
#else
    _errorCode = CC_I2C_NOT_DEFINED_ERROR;
#endif
}

void ClosedCube::Driver::I2CDevice::printI2CSettings()
{
#if defined(CC_ARDUINO)
    Serial.print("ClosedCube I2C driver - Version:0x");
    Serial.print(CC_I2C_DRIVER_VERSION, HEX);
    Serial.print(" Pins:(SDA=");
    Serial.print(SDA);
    Serial.print(", SCL=");
    Serial.print(SCL);
    Serial.println(")");
#endif
}

int8_t ClosedCube::Driver::I2CDevice::readByteFromReg(uint8_t reg)
{
    return readByteFromReg(reg, CC_I2C_RW_DELAY_MS);
}

int8_t ClosedCube::Driver::I2CDevice::readByteFromReg(uint8_t reg, uint8_t delay_ms)
{
    clearError();
    writeByte(reg, false);
    if (delay_ms > 0)
    {
        delay(delay_ms);
    }
    return readByte();
}

void ClosedCube::Driver::I2CDevice::writeByteToReg(uint8_t reg, int8_t value)
{
    clearError();

#if defined(CC_ARDUINO)
    _wire->beginTransmission(_address);
    _wire->write(reg);
    _wire->write(value);
    _errorCode = _wire->endTransmission();
#else
    _errorCode = CC_I2C_NOT_DEFINED_ERROR;
#endif
}

int16_t ClosedCube::Driver::I2CDevice::readWordFromReg(uint8_t reg)
{
    return readWordFromReg(reg, CC_I2C_RW_DELAY_MS);
}

int16_t ClosedCube::Driver::I2CDevice::readWordFromReg(uint8_t reg, uint8_t delay_ms)
{
    clearError();
    writeByte(reg, false);
    if (delay_ms > 0)
    {
        delay(delay_ms);
    }
    return readWord();
}

void ClosedCube::Driver::I2CDevice::writeWordToReg(uint8_t reg, int16_t value)
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

int8_t ClosedCube::Driver::I2CDevice::readByte()
{
    clearError();

    int8_t result = 0x00;

#if defined(CC_ARDUINO)
    int n = _wire->requestFrom(_address, (uint8_t)1);
    if (n == 1)
    {
        result = _wire->read();
    }
    else
    {
        _errorCode = CC_I2C_ERROR_REQ_INCORRECT;
    }
#else
    _errorCode = CC_I2C_NOT_DEFINED_ERROR;
#endif

    return result;
}

int16_t ClosedCube::Driver::I2CDevice::readWord()
{
    clearError();

    byte msb = 0x00;
    byte lsb = 0x00;

#if defined(CC_ARDUINO)
    int n = _wire->requestFrom(_address, (uint8_t)2);
    if (n == 2)
    {
        msb = _wire->read();
        lsb = _wire->read();
    }
    else
    {
        _errorCode = CC_I2C_ERROR_REQ_INCORRECT;
    }

#else
    _errorCode = CC_I2C_NOT_DEFINED_ERROR;
#endif

    return (int16_t)(msb << 8 | (lsb & 0xFF));
}

int32_t ClosedCube::Driver::I2CDevice::readInt()
{
    clearError();

    byte buf[4];

#if defined(CC_ARDUINO)
    int n = _wire->requestFrom(_address, (uint8_t)4);
    if (n == 4)
    {
        _wire->readBytes(buf, 4);
    }
    else
    {
        _errorCode = CC_I2C_ERROR_REQ_INCORRECT;
    }

    return (int32_t)buf[0] << 24 | ((int32_t)buf[1] & 0xFF) << 16 | (buf[2] & 0xFF) << 8 | (buf[3] & 0xFF);
#else
    _errorCode = CC_I2C_NOT_DEFINED_ERROR;
#endif
}

void ClosedCube::Driver::I2CDevice::writeByte(int8_t value)
{
    writeByte(value, true);
}

void ClosedCube::Driver::I2CDevice::writeByte(int8_t value, bool stop)
{
    clearError();

#if defined(CC_ARDUINO)
    _wire->beginTransmission(_address);
    _wire->write(value);
    _errorCode = _wire->endTransmission(stop);
#else
    _errorCode = CC_I2C_NOT_DEFINED_ERROR;
#endif
}

void ClosedCube::Driver::I2CDevice::writeWord(int16_t value)
{
    writeWord(value, true);
}

void ClosedCube::Driver::I2CDevice::writeWord(int16_t value, bool stop)
{
    clearError();

#if defined(CC_ARDUINO)
    _wire->beginTransmission(_address);
    _wire->write((value >> 8) & 0xFF);
    _wire->write((value)&0xFF);
    _errorCode = _wire->endTransmission(stop);
#else
    _errorCode = CC_I2C_NOT_DEFINED_ERROR;
#endif
}

void ClosedCube::Driver::I2CDevice::writeInt(int32_t value)
{
    writeInt(value, true);
}

void ClosedCube::Driver::I2CDevice::writeInt(int32_t value, bool stop)
{
    clearError();

#if defined(CC_ARDUINO)
    _wire->beginTransmission(_address);
    _wire->write((value >> 24) & 0xFF);
    _wire->write((value >> 16) & 0xFF);
    _wire->write((value >> 8) & 0xFF);
    _wire->write((value)&0xFF);
    _errorCode = _wire->endTransmission(stop);
#else
    _errorCode = CC_I2C_NOT_DEFINED_ERROR;
#endif
}

void ClosedCube::Driver::I2CDevice::readBytes(byte *buf, uint8_t size) {
    readBytes(buf,size,true);
}

void ClosedCube::Driver::I2CDevice::readBytes(byte *buf, uint8_t size, bool stop)
{
#if defined(CC_ARDUINO)
    _wire->requestFrom(_address, size, stop);
    _wire->readBytes(buf, size);
#else
    _errorCode = CC_I2C_NOT_DEFINED_ERROR;
#endif
}

void ClosedCube::Driver::I2CDevice::writeBytes(byte *buf, uint8_t size, bool stop)
{
#if defined(CC_ARDUINO)
    _wire->beginTransmission(_address);
    uint8_t i = 0;
    for (i = 0; i < size; i++)
    {
        _wire->write(buf[i]);
    }
    _errorCode = _wire->endTransmission(stop);
#else
    _errorCode = CC_I2C_NOT_DEFINED_ERROR;
#endif
}

void ClosedCube::Driver::I2CDevice::writeBytes(byte *buf, uint8_t size)
{
    writeBytes(buf, size, true);
}

void ClosedCube::Driver::I2CDevice::clearError()
{
    _errorCode = CC_I2C_OK;
}
