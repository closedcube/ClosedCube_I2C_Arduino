/*
 *  Copyright (c) 2018 ClosedCube Limited.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of the copyright holder nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 */

#include "ClosedCube_I2C.h"

ClosedCube::Driver::I2CDevice::I2CDevice(uint8_t address) : _address(address) {

}

ClosedCube::Driver::I2CDevice::I2CDevice() {

}


byte ClosedCube::Driver::I2CDevice::readByteFromReg(uint8_t reg) {
    return readByteFromReg(reg, CC_I2C_RW_DELAY_MS);
}

byte ClosedCube::Driver::I2CDevice::readByteFromReg(uint8_t reg, uint8_t delay_ms) {
    clearError();

    writeByte(reg, false);
    if (delay_ms > 0)
        delay(delay_ms);
    return readByte();
}

void ClosedCube::Driver::I2CDevice::writeByteToReg(uint8_t reg, byte value) {
    clearError();

#if defined(CC_ARDUINO)
    Wire.beginTransmission(_address);
    Wire.write(reg);
    Wire.write(value);
    _errorCode = Wire.endTransmission();
#else
    _errorCode = CC_I2C_NOT_DEFINED_ERROR;
#endif
}

short ClosedCube::Driver::I2CDevice::readWordFromReg(uint8_t reg) {
    return readWordFromReg(reg, CC_I2C_RW_DELAY_MS);
}

short ClosedCube::Driver::I2CDevice::readWordFromReg(uint8_t reg, uint8_t delay_ms) {
    clearError();

    writeByte(reg, false);
    if (delay_ms > 0)
        delay(delay_ms);
    return readWord();
}

void ClosedCube::Driver::I2CDevice::writeWordToReg(uint8_t reg, short value) {
    clearError();

#if defined(CC_ARDUINO)
    Wire.beginTransmission(_address);
    Wire.write(reg);
    Wire.write((value >> 8) & 0xFF);
    Wire.write((value) && 0xFF);
    _errorCode = Wire.endTransmission();
#else
    _errorCode = CC_I2C_NOT_DEFINED_ERROR;
#endif
}


uint8_t ClosedCube::Driver::I2CDevice::lastErrorCode() {
    return _errorCode;
}

byte ClosedCube::Driver::I2CDevice::readByte() {
    clearError();

    byte result = 0x00;

#if defined(CC_ARDUINO)
    int n = Wire.requestFrom(_address, (uint8_t) 1);
    if (n == 1)
        result = Wire.read();
    else
        _errorCode = CC_I2C_ERROR_REQ_INCORRECT;
#else
    _errorCode = CC_I2C_NOT_DEFINED_ERROR;
#endif

    return result;
}

short ClosedCube::Driver::I2CDevice::readWord() {
    clearError();

    byte msb = 0x00;
    byte lsb = 0x00;

#if defined(CC_ARDUINO)
    int n = Wire.requestFrom(_address, (uint8_t) 2);
    if (n == 2) {
        msb = Wire.read();
        lsb = Wire.read();
    } else {
        _errorCode = CC_I2C_ERROR_REQ_INCORRECT;
    }

#else
    _errorCode = CC_I2C_NOT_DEFINED_ERROR;
#endif

    return msb << 8 | (lsb & 0xFF);
}


int ClosedCube::Driver::I2CDevice::readInt() {
    clearError();

    char buf[4];

#if defined(CC_ARDUINO)
    int n = Wire.requestFrom(_address, (uint8_t) 4);
    if (n == 4)
        Wire.readBytes(buf, 4);
    else
        _errorCode = CC_I2C_ERROR_REQ_INCORRECT;
#else
    _errorCode = CC_I2C_NOT_DEFINED_ERROR;
#endif

    return buf[0] << 24 | (buf[1] & 0xFF) << 16 | (buf[2] & 0xFF) << 8 | (buf[3] & 0xFF);
}


void ClosedCube::Driver::I2CDevice::writeByte(byte value) {
    writeByte(value, true);
}

void ClosedCube::Driver::I2CDevice::writeByte(byte value, bool stop) {
    clearError();

#if defined(CC_ARDUINO)
    Wire.beginTransmission(_address);
    Wire.write(value);
    _errorCode = Wire.endTransmission(stop);
#else
    _errorCode = CC_I2C_NOT_DEFINED_ERROR;
#endif
}

void ClosedCube::Driver::I2CDevice::writeWord(short value) {
    writeWord(value, true);
}

void ClosedCube::Driver::I2CDevice::writeWord(short value, bool stop) {
    clearError();
#if defined(CC_ARDUINO)
    Wire.beginTransmission(_address);
    Wire.write((value >> 8) & 0xFF);
    Wire.write((value) & 0xFF);
    _errorCode = Wire.endTransmission(stop);
#else
    _errorCode = CC_I2C_NOT_DEFINED_ERROR;
#endif
}

void ClosedCube::Driver::I2CDevice::writeInt(int value) {
    writeInt(value, true);
}

void ClosedCube::Driver::I2CDevice::writeInt(int value, bool stop) {
    clearError();
#if defined(CC_ARDUINO)
    Wire.beginTransmission(_address);
    Wire.write((value >> 24) & 0xFF);
    Wire.write((value >> 16) & 0xFF);
    Wire.write((value >> 8) & 0xFF);
    Wire.write((value) & 0xFF);
    _errorCode = Wire.endTransmission(stop);
#else
    _errorCode = CC_I2C_NOT_DEFINED_ERROR;
#endif
}

void ClosedCube::Driver::I2CDevice::readBytes(char *buf, uint8_t size) {
    readBytes(buf,size,true);
}

void ClosedCube::Driver::I2CDevice::readBytes(char *buf, uint8_t size, bool stop) {
    Wire.beginTransmission(_address);
    Wire.readBytes(buf,size);
    Wire.endTransmission(stop);
}

void ClosedCube::Driver::I2CDevice::clearError() {
    _errorCode = CC_I2C_OK;
}



