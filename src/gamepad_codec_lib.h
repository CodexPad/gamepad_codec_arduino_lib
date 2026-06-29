#pragma once

#ifndef _GAMEPAD_CODEC_LIB_H_
#define _GAMEPAD_CODEC_LIB_H_

#include <WString.h>

/**
 * @file gamepad_codec_lib.h
 */

namespace gamepad_codec_lib {

/**
 * @~English
 * @brief Major version number.
 */
/**
 * @~Chinese
 * @brief 主版本号。
 */
constexpr uint8_t kVersionMajor = 1;

/**
 * @~English
 * @brief Minor version number.
 */
/**
 * @~Chinese
 * @brief 次版本号。
 */
constexpr uint8_t kVersionMinor = 0;

/**
 * @~English
 * @brief Patch version number.
 */
/**
 * @~Chinese
 * @brief 修订版本号。
 */
constexpr uint8_t kVersionPatch = 0;

/**
 * @~English
 * @brief Get the version number string.
 * @return The version number string in the format of major.minor.patch.
 */
/**
 * @~Chinese
 * @brief 获取版本号字符串。
 * @return 版本号字符串，格式为 major.minor.patch。
 */
String Version() { return String(kVersionMajor) + '.' + kVersionMinor + '.' + kVersionPatch; }
}  // namespace gamepad_codec_lib
#endif