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

#ifndef _CLOSEDCUBE_I2C_H
#define _CLOSEDCUBE_I2C_H

#define CC_ARDUINO 1
    #include "Arduino.h"
    #include <Wire.h>
#else
#endif

#define CC_I2C_RW_DELAY_MS 5
#define CC_I2C_ERROR_NOT_DEFINED 0xFF
#define CC_I2C_CRC_ERROR 0xF0
#define CC_I2C_ERROR_REQ_INCORRECT 0xA0
#define CC_I2C_OK 0x00

namespace ClosedCube {
    namespace Driver {

        class I2CDevice {

        public:

            I2CDevice();

            I2CDevice(uint8_t address);

            void init();

            int8_t readByte();

            int16_t readWord();

            int32_t readInt();

            void readBytes(byte *buf, uint8_t size);

            void readBytes(byte *buf, uint8_t size, bool stop);

            void writeByte(int8_t value);

            void writeByte(int8_t value, bool stop);

            void writeWord(int16_t value);

            void writeWord(int16_t value, bool stop);

            void writeInt(int32_t value);

            void writeInt(int32_t value, bool stop);

            int8_t readByteFromReg(uint8_t reg, byte delay_ms);

            int8_t readByteFromReg(uint8_t reg);

            int16_t readWordFromReg(uint8_t reg, byte delay_ms);

            int16_t readWordFromReg(uint8_t reg);

            void writeByteToReg(uint8_t reg, int8_t value);

            void writeWordToReg(uint8_t reg, int16_t value);

            void address(uint8_t address) { _address = address; }

            uint8_t lastErrorCode();

        private:

            uint8_t _address;
            uint8_t _errorCode;

            void clearError();
        };

    };
};


