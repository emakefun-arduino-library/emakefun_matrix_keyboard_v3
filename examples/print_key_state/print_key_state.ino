/**
 * @~Chinese
 * @file print_key_state.ino
 * @brief 示例：循环判断每个按键的状态。
 * @example print_key_state.ino
 * 循环判断每个按键的状态
 */
/**
 * @~English
 * @file print_key_state.ino
 * @brief Example: Loop to determine the status of each key.
 * @example print_key_state.ino
 * Loop to determine the status of each key.
 */

#include <Wire.h>

#include "matrix_keyboard.h"

#define PRINT_KEY_STATE(key)                                                    \
  if (g_matrix_keyboard.Pressed(emakefun::MatrixKeyboard::kKey##key)) {         \
    Serial.println(F("key " #key " pressed"));                                  \
  } else if (g_matrix_keyboard.Pressing(emakefun::MatrixKeyboard::kKey##key)) { \
    Serial.println(F("key " #key " pressing"));                                 \
  } else if (g_matrix_keyboard.Released(emakefun::MatrixKeyboard::kKey##key)) { \
    Serial.println(F("key " #key " released"));                                 \
  }

namespace {
#if defined(ESP32)
constexpr gpio_num_t kI2cPinSda = GPIO_NUM_21;
constexpr gpio_num_t kI2cPinScl = GPIO_NUM_22;
#endif

emakefun::MatrixKeyboard g_matrix_keyboard(Wire, emakefun::MatrixKeyboard::kDefaultI2cAddress);

}  // namespace

void setup() {
  Serial.begin(115200);

#if defined(ESP32)
  Wire.begin(kI2cPinSda, kI2cPinScl);
#else
  Wire.begin();
#endif

  const auto result = g_matrix_keyboard.Initialize();

  if (emakefun::MatrixKeyboard::ErrorCode::kOK == result) {
    Serial.println(F("matrix keyboard initialization successful"));
  } else {
    Serial.print(F("Error: matrix keyboard initialization failed: "));
    Serial.println(static_cast<uint32_t>(result));
    while (true);
  }
}

void loop() {
  g_matrix_keyboard.Tick();
  PRINT_KEY_STATE(0);
  PRINT_KEY_STATE(1);
  PRINT_KEY_STATE(2);
  PRINT_KEY_STATE(3);
  PRINT_KEY_STATE(4);
  PRINT_KEY_STATE(5);
  PRINT_KEY_STATE(6);
  PRINT_KEY_STATE(7);
  PRINT_KEY_STATE(8);
  PRINT_KEY_STATE(9);
  PRINT_KEY_STATE(A);
  PRINT_KEY_STATE(B);
  PRINT_KEY_STATE(C);
  PRINT_KEY_STATE(D);
  PRINT_KEY_STATE(Asterisk);
  PRINT_KEY_STATE(NumberSign);
}
