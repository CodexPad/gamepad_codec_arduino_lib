#include "gamepad_codec_encoder.h"

#include <string.h>

#include "robust_frame.h"

/**
 * @file gamepad_codec_encoder.cpp
 */

namespace {
constexpr uint8_t kPayloadType = 0x01;
}  // namespace

namespace gamepad::codec {
uint8_t* Encode(const gamepad::input::State& state, uint8_t* output, size_t* output_length) noexcept {
  if (output == nullptr || output_length == nullptr) {
    return output;
  }

  uint8_t payload[1 + sizeof(gamepad::input::State)] = {0};
  payload[0] = kPayloadType;
  memcpy(&payload[1], &state, sizeof(state));

  robust_frame::Serialize(payload, 1 + sizeof(gamepad::input::State), output, output_length);

  return output;
}

void Encode(const gamepad::input::State& state, Stream& stream) noexcept {
  uint8_t payload[1 + sizeof(gamepad::input::State)] = {0};
  payload[0] = kPayloadType;
  memcpy(&payload[1], &state, sizeof(state));

  robust_frame::Serialize(payload, 1 + sizeof(gamepad::input::State), stream);
}

}  // namespace gamepad::codec