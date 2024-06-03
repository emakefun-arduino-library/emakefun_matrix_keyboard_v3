#include "matrix_keyboard_v3.h"

namespace emakefun {

MatrixKeyboardV3::MatrixKeyboardV3(TwoWire& wire, const uint8_t i2c_address)
    : wire_(wire), i2c_address_(i2c_address), key_(kKeyNone) {
  key_.Start(10);
}

MatrixKeyboardV3::ErrorCode MatrixKeyboardV3::Initialize() {
  wire_.beginTransmission(i2c_address_);
  return static_cast<ErrorCode>(wire_.endTransmission());
}

void MatrixKeyboardV3::Tick() {
  last_key_ = key_();
  key_ = ReadKey();
}

bool MatrixKeyboardV3::Pressed(const MatrixKeyboardV3::Key key) const {
  return (last_key_ & key) == 0 && (key_() & key) != 0;
}

bool MatrixKeyboardV3::Pressing(const MatrixKeyboardV3::Key key) const {
  return (last_key_ & key) != 0 && (key_() & key) != 0;
}

bool MatrixKeyboardV3::Released(const MatrixKeyboardV3::Key key) const {
  return (last_key_ & key) != 0 && (key_() & key) == 0;
}

MatrixKeyboardV3::Key MatrixKeyboardV3::ReadKey() {
  Key key = kKeyNone;

  if (sizeof(key) != wire_.requestFrom(i2c_address_, sizeof(key))) {
    return key;
  }

  if (sizeof(key) != wire_.readBytes(reinterpret_cast<uint8_t*>(&key), sizeof(key))) {
    return key;
  }

  return key;
}
}  // namespace emakefun