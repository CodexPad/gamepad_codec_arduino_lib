#pragma once

#ifndef _GAMEPAD_CODEC_ENCODER_H_
#define _GAMEPAD_CODEC_ENCODER_H_

#include <stddef.h>
#include <stdint.h>

#include "gamepad_input_tracker.h"
#include "robust_frame.h"

/**
 * @file gamepad_codec_encoder.h
 */

namespace gamepad::codec {
/**
 * @~English
 * @brief Maximum size (in bytes) of an encoded frame, including all overhead
 *        (framing, escaping, CRC, etc.).
 * @details This constant can be used to statically allocate a buffer large enough
 *          to hold any encoded frame. It is computed at compile time.
 */
/**
 * @~Chinese
 * @brief 编码后一帧的最大字节数（包含帧头、转义、CRC 等全部开销）。
 * @details 该常量可用于静态分配足以容纳任何编码帧的缓冲区。它在编译期计算得出。
 */
constexpr size_t kMaxFrameSize = robust_frame::MaxSerializedLength(1 + sizeof(gamepad::input::State));

/**
 * @~English
 * @brief Encode a gamepad::input::State into a robust frame.
 * @param state        The input state to encode.
 * @param output       Pointer to the output buffer. Must be at least @ref kMaxFrameSize bytes.
 * @param output_length Pointer to a size_t that receives the actual number of bytes written.
 *                      If nullptr, no length is stored (but still safe to pass).
 * @return The `output` pointer, for convenient chaining.
 * @note The encoded frame is compatible with `gamepad::codec::Decoder`.
 */
/**
 * @~Chinese
 * @brief 将 gamepad::input::State 编码为健壮帧。
 * @param state        待编码的输入状态。
 * @param output       输出缓冲区指针，必须至少为 @ref kMaxFrameSize 字节。
 * @param output_length 指向 size_t 的指针，用于接收实际写入的字节数。
 *                      若为 nullptr，则不存储长度（但仍可安全调用）。
 * @return 返回 `output` 指针，方便链式调用。
 * @note 编码后的帧与 `gamepad::codec::Decoder` 兼容。
 */
uint8_t* Encode(const gamepad::input::State& state, uint8_t* output, size_t* output_length) noexcept;

/**
 * @~English
 * @brief Encode a gamepad::input::State into a robust frame and send directly to a Stream.
 * @param state   The input state to encode.
 * @param stream  The output stream (e.g., Serial, SoftwareSerial, etc.) to write the frame to.
 * @note The encoded frame is compatible with `gamepad::codec::Decoder`.
 * @note This function is useful for sending frames directly without an intermediate buffer.
 */
/**
 * @~Chinese
 * @brief 将 gamepad::input::State 编码为健壮帧并直接发送到 Stream。
 * @param state  待编码的输入状态。
 * @param stream 输出流（如 Serial、SoftwareSerial 等），用于写入帧数据。
 * @note 编码后的帧与 `gamepad::codec::Decoder` 兼容。
 * @note 此函数适用于无需中间缓冲区，直接发送帧的场景。
 */
void Encode(const gamepad::input::State& state, Stream& stream) noexcept;
}  // namespace gamepad::codec

#endif