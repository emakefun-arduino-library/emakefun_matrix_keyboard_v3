/*
 * MIT License
 *
 * Copyright (c) 2023 emakefun
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "matrix_keyboard.h"

#include <Wire.h>
#include <arduino.h>

#define SETTING_REGISTER_START_BYTE (0xB0)
#define TOUCH_KEY_STATUS_DATA_REGISTER (0x08)
#define KEY_TRIGGER_THRESHOLD_VALUE (0x3F)
#define KEY_STATUS_STRUCTURE_SIZE (2)

#define WIRE_TIMEOUT_US (3000)

#define LOG(...) (void(0))  // for debug

namespace {
const MatrixKeyboard::Key g_keys[] = {
    MatrixKeyboard::kKeyNone,
    MatrixKeyboard::kKey1,
    MatrixKeyboard::kKey2,
    MatrixKeyboard::kKey3,
    MatrixKeyboard::kKeyA,
    MatrixKeyboard::kKey4,
    MatrixKeyboard::kKey5,
    MatrixKeyboard::kKey6,
    MatrixKeyboard::kKeyB,
    MatrixKeyboard::kKey7,
    MatrixKeyboard::kKey8,
    MatrixKeyboard::kKey9,
    MatrixKeyboard::kKeyC,
    MatrixKeyboard::kKeyAsterisk,
    MatrixKeyboard::kKey0,
    MatrixKeyboard::kKeyPound,
    MatrixKeyboard::kKeyD,
};
}  // namespace

MatrixKeyboard::MatrixKeyboard(const uint8_t device_i2c_address) : device_i2c_address_(device_i2c_address) {
}

bool MatrixKeyboard::Setup() {
  const uint8_t setting_bytes[] = {
      0x00,  // Option1
      0x00,  // Reserve
      0x83,  // Reserve
      0xF3,  // Reserve
      0x98,  // Option2

      KEY_TRIGGER_THRESHOLD_VALUE,  // Key1 Trigger threshold value
      KEY_TRIGGER_THRESHOLD_VALUE,  // Key2 Trigger threshold value
      KEY_TRIGGER_THRESHOLD_VALUE,  // Key3 Trigger threshold value
      KEY_TRIGGER_THRESHOLD_VALUE,  // Key4 Trigger threshold value

      KEY_TRIGGER_THRESHOLD_VALUE,  // Key5 Trigger threshold value
      KEY_TRIGGER_THRESHOLD_VALUE,  // Key6 Trigger threshold value
      KEY_TRIGGER_THRESHOLD_VALUE,  // Key7 Trigger threshold value
      KEY_TRIGGER_THRESHOLD_VALUE,  // Key8 Trigger threshold value

      KEY_TRIGGER_THRESHOLD_VALUE,  // Key9 Trigger threshold value
      KEY_TRIGGER_THRESHOLD_VALUE,  // Key10 Trigger threshold value
      KEY_TRIGGER_THRESHOLD_VALUE,  // Key11 Trigger threshold value
      KEY_TRIGGER_THRESHOLD_VALUE,  // Key12 Trigger threshold value

      KEY_TRIGGER_THRESHOLD_VALUE,  // Key13 Trigger threshold value
      KEY_TRIGGER_THRESHOLD_VALUE,  // Key14 Trigger threshold value
      KEY_TRIGGER_THRESHOLD_VALUE,  // Key15 Trigger threshold value
      KEY_TRIGGER_THRESHOLD_VALUE,  // Key16 Trigger threshold value
  };

  Wire.begin();
  Wire.beginTransmission(device_i2c_address_);
  Wire.write(SETTING_REGISTER_START_BYTE);
  uint8_t checksum = 0;
  for (auto i = 0; i < sizeof(setting_bytes); i++) {
    checksum += setting_bytes[i];
    if (1 != Wire.write(setting_bytes[i])) {
      LOG("Wire.write failed");
      return false;
    }
    delay(20);
  }

  if (1 != Wire.write(checksum)) {
    LOG("Wire.write failed");
    return false;
  }

  if (0 != Wire.endTransmission()) {
    LOG("Wire.endTransmission failed");
    return false;
  }

  return true;
}

char MatrixKeyboard::GetTouchedKey() {
  Wire.setWireTimeout(WIRE_TIMEOUT_US, true);
  Wire.clearWireTimeoutFlag();
  Wire.beginTransmission(device_i2c_address_);
  if (1 != Wire.write(TOUCH_KEY_STATUS_DATA_REGISTER)) {
    LOG("Wire.write failed.");
    return '\0';
  }

  if (0 != Wire.endTransmission()) {
    LOG("Wire.endTransmission failed");
    return '\0';
  }

  if (0 ==
      Wire.requestFrom(device_i2c_address_, static_cast<uint8_t>(KEY_STATUS_STRUCTURE_SIZE), static_cast<uint8_t>(false))) {
    return '\0';
  }

  uint8_t key_status = 0;
  uint8_t key = 0;

  while (Wire.available()) {
    key_status = Wire.read();
    if (key_status != 0) {
      while ((key_status & 0x1) == 0) {
        key_status >>= 1;
        key++;
      }
      return g_keys[key + 1];
    }

    key += 8;
  }

  return '\0';
}