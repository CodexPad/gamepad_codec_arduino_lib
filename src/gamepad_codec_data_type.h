#pragma once

#ifndef _GAMEPAD_CODEC_DATA_TYPE_H_
#define _GAMEPAD_CODEC_DATA_TYPE_H_

#include <stdint.h>

#include "gamepad_input_tracker.h"

/**
 * @file gamepad_codec_data_type.h
 */

namespace gamepad::codec {
/**
 * @~English
 * @brief Payload size for DataType::kGamepadInputState.
 * @details 1 byte type identifier + sizeof(gamepad::input::State).
 *          Use this constant when encoding/decoding this specific data type.
 */
/**
 * @~Chinese
 * @brief DataType::kGamepadInputState 对应的有效载荷大小。
 * @details 1 字节类型标识符 + sizeof(gamepad::input::State)。
 *          编码/解码此特定数据类型时使用此常量。
 */
#if __cplusplus >= 201703L
constexpr inline size_t kGamepadInputStatePayloadSize = sizeof(gamepad::input::State) + 1;
#else
static constexpr size_t kGamepadInputStatePayloadSize = sizeof(gamepad::input::State) + 1;
#endif

/**
 * @~English
 * @brief Data type identifiers for the payload.
 * @details The first byte of the payload identifies the type of data being
 * transmitted.
 */
/**
 * @~Chinese
 * @brief 有效载荷的数据类型标识符。
 * @details 有效载荷的第一个字节标识所传输数据的类型。
 */
enum class DataType : uint8_t {
  /**
   * @~English
   * @brief Gamepad input state (buttons + axes).
   */
  /**
   * @~Chinese
   * @brief 手柄输入状态（按钮 + 摇杆）。
   */
  kGamepadInputState = 0x01,
};

}  // namespace gamepad::codec

#endif