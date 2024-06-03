/**
 * @example print_key.ino
 */

#include <Wire.h>

#include "matrix_keyboard_v3.h"

#define INFINITE_LOOP_ON_FAILURE InfiniteLoopOnFailure(__FUNCTION__, __LINE__)

#define PRINT_KEY(key)                                                            \
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
  g_matrix_keyboard.Update();

  PRINT_KEY(0);
  PRINT_KEY(1);
  PRINT_KEY(2);
  PRINT_KEY(3);
  PRINT_KEY(4);
  PRINT_KEY(5);
  PRINT_KEY(6);
  PRINT_KEY(7);
  PRINT_KEY(8);
  PRINT_KEY(9);
  PRINT_KEY(A);
  PRINT_KEY(B);
  PRINT_KEY(C);
  PRINT_KEY(D);
  PRINT_KEY(Asterisk);
  PRINT_KEY(NumberSign);
}
