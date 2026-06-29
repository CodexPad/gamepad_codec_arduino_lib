#pragma once

#ifndef _GAMEPAD_CODEC_DECODER_H_
#define _GAMEPAD_CODEC_DECODER_H_

#include <Stream.h>
#include <stdint.h>

#include "gamepad_input_tracker.h"
#include "robust_frame.h"

/**
 * @file gamepad_codec_decoder.h
 */

namespace gamepad::codec {

/**
 * @~English
 * @brief Decoder class responsible for parsing gamepad input frames from a byte stream and updating the state tracker.
 * @details Binds to a Stream (e.g., Serial) object. When Update() is called, it reads all available data, uses
 *          RobustFrame::Parser to synchronize frame boundaries, identifies the payload type (0x01), deserializes data into
 *          gamepad::input::State, and finally updates the internal Tracker.
 */
/**
 * @~Chinese
 * @brief 解码器类，负责从字节流中解析手柄输入帧并更新状态追踪器。
 * @details 该类绑定一个 Stream（如 Serial）对象，在调用 Update()时会读取流中所有可用数据，通过 RobustFrame::Parser
 *          同步帧边界，识别有效载荷类型，并将数据反序列化为 gamepad::input::State，最终更新内部 Tracker。
 */
class Decoder {
 public:
  /**
   * @~English
   * @brief Constructor.
   * @param[in] stream Reference to a Stream object for reading data byte stream.
   */
  /**
   * @~Chinese
   * @brief 构造函数。
   * @param[in] stream 用于读取数据字节流的 Stream 对象引用。
   */
  Decoder(Stream &stream) noexcept;

  /**
   * @~English
   * @brief Update, need to be called in Loop
   */
  /**
   * @~Chinese
   * @brief 更新，需要在Loop中不断调用
   */
  const gamepad::input::Tracker &Update() noexcept;

  /**
   * @~English
   * @brief Get the input tracker object
   * @details This function returns a reference to the internal input tracker object.
   * @return Input tracker object
   */
  /**
   * @~Chinese
   * @brief 获取输入跟踪器对象
   * @details 此函数返回内部输入跟踪器对象的引用。
   * @return 输入跟踪器对象
   */
  const gamepad::input::Tracker &input_tracker() const noexcept { return input_tracker_; }

 private:
  Decoder(const Decoder &) = delete;
  Decoder &operator=(const Decoder &) = delete;

  void OnFrame(const uint8_t *data, size_t data_length);

  robust_frame::Parser parser_;
  gamepad::input::Tracker input_tracker_;

  Stream &stream_;
};

}  // namespace gamepad::codec

#endif