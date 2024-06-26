#include "matrix_keyboard_v3.h"

namespace emakefun {

MatrixKeyboardV3::MatrixKeyboardV3(TwoWire& wire, const uint8_t i2c_address)
    : wire_(wire), i2c_address_(i2c_address), key_(kKeyNone) {
}

MatrixKeyboardV3::ErrorCode MatrixKeyboardV3::Initialize() {
  ErrorCode ret = kUnknownError;
  for (uint8_t i = 0; i < 5; i++) {
    wire_.beginTransmission(i2c_address_);
    ret = static_cast<ErrorCode>(wire_.endTransmission());
    if (ret == kOK) {
      return ret;
    }
  }
  return ret;
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

bool MatrixKeyboardV3::Idle(const MatrixKeyboardV3::Key key) const {
  return (last_key_ & key) == 0 && (key_() & key) == 0;
}

MatrixKeyboardV3::KeyState MatrixKeyboardV3::GetKeyState(const Key key) const {
  if (Pressed(key)) {
    return kKeyStatePressed;
  } else if (Pressing(key)) {
    return kKeyStatePressing;
  } else if (Released(key)) {
    return kKeyStateReleased;
  }

  return kKeyStateIdle;
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