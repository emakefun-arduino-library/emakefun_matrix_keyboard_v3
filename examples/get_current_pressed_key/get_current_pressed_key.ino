/**
 * @~Chinese
 * @file get_current_pressed_key.ino
 * @brief 示例：获取当前按下的按键值若有多个按键按下则返回第一个。
 * @example get_current_pressed_key.ino
 * 获取当前按下的按键值若有多个按键按下则返回第一个。
 */
/**
 * @~English
 * @file get_current_pressed_key.ino
 * @brief Example: Get the current pressed key value. If multiple keys are pressed, return the first one.
 * @example get_current_pressed_key.ino
 * Get the current pressed key value. If multiple keys are pressed, return the first one.
 */
#include <Wire.h>

#include "matrix_keyboard.h"

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

  char key_value = g_matrix_keyboard.GetCurrentPressedKey();
  if (key_value != '\0') {
    Serial.print("Current pressed key: ");
    Serial.println(key_value);
  }
}