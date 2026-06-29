#include "gamepad_codec_encoder.h"

#include <string.h>

/**
 * @file gamepad_codec_encoder.cpp
 */

namespace gamepad::codec {
ByteBuffer Encode(const gamepad::input::State &state) noexcept {
  uint8_t data[kGamepadInputStatePayloadSize] = {0};
  data[0] = static_cast<uint8_t>(DataType::kGamepadInputState);
  memcpy(&data[1], &state, sizeof(state));

  return robust_frame::Serialize(data, sizeof(data));
}

void Encode(const gamepad::input::State &state, Stream &stream) noexcept {
  uint8_t data[kGamepadInputStatePayloadSize] = {0};
  data[0] = static_cast<uint8_t>(DataType::kGamepadInputState);
  memcpy(&data[1], &state, sizeof(state));

  robust_frame::Serialize(data, sizeof(data), stream);
}

}  // namespace gamepad::codec