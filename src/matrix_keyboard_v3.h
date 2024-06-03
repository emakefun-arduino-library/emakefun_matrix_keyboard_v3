#ifndef EMAKEFUN_MATRIX_KEY_BOARD_V3_H_
#define EMAKEFUN_MATRIX_KEY_BOARD_V3_H_

#include <Wire.h>
#include <stdint.h>

#include "debouncer.h"

namespace emakefun {
/**
 * @brief 矩阵键盘
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
    kUnknownError = 7,                        /**< 7: 未知错误*/
  };

  enum Key : uint16_t {
    kKeyNone = 0,
    kKey0 = uint16_t(1) << 7,
    kKey1 = uint16_t(1) << 0,
    kKey2 = uint16_t(1) << 4,
    kKey3 = uint16_t(1) << 8,
    kKey4 = uint16_t(1) << 1,
    kKey5 = uint16_t(1) << 5,
    kKey6 = uint16_t(1) << 9,
    kKey7 = uint16_t(1) << 2,
    kKey8 = uint16_t(1) << 6,
    kKey9 = uint16_t(1) << 10,
    kKeyA = uint16_t(1) << 12,
    kKeyB = uint16_t(1) << 13,
    kKeyC = uint16_t(1) << 14,
    kKeyD = uint16_t(1) << 15,
    kKeyAsterisk = uint16_t(1) << 3,
    kKeyNumberSign = uint16_t(1) << 11,
  };

  /**
   * @brief 构造函数
   * @param [in] wire TwoWire对象，默认为Arduino Wire
   * @param [in] i2c_address 手势识别传感器I2C地址，默认值为0x39
   */
  explicit MatrixKeyboardV3(TwoWire& wire = Wire, const uint8_t i2c_address = kDefaultI2cAddress);

  /**
   * @brief 初始化
   * @return 返回值，参考 @ref ErrorCode
   */
  ErrorCode Initialize();

  void Tick();

  bool Pressed(const Key key) const;

  bool Pressing(const Key key) const;

  bool Released(const Key key) const;

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