# STM32_TimerInterrupt Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/STM32_TimerInterrupt.svg?)](https://www.ardu-badge.com/STM32_TimerInterrupt)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/STM32_TimerInterrupt.svg)](https://github.com/khoih-prog/STM32_TimerInterrupt/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/STM32_TimerInterrupt/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/STM32_TimerInterrupt.svg)](http://github.com/khoih-prog/STM32_TimerInterrupt/issues)

---
---


* [Changelog](#changelog)
  * [Releases v1.3.0](#releases-v130)
  * [Releases v1.2.1](#releases-v121)
  * [Releases v1.2.0](#releases-v120)
  * [Releases v1.1.1](#releases-v111)
  * [Releases v1.0.1](#releases-v101)
  * [Releases v1.0.0](#releases-v100)

---
---


## Changelog

### Releases v1.3.0

1. Fix `multiple-definitions` linker error. Drop `src_cpp` and `src_h` directories
2. Fix reattachInterrupt() bug. Check [bugfix: reattachInterrupt() pass wrong frequency value to setFrequency() #19](https://github.com/khoih-prog/ESP8266TimerInterrupt/pull/19)
3. Update `Packages_Patches`

### Releases v1.2.1

1. Add support to **STM32L5 (NUCLEO_L552ZE_Q)**
2. Verify OK with **STM32H7 (NUCLEO_H743ZI2)**

### Releases v1.2.0

1. Add better debug feature.
2. Optimize code and examples to reduce RAM usage
3. Add Table of Contents

### Releases v1.1.1

1. Add example [**Change_Interval**](examples/Change_Interval) and [**ISR_16_Timers_Array_Complex**](examples/ISR_16_Timers_Array_Complex)
2. Bump up version to sync with other TimerInterrupt Libraries. Modify Version String.


### Releases v1.0.1

1. Add complicated example [ISR_16_Timers_Array](examples/ISR_16_Timers_Array) utilizing and demonstrating the full usage of 16 independent ISR Timers.

### Releases v1.0.0

1. Permit up to 16 super-long-time, super-accurate ISR-based timers to avoid being blocked
2. Using cpp code besides Impl.h code to use if Multiple-Definition linker error.

