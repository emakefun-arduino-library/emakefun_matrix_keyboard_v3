/**
 * @example print_key_state.ino
 */

#include <Wire.h>

#include "matrix_keyboard_v3.h"

#define INFINITE_LOOP_ON_FAILURE InfiniteLoopOnFailure(__FUNCTION__, __LINE__)

#define PRINT_KEY_STATE(key)                                                      \
  if (g_matrix_keyboard.Pressed(emakefun::MatrixKeyboardV3::kKey##key)) {         \
    Serial.println(F("key " #key " pressed"));                                    \
  } else if (g_matrix_keyboard.Pressing(emakefun::MatrixKeyboardV3::kKey##key)) { \
    Serial.println(F("key " #key " pressing"));                                   \
  } else if (g_matrix_keyboard.Released(emakefun::MatrixKeyboardV3::kKey##key)) { \
    Serial.println(F("key " #key " released"));                                   \
  }

namespace {

emakefun::MatrixKeyboardV3 g_matrix_keyboard;

void InfiniteLoopOnFailure(const char* function, const uint32_t line_number) {
  Serial.println(String(F("entering an infinite loop due to failure in ")) + function + F(", at line number: ") + line_number);
  while (true) {
    yield();
  }
}

}  // namespace

void setup() {
  Serial.begin(115200);
  Serial.println(F("setup"));

  Wire.begin();

  const auto ret = g_matrix_keyboard.Initialize();

  if (emakefun::MatrixKeyboardV3::kOK == ret) {
    Serial.println(F("matrix keyboard initialization successful"));
  } else {
    Serial.print(F("matrix keyboard initialization failed: "));
    Serial.println(ret);
    INFINITE_LOOP_ON_FAILURE;
  }

  Serial.println(F("setup successful"));
}

void loop() {
  g_matrix_keyboard.Tick();  // 在循环中调用

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
