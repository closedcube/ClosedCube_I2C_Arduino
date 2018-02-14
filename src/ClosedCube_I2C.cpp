/*

Arduino library for ClosedCube I2C-driver wrapper

---

The MIT License (MIT)

Copyright (c) 2018 ClosedCube Limited

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

*/

#include "ClosedCube_I2C.h"



ClosedCube::Driver::I2CDevice::I2CDevice(uint8_t address) : _address(address)
{

}

ClosedCube::Driver::I2CDevice::I2CDevice()
{
	
}



byte ClosedCube::Driver::I2CDevice::readByteFromReg(uint8_t reg) {
	return readByteFromReg(reg, CC_I2C_RW_DELAY_MS);
}

byte ClosedCube::Driver::I2CDevice::readByteFromReg(uint8_t reg, uint8_t delay_ms)
{
	clearError();

	writeByte(reg, false);
	if (delay_ms > 0)
		delay(delay_ms);
	return readByte();
}

void ClosedCube::Driver::I2CDevice::writeByteToReg(uint8_t reg, byte value)
{
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

short ClosedCube::Driver::I2CDevice::readWordFromReg(uint8_t reg, uint8_t delay_ms)
{
	clearError();

	writeByte(reg,false);
	if (delay_ms > 0)
		delay(delay_ms);
	return readWord();
}

void ClosedCube::Driver::I2CDevice::writeWordToReg(uint8_t reg, short value)
{
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


uint8_t ClosedCube::Driver::I2CDevice::lastErrorCode()
{
	return _errorCode;
}

byte ClosedCube::Driver::I2CDevice::readByte()
{
	clearError();

	byte result = 0x00;

#if defined(CC_ARDUINO)
	int n = Wire.requestFrom(_address, (uint8_t)1);
	if (n == 1)
		result = Wire.read();
	else
		_errorCode = CC_I2C_ERROR_REQ_INCORRECT;
#else
	_errorCode = CC_I2C_NOT_DEFINED_ERROR;
#endif

	return result;
}

short ClosedCube::Driver::I2CDevice::readWord()
{
	clearError();

	byte msb = 0x00;
	byte lsb = 0x00;

#if defined(CC_ARDUINO)
	int n = Wire.requestFrom(_address, (uint8_t)2);
	if (n == 2)
	{
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


int ClosedCube::Driver::I2CDevice::readInt()
{
	clearError();

	char buf[4];

#if defined(CC_ARDUINO)
	int n = Wire.requestFrom(_address, (uint8_t)4);
	if (n == 4)
		Wire.readBytes(buf, 4);
	else
		_errorCode = CC_I2C_ERROR_REQ_INCORRECT;
#else
	_errorCode = CC_I2C_NOT_DEFINED_ERROR;
#endif

	return buf[0] << 24 | (buf[1] & 0xFF) << 16 | (buf[2] & 0xFF) << 8 | (buf[3] & 0xFF);
}


void ClosedCube::Driver::I2CDevice::writeByte(byte value)
{
	writeByte(value, true);
}

void ClosedCube::Driver::I2CDevice::writeByte(byte value, bool stop)
{
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
	writeWord(value,true);
}

void ClosedCube::Driver::I2CDevice::writeWord(short value, bool stop)
{
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

void ClosedCube::Driver::I2CDevice::writeInt(int value)
{
	writeInt(value, true);
}

void ClosedCube::Driver::I2CDevice::writeInt(int value, bool stop)
{
	clearError();
#if defined(CC_ARDUINO)
	Wire.beginTransmission(_address);
	Wire.write((value >> 24) & 0xFF);
	Wire.write((value >> 16) & 0xFF);
	Wire.write((value >> 8) & 0xFF);
	Wire.write((value) & 0xFF);
	_errorCode = Wire.endTransmission();
#else
	_errorCode = CC_I2C_NOT_DEFINED_ERROR;
#endif
}


void ClosedCube::Driver::I2CDevice::clearError()
{
	_errorCode = CC_I2C_OK;
}



