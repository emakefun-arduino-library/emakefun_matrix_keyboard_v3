#ifndef EMAKEFUN_MATRIX_KEY_BOARD_V3_H_
#define EMAKEFUN_MATRIX_KEY_BOARD_V3_H_

#include <Wire.h>
#include <stdint.h>

#include "debouncer.h"

namespace emakefun {
/**
 * @brief 矩阵键盘V3
 * @class MatrixKeyboardV3
 */
class MatrixKeyboardV3 {
 public:
  /**
   * @brief 默认I2C地址
   *
   */
  static constexpr uint8_t kDefaultI2cAddress = 0x65; /**< 0x65: 默认I2C地址 */

  /**
   * @enum ErrorCode
   * @brief 错误码
   */
  enum ErrorCode : uint32_t {
    kOK = 0,                                  /**< 0：成功 */
    kI2cDataTooLongToFitInTransmitBuffer = 1, /**< 1：I2C数据太长，无法装入传输缓冲区 */
    kI2cReceivedNackOnTransmitOfAddress = 2,  /**< 2：在I2C发送地址时收到NACK */
    kI2cReceivedNackOnTransmitOfData = 3,     /**< 3：在I2C发送数据时收到NACK */
    kI2cOtherError = 4,                       /**< 4：其他I2C错误 */
    kI2cTimeout = 5,                          /**< 5：I2C通讯超时 */
    kInvalidParameter = 6,                    /**< 6：参数错误 */
    kUnknownError = 7,                        /**< 7：未知错误 */
  };

  /**
   * @enum Key
   * @brief 键值
   */
  enum Key : uint16_t {
    kKeyNone = static_cast<Key>(0),             /**< 无按键 */
    kKey0 = static_cast<Key>(1) << 7,           /**< 按键0 */
    kKey1 = static_cast<Key>(1) << 0,           /**< 按键1 */
    kKey2 = static_cast<Key>(1) << 4,           /**< 按键2 */
    kKey3 = static_cast<Key>(1) << 8,           /**< 按键3 */
    kKey4 = static_cast<Key>(1) << 1,           /**< 按键4 */
    kKey5 = static_cast<Key>(1) << 5,           /**< 按键5 */
    kKey6 = static_cast<Key>(1) << 9,           /**< 按键6 */
    kKey7 = static_cast<Key>(1) << 2,           /**< 按键7 */
    kKey8 = static_cast<Key>(1) << 6,           /**< 按键8 */
    kKey9 = static_cast<Key>(1) << 10,          /**< 按键9 */
    kKeyA = static_cast<Key>(1) << 12,          /**< 按键A */
    kKeyB = static_cast<Key>(1) << 13,          /**< 按键B */
    kKeyC = static_cast<Key>(1) << 14,          /**< 按键C */
    kKeyD = static_cast<Key>(1) << 15,          /**< 按键D */
    kKeyAsterisk = static_cast<Key>(1) << 3,    /**< 按键星号 */
    kKeyNumberSign = static_cast<Key>(1) << 11, /**< 按键井号 */
  };

  /**
   * @enum KeyState
   * @brief 按键状态
   */
  enum KeyState : uint8_t {
    kKeyStateIdle,     /**< 按键空闲 */
    kKeyStatePressed,  /**< 按键被按下*/
    kKeyStatePressing, /**< 按键被按住*/
    kKeyStateReleased, /**< 按键被弹起*/
  };

  /**
   * @brief 构造函数
   * @param [in] wire TwoWire对象，默认为Wire
   * @param [in] i2c_address 手势识别传感器I2C地址，默认值为0x65
   */
  explicit MatrixKeyboardV3(TwoWire& wire = Wire, const uint8_t i2c_address = kDefaultI2cAddress);

  /**
   * @brief 初始化
   * @return 返回值请参考 @ref ErrorCode
   */
  ErrorCode Initialize();

  /**
   * @brief 扫描按键，在函数loop中调用，每次循环先调用该函数再查询按键状态
   *
   */
  void Tick();

  /**
   * @brief 查询按键是否被按下
   *
   * @param key 按键，参考枚举 @ref Key
   * @return 返回true代表按键被按下
   */
  bool Pressed(const Key key) const;

  /**
   * @brief 查询按键是否被按住
   *
   * @param key 按键，参考枚举 @ref Key
   * @return 返回true代表按键被按住
   */
  bool Pressing(const Key key) const;

  /**
   * @brief 查询按键是否被释放
   *
   * @param key 按键，参考枚举 @ref Key
   * @return 返回true代表按键被释放
   */
  bool Released(const Key key) const;

  /**
   * @brief 查询按键是否空闲
   *
   * @param key 按键，参考枚举 @ref Key
   * @return 返回true代表按键空闲
   */
  bool Idle(const Key key) const;

  /**
   * @brief 查询按键状态
   *
   * @param key 按键，参考枚举 @ref Key
   * @return 按键状态，参考枚举 @ref KeyState
   */
  KeyState GetKeyState(const Key key) const;

 private:
  MatrixKeyboardV3(const MatrixKeyboardV3&) = delete;
  MatrixKeyboardV3& operator=(const MatrixKeyboardV3&) = delete;
  Key ReadKey();

  TwoWire& wire_ = Wire;
  const uint8_t i2c_address_ = kDefaultI2cAddress;
  Debouncer<Key> key_;
  Key last_key_ = kKeyNone;
};
}  // namespace emakefun
#endif