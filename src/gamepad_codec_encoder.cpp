#include "gamepad_codec_encoder.h"

#include <string.h>

/**
 * @file gamepad_codec_encoder.cpp
 */

namespace gamepad::codec {
uint8_t *Encode(const gamepad::input::State &state, uint8_t *output, size_t *output_length) noexcept {
  if (output == nullptr || output_length == nullptr) {
    return output;
  }

  uint8_t data[kDataTypeGamepadInputStateSize] = {0};
  data[0] = static_cast<uint8_t>(DataType::kGamepadInputState);
  memcpy(&data[1], &state, sizeof(state));

  robust_frame::Serialize(data, sizeof(data), output, output_length);

  return output;
}

void Encode(const gamepad::input::State &state, Stream &stream) noexcept {
  uint8_t data[kDataTypeGamepadInputStateSize] = {0};
  data[0] = static_cast<uint8_t>(DataType::kGamepadInputState);
  memcpy(&data[1], &state, sizeof(state));

  robust_frame::Serialize(data, sizeof(data), stream);
}

}  // namespace gamepad::codec