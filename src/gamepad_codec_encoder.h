#pragma once

#ifndef _GAMEPAD_CODEC_ENCODER_H_
#define _GAMEPAD_CODEC_ENCODER_H_

#include <stddef.h>
#include <stdint.h>

#include "gamepad_codec_data_type.h"
#include "gamepad_input_tracker.h"
#include "robust_frame.h"

/**
 * @file gamepad_codec_encoder.h
 */

namespace gamepad::codec {
/**
 * @~English
 * @brief Encode a gamepad::input::State into a RobustFrame.
 * @param[in] state The input state to encode.
 * @return ByteBuffer containing the encoded frame. The buffer is automatically
 *         sized to accommodate the maximum possible frame length.
 * @note Use this function when you need to send the frame later or via custom
 *       transport. For direct sending via Stream, use the Stream overload.
 */
/**
 * @~Chinese
 * @brief 将 gamepad::input::State 编码为 RobustFrame 帧。
 * @param[in] state 待编码的输入状态。
 * @return 包含编码后帧数据的 ByteBuffer。缓冲区会自动分配足够的空间以容纳最大可能的帧长度。
 * @note 当需要稍后发送帧或通过自定义传输方式发送时，使用此函数。如需直接通过 Stream 发送，
 *       请使用 Stream 重载版本。
 */
ByteBuffer Encode(const gamepad::input::State &state) noexcept;

/**
 * @~English
 * @brief Encode a gamepad::input::State into a robust frame and send directly to a Stream.
 * @param[in] state The input state to encode.
 * @param[in] stream The output stream, to write the frame to.
 * @note This function is suitable for scenarios where frames are sent directly via Stream without requiring an intermediate
 *       buffer
 */
/**
 * @~Chinese
 * @brief 将 gamepad::input::State 编码为 RobustFrame 帧并直接发送到 Stream。
 * @param[in] state 待编码的输入状态。
 * @param[in] stream 输出流，用于写入帧数据。
 * @note 此函数适用于无需中间缓冲区，直接通过 Stream 发送帧的场景。
 */
void Encode(const gamepad::input::State &state, Stream &stream) noexcept;
}  // namespace gamepad::codec
#endif