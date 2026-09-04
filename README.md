### FlySky iBUS Interface via Arduino Mega

A robust, fail-safe C++ implementation for decoding multi-channel RC signals using an **Arduino Mega 2560** and the **FlySky FS-i6X / FS-iA10B** radio system via the serial **iBUS protocol**. 

This system forms the core control and telemetry framework for a **football shooting robot**. 

### 🏗️ System Architecture & Environment

The project leverages a dedicated hardware serial subsystem to ensure low-latency control loops while maintaining an active debugging link. 

### Environment

* **Framework:** Arduino Core (C++)
* **IDE / Toolchain:** PlatformIO (VS Code extension)
* **Primary Dependency:** bmellink/IBusBM library

### 📄 License

This repository is distributed under the open-source [MIT License](LICENSE).
