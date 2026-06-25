#include "gamepad_codec_decoder.h"

#include <string.h>

/**
 * @file gamepad_codec_decoder.cpp
 */

namespace gamepad::codec {
namespace {
constexpr uint8_t kPayloadType = 0x01;
}

Decoder::Decoder(Stream& stream) noexcept : stream_(stream), parser_(sizeof(input::State) + 1) {}

const gamepad::input::Tracker& Decoder::Update() noexcept {
  input_tracker_.Tick();

  while (stream_.available() > 0) {
    parser_.Process(stream_.read());

    if (parser_.is_complete()) {
      OnFrame(parser_.data());
      parser_.Reset();
    }
  }

  return input_tracker_;
}

void Decoder::OnFrame(const uint8_t* data) {
  if (data != nullptr && data[0] == kPayloadType) {
    input_tracker_.Update(gamepad::input::State::FromBytes(data + 1));
  }
}
}  // namespace gamepad::codec