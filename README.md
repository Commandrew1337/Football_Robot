### FlySky iBUS Interface via Arduino Mega

A robust, fail-safe C++ implementation for decoding multi-channel RC signals using an **Arduino Mega 2560** and the **FlySky FS-i6X / FS-iA10B** radio system via the serial **iBUS protocol**. 

This system forms the core control and telemetry framework for an upcoming **football shooting robot project**. 

### 🏗️ System Architecture & Environment

The project leverages a dedicated hardware serial subsystem to ensure low-latency control loops while maintaining an active debugging link. 

### Environment

* **Framework:** Arduino Core (C++)
* **IDE / Toolchain:** PlatformIO (VS Code extension)
* **Primary Dependency:** bmellink/IBusBM library

### Hardware Pinout (iBUS Serial Bridge)

By utilizing the iBUS serial protocol, a single data wire captures all transmitter channels simultaneously. Connection to the receiver is isolated entirely to the **i-BUS SERVO** column (far-right side of the FS-iA10B receiver casing). 

Hardware Component 

Source Connection (Receiver Layer) 

Target Pin (Arduino Mega) 

Function 

****Data Signal (S)****
Top Pin / Yellow Wire**Pin 19 (RX1)**Inbound Serial Frame Stream
****Power (VCC)****
Middle Pin / Red Wire**5V Pin**Shared 5V Power Rail
****Ground (GND)****
Bottom Pin / Black Wire**GND Pin**Common Ground Reference

*Note: The main USB hardware serial port (Serial0) remains entirely unencumbered. This architecture allows you to upload software configurations and read real-time diagnostics without disconnecting the radio subsystem.* 

### ⚙️ Core Configuration Requirements

Before deploying the codebase, the handheld transmitter must be configured to establish the proper communication parameters: 

1. **Output Mode:** Set the transmitter system properties to output via **i-BUS** (System Setup > RX Setup > Output Mode).
2. **Channel Mapping:** Map auxiliary toggle switches (such as the 3-way SwC switch) to available software channels (Functions Setup > Aux. Channels).
3. **Failsafe Protocol:** Turn all internal transmitter failsafes **OFF**. This ensures the data stream drops to absolute zero when communication breaks, allowing the Arduino firmware to natively detect signal loss and execute a safe emergency stop.

### 📄 License

This repository is distributed under the open-source [MIT License](LICENSE).
