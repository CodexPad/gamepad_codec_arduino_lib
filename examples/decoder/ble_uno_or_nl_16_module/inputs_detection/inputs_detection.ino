/**
 * @~English
 * @file decoder/ble_uno_or_nl_16_module/basic_polling/basic_polling.ino
 * @example decoder/ble_uno_or_nl_16_module/basic_polling/basic_polling.ino
 * @brief The demonstration periodically prints the real-time button status and joystick movements of connected controllers
 *        using a basic polling method.
 * @details This example first sends AT commands to establish a Bluetooth connection to a target device, then continuously
 *          monitors all user inputs. It showcases the detection of three distinct button states:
 *          **pressed** (momentary press), **released** (momentary release), and **holding** (sustained press). It also
 *          monitors the analog joystick axes and prints their values when a significant change beyond a set threshold is
 *          detected, filtering out minor jitter.
 * @note The `Update()` method must be called as frequently as possible within the main loop without delays to ensure
 *       real-time responsiveness and prevent data packet loss.
 * @see gamepad::codec::Decoder::Update
 */
/**
 * @~Chinese
 * @file decoder/ble_uno_or_nl_16_module/basic_polling/basic_polling.ino
 * @example decoder/ble_uno_or_nl_16_module/basic_polling/basic_polling.ino
 * @brief 演示通过基本轮询方式定期打印已连接手柄的实时按钮状态与摇杆移动。
 * @details 本示例先通过 AT 指令与指定的蓝牙设备建立连接，然后持续监控所有用户输入。
 *          它展示了三种不同的按钮状态检测：**按下**（瞬间按下）、**释放**（瞬间释放）和**持续按住**。
 *          同时，它监控模拟摇杆轴，当检测到超过设定阈值的显著变化时打印其值，从而过滤微小抖动。
 * @note 必须在主循环中尽可能频繁地调用 `Update()` 方法，且不得添加延时，以确保实时响应性并防止数据包丢失。
 * @see gamepad::codec::Decoder::Update
 */
#include <SoftwareSerial.h>

#include "gamepad_codec_decoder.h"

/**
 * IMPORTANT:
 * This using directive is REQUIRED to directly access `Button` and `Axis`.
 * Without it, you must write the fully qualified names:
 *   gamepad::input::Button::kUp
 *   gamepad::input::Axis::kLeftStickX
 * Forgetting this line will cause compile errors when using Button or Axis.
 */
/**
 * 重要：
 * 必须使用该命名空间，否则无法直接访问 `Button` 和 `Axis`。
 * 如果没有这一行，就必须写成完整限定名：
 *   gamepad::input::Button::kUp
 *   gamepad::input::Axis::kLeftStickX
 * 忘记引入命名空间会导致编译失败。
 */
using namespace gamepad::input;

namespace {
// Debug serial port pin definitions.
// Connect external serial port tools to these pins to view debug output.
// 调试串口引脚定义。
// 将外部串口工具连接到这些引脚以查看调试输出。
constexpr uint8_t kDebugSerialRxPin = 6;  // RX pin for debug output | 调试输出接收引脚
constexpr uint8_t kDebugSerialTxPin = 5;  // TX pin for debug output | 调试输出发送引脚

// Replace with your target device's Bluetooth device address.
// 替换为目标设备的 Bluetooth device address。
const String kBluetoothDeviceAddress = "16:00:00:00:03:05";

// The serial port baud rate for communication with the Bluetooth module (please refer to the module data manual, replace with
// the corresponding baud rate for the module).
// 与蓝牙模块通信的串口波特率（请查阅模块数据手册，替换为模块对应的波特率）。
constexpr uint32_t kBluetoothModuleSerialBaudRate = 115200;

constexpr Button kAllButtons[] = {Button::kUp,        Button::kDown,   Button::kLeft,    Button::kRight, Button::kSquareX,
                                  Button::kTriangleY, Button::kCrossA, Button::kCircleB, Button::kL1,    Button::kL2,
                                  Button::kL3,        Button::kR1,     Button::kR2,      Button::kR3,    Button::kSelect,
                                  Button::kStart,     Button::kHome};

// The serial instance passed in by the decoder here is the same as the one passed in Connect().
// 此处解码器传入的串口实例，与 Connect() 中传入的为同一个串口实例。
gamepad::codec::Decoder g_decoder(Serial);

// A soft serial port used for debugging output.
// 用于调试输出的软串口。
SoftwareSerial g_debug_serial(kDebugSerialRxPin, kDebugSerialTxPin);

const String ButtonToString(Button button) {
  switch (button) {
    case Button::kUp: {
      return "Up";  // UP button | 上按钮
    }
    case Button::kDown: {
      return "Down";  // DOWN button | 下按钮
    }
    case Button::kLeft: {
      return "Left";  // LEFT button | 左按钮
    }
    case Button::kRight: {
      return "Right";  // RIGHT button | 右按钮
    }
    case Button::kSquareX: {
      return "Square(X)";  // SQUARE or X button | 方形 或者 X 按钮
    }
    case Button::kTriangleY: {
      return "Triangle(Y)";  // TRIANGLE or Y button | 三角 或者 Y 按钮
    }
    case Button::kCrossA: {
      return "Cross(A)";  //  CROSS or A button | 叉型 或者 A 按钮
    }
    case Button::kCircleB: {
      return "Circle(B)";  // CIRCLE or B button | 圆形 或者 B 按钮
    }
    case Button::kL1: {
      return "L1";  // L1 button | L1按钮
    }
    case Button::kL2: {
      return "L2";  // L2 button | L2按钮
    }
    case Button::kL3: {
      return "L3";  // L3 button | L3按钮
    }
    case Button::kR1: {
      return "R1";  // R1 button | R1按钮
    }
    case Button::kR2: {
      return "R2";  // R2 button | R2按钮
    }
    case Button::kR3: {
      return "R3";  // R3 button | R3按钮
    }
    case Button::kSelect: {
      return "Select";  // SELECT button | 选择按钮
    }
    case Button::kStart: {
      return "Start";  // START button | 开始按钮
    }
    case Button::kHome: {
      return "Home";  // HOME button | 首页按钮
    }
    default: {
      return {};  // Unknown button returns empty string | 未知按钮返回空字符串
    }
  }
}

/**
 * @~English
 * @brief Send AT commands to the Bluetooth module to establish a BLE connection.
 * @details Executes a sequence of AT commands to configure the Bluetooth module as master and connect to the target device
 *          at the specified MAC address.
 *          Command order: AT+DISCON → AT+RESET → AT+ECHO=0 → AT+ROLE=0 → AT+AUTOCON=0 → AT+CON=<mac>.
 * @param[in] bluetooth_stream Stream connected to the Bluetooth module.
 *                             Note: This must be the same Stream instance that is passed to the decoder
 *                             (e.g., Serial), as the decoder reads incoming data from this same serial port.
 * @param[in] bluetooth_device_address MAC address of the target device in format "XX:XX:XX:XX:XX:XX".
 */
/**
 * @~Chinese
 * @brief 向蓝牙模块发送 AT 指令以建立 BLE 连接。
 * @details 依次执行 AT 指令序列，将蓝牙模块配置为主机模式并连接到指定 MAC 地址的目标设备。
 *          指令顺序：AT+DISCON → AT+RESET → AT+ECHO=0 → AT+ROLE=0 → AT+AUTOCON=0 → AT+CON=<mac>。
 * @param[in] bluetooth_stream 连接蓝牙模块的 Stream 对象。
 *                             注意：此对象必须与传入解码器的 Stream 是同一个实例（如 Serial），
 *                             因为解码器正是从这同一个串口读取手柄发来的数据。
 * @param[in] bluetooth_device_address 目标设备的 MAC 地址，格式为 "XX:XX:XX:XX:XX:XX"。
 */
void Connect(Stream &bluetooth_stream, const String &bluetooth_device_address) {
  if (bluetooth_device_address.length() != 17 || bluetooth_device_address[2] != ':' || bluetooth_device_address[5] != ':' ||
      bluetooth_device_address[8] != ':' || bluetooth_device_address[11] != ':' || bluetooth_device_address[14] != ':') {
    g_debug_serial.println("Error: Invalid MAC address format. Expected: XX:XX:XX:XX:XX:XX");
    while (true);
  }

  g_debug_serial.print("Start to connect ");
  g_debug_serial.println(kBluetoothDeviceAddress);

  // The module may be in a connected state. Send the disconnection command first to ensure the module is in an unconnected
  // state.
  // 模块可能处于连接状态，先发送断开指令，确保模块是未连接状态。
  bluetooth_stream.println("AT+DISCON");
  delay(100);

  // Software reset BLE chip, clear all pairing and configuration data.
  // 软件复位蓝牙芯片，清除所有配对和配置数据。
  bluetooth_stream.println("AT+RESET");
  delay(100);

  // Close AT information echo.
  // 关闭AT信息回显。
  bluetooth_stream.println("AT+ECHO=0");
  delay(100);

  // Set the module to host mode so that it can actively connect to the BLE of the slave device.
  // 设置模块为主机模式，使其能够主动连接从机蓝牙。
  bluetooth_stream.println("AT+ROLE=0");
  delay(100);

  // Disable the module's automatic Bluetooth connection mode.
  // 关闭模块的蓝牙自动连接模式。
  bluetooth_stream.println("AT+AUTOCON=0");
  delay(100);

  // Initiate BLE connection with the slave using the specified MAC address.
  // 使用指定的MAC地址发起与从机蓝牙连接。
  bluetooth_stream.print("AT+CON=");
  bluetooth_stream.println(bluetooth_device_address);
  delay(100);

  g_debug_serial.println("Connected");
}
}  // namespace

void setup() {
  g_debug_serial.begin(115200);

  Serial.begin(kBluetoothModuleSerialBaudRate);

  Connect(Serial, kBluetoothDeviceAddress);
}

void loop() {
  // ==========================================================================
  // 🔴 CRITICAL: Call Update() as frequently as possible in loop()
  // ==========================================================================
  // • Update() processes incoming Bluetooth packets from the CodexPad
  // • Any delay(...) or long blocking code WILL cause:
  //     - Packet loss
  //     - Input lag
  //     - Unstable connection
  //
  // • For real-time control, call Update() every loop iteration
  //   without any blocking operations
  //
  // 🔴【重要】Update() 必须在 loop() 中尽可能高频调用
  // • Update() 负责处理来自 CodexPad 的蓝牙数据包
  // • 任何形式的 delay 或阻塞代码都会导致：
  //     - 数据丢失
  //     - 响应延迟
  //     - 连接不稳定
  //
  // • 实时控制应用中，必须每轮循环都调用 Update()，不可阻塞
  // ==========================================================================
  const Tracker &it = g_decoder.Update();
  // ==========================================================================
  // Tracker: Gamepad Input Snapshot & Change Engine
  // ==========================================================================
  // • Returned by Update()
  // • Maintains previous and current input snapshots
  // • Enables edge detection and delta detection
  //
  // Tracker 由 Update() 返回
  // 内部保存上一帧和当前帧的输入数据
  // 支持边沿检测和差值检测
  //
  // 📚 https://codexpad.github.io/gamepad_input_arduino_lib/
  // ==========================================================================

  // ==========================================================================
  // 🟢 Button State Change Detection (Edge-Based)
  // ==========================================================================
  // • pressed()  → button was just pressed (released → pressed)
  // • released() → button was just released (pressed → released)
  // • holding()  → button remains pressed across frames
  //
  // These APIs are *frame-differential* and rely on Update() frequency.
  // Ideal for UI navigation, action triggers, and avoiding repeat firing.
  //
  // • pressed()  → 按钮刚被按下（弹起 → 按下）
  // • released() → 按钮刚被释放（按下 → 弹起）
  // • holding()  → 按钮在两帧之间持续按下
  //
  // 这些接口是“帧间差分”的，依赖于 Update() 的高频调用
  // 非常适合 UI 导航、动作触发和防止长按连发
  // ==========================================================================
  for (auto button : kAllButtons) {
    if (it.pressed(button)) {
      g_debug_serial.print("Button ");
      g_debug_serial.print(ButtonToString(button));
      g_debug_serial.println(": pressed");
    } else if (it.released(button)) {
      g_debug_serial.print("Button ");
      g_debug_serial.print(ButtonToString(button));
      g_debug_serial.println(": released");
    } else if (it.holding(button)) {
      g_debug_serial.print("Button ");
      g_debug_serial.print(ButtonToString(button));
      g_debug_serial.println(": holding");
    }
  }

  // ==========================================================================
  // 🟢 Joystick Axis Change Detection (Threshold-Based Filtering)
  // ==========================================================================
  // • AxisChanged() detects significant changes between frames
  // • Uses a threshold to filter out noise and minor jitter
  // • Only reports movement when change ≥ threshold
  //
  // • AxisChanged() 用于检测摇杆轴值的有效变化
  // • 使用阈值过滤微小抖动和噪声
  // • 只有当变化幅度 ≥ 阈值时才视为有效移动
  // ==========================================================================
  constexpr uint8_t kAxisValueChangeThreshold = 2;

  if (it.AxisChanged(Axis::kLeftStickX, kAxisValueChangeThreshold) ||
      it.AxisChanged(Axis::kLeftStickY, kAxisValueChangeThreshold) ||
      it.AxisChanged(Axis::kRightStickX, kAxisValueChangeThreshold) ||
      it.AxisChanged(Axis::kRightStickY, kAxisValueChangeThreshold)) {
    g_debug_serial.print("L(X: ");
    g_debug_serial.print(it[Axis::kLeftStickX]);
    g_debug_serial.print(", Y: ");
    g_debug_serial.print(it[Axis::kLeftStickY]);
    g_debug_serial.print("), R(X: ");
    g_debug_serial.print(it[Axis::kRightStickX]);
    g_debug_serial.print(", Y: ");
    g_debug_serial.print(it[Axis::kRightStickY]);
    g_debug_serial.println(")");
  }
}