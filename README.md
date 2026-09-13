# Football Robot

A robust, fail-safe C++ control and telemetry system for an **Arduino Mega 2560** using a **FlySky FS-i6X / FS-iA10B** radio system via the serial **iBUS protocol**.

The system provides:

- FlySky iBUS RC control
- Four-motor drivetrain control
- Pneumatic valve and relay control
- Compressor control
- Horn control
- Robot Signal Light (RSL) control
- Battery voltage monitoring and telemetry
- Receiver disconnect failsafe

## System Architecture & Environment

The project uses the Arduino Mega's hardware serial interfaces for low-latency RC control, telemetry, and serial communication.

### Environment

- **Microcontroller:** Arduino Mega 2560
- **Framework:** Arduino Core (C++)
- **IDE / Toolchain:** PlatformIO (VS Code extension)
- **RC System:** FlySky FS-i6X / FS-iA10B
- **RC Protocol:** Serial iBUS
- **Primary Dependency:** bmellink/IBusBM

Hardware pins and RC channel assignments are defined in `robotConfig.h`.

## 📄 License

This repository is distributed under the open-source [MIT License](LICENSE).