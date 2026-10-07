

  
# 🎛️ DHT11 Monitor on ESP32
  


---

## Design Choices and Goals

I was inspired to do this project because of interesting in hardware tinkering mainly from software level.
The main goal was to create the stable, memory safe and concurrent app for DHT11 sensor working with 7-segment display.
For safely data transferring between the sensor and controller I'm using queues and tasks (via FreeRTOS).

### The library for controlling the 7-segment display was also developed by me.

https://github.com/Majkii2006/SevSegDisplayDriver



---

## Showcase


<img src="assets/1.jpg" width="49%" /> <img src="assets/2.jpg" width="49%" />

Video on Youtube -> https://youtu.be/R9kOajqdzmk

---



## Requirements

- **OS:** Linux  
  Developed and tested on Arch Linux

- **Compiler:** GCC, xtensa-esp-elf-gcc

- **Build system:** CMake

- **Other Dependencies:** ESP-IDF by Espressif,
                          7-Segment-Library Driver (https://github.com/Majkii2006/SevSegDisplayDriver)

---

## Clone and Build

```bash
git clone https://github.com/Majkii2006/DHT11-Monitoring && cd DHT-11-Monitoring
```
Then, activate the SDK from Espressif in your working directory:

```bash
. $HOME/esp-idf/esp/export.sh
```

Then configure and build the project:

```bash
idf.py build
```
Finally, you can flash and monitor the ESP Module with builded project:
```bash
idf.py flash monitor
```
---
  
## License

Licensed under the Apache License, Version 2.0.  
See the [LICENSE](LICENSE) and [NOTICE](NOTICE) files for details.
