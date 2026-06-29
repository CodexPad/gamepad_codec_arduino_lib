#include "gamepad_codec_decoder.h"

#include <string.h>

#include "gamepad_codec_data_type.h"

/**
 * @file gamepad_codec_decoder.cpp
 */

namespace gamepad::codec {
Decoder::Decoder(Stream &stream) noexcept : stream_(stream), parser_(kGamepadInputStatePayloadSize) {}

const gamepad::input::Tracker &Decoder::Update() noexcept {
  input_tracker_.Tick();

  while (stream_.available() > 0) {
    parser_.Process(stream_.read());

    if (parser_.is_complete()) {
      OnFrame(parser_.data(), parser_.data_length());
    }
  }

  return input_tracker_;
}

void Decoder::OnFrame(const uint8_t *data, const size_t data_length) {
  if (data != nullptr && data[0] == static_cast<uint8_t>(DataType::kGamepadInputState) &&
      data_length == kGamepadInputStatePayloadSize) {
    input_tracker_.Update(gamepad::input::State::FromBytes(data + 1));
  }
}
}  // namespace gamepad::codec