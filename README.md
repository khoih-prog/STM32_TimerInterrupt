# STM32_TimerInterrupt Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/STM32_TimerInterrupt.svg?)](https://www.ardu-badge.com/STM32_TimerInterrupt)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/STM32_TimerInterrupt.svg)](https://github.com/khoih-prog/STM32_TimerInterrupt/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/STM32_TimerInterrupt/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/STM32_TimerInterrupt.svg)](http://github.com/khoih-prog/STM32_TimerInterrupt/issues)

---
---

## Features

This library enables you to use Interrupt from Hardware Timers on an STM32-based board.

### Why do we need this Hardware Timer Interrupt?

Imagine you have a system with a **mission-critical** function, measuring water level and control the sump pump or doing something much more important. You normally use a software timer to poll, or even place the function in loop(). But what if another function is **blocking** the loop() or setup().

So your function **might not be executed, and the result would be disastrous.**

You'd prefer to have your function called, no matter what happening with other functions (busy loop, bug, etc.).

The correct choice is to use a Hardware Timer with **Interrupt** to call your function.

These hardware timers, using interrupt, still work even if other functions are blocking. Moreover, they are much more **precise** (certainly depending on clock frequency accuracy) than other software timers using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

Functions using normal software timers, relying on loop() and calling millis(), won't work if the loop() or setup() is blocked by certain operation. For example, certain function is blocking while it's connecting to WiFi or some services.

The catch is **your function is now part of an ISR (Interrupt Service Routine), and must be lean / mean, and follow certain rules.** More to read on:

[**HOWTO Attach Interrupt**](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)

---

#### Important Notes:

1. Inside the attached function, **delay() won’t work and the value returned by millis() will not increment.** Serial data received while in the function may be lost. You should declare as **volatile any variables that you modify within the attached function.**

2. Typically global variables are used to pass data between an ISR and the main program. To make sure variables shared between an ISR and the main program are updated correctly, declare them as volatile.

---
---

### Releases v1.0.0

1. Permit up to 16 super-long-time, super-accurate ISR-based timers to avoid being blocked
2. Using cpp code besides Impl.h code to use if Multiple-Definition linker error.

#### Supported Boards

1. **STM32 boards with built-in Ethernet LAN8742A** such as :

  - **Nucleo-144 (F429ZI, F767ZI)**
  - **Discovery (STM32F746G-DISCOVERY)**
  - **All STM32 boards (STM32F/L/H/G/WB/MP1) with 32K+ Flash, with Built-in Ethernet**
  
2. **STM32F/L/H/G/WB/MP1 boards (with 32+K Flash) running W5x00 or ENC28J60 shields)**

- Nucleo-144
- Nucleo-64
- Discovery
- Generic STM32F0, STM32F1, STM32F2, STM32F3, STM32F4, STM32F7 (with 64+K Flash): x8 and up
- STM32L0, STM32L1, STM32L4
- STM32G0, STM32G4
- STM32H7
- STM32WB
- STM32MP1
- LoRa boards
- 3-D printer boards
- Generic Flight Controllers
- Midatronics boards

---
---

## Prerequisite

 1. [`Arduino IDE 1.8.13+` for Arduino](https://www.arduino.cc/en/Main/Software)
 2. [`Arduino Core for STM32 1.9.0+`](https://github.com/stm32duino/Arduino_Core_STM32) for STM32 (Use Arduino Board Manager)
 3. [`Blynk library 0.6.1+`](https://github.com/blynkkk/blynk-library) to use with certain example.
 4. To use with certain example, depending on which Ethernet card you're using:
   - [`STM32Ethernet library v1.2.0+`](https://github.com/stm32duino/STM32Ethernet) for built-in Ethernet LAN8742A on (Nucleo-144, Discovery). To be used with [`STM32duino_LwIP library v2.1.2+`](https://github.com/stm32duino/LwIP). 
   - [`Ethernet library v2.0.0+`](https://www.arduino.cc/en/Reference/Ethernet) for W5100, W5200 and W5500.
   - [`Ethernet2 library v1.0.4+`](https://github.com/khoih-prog/Ethernet2) for W5500 (Deprecated, use Arduino Ethernet library).
   - [`Ethernet3 library v1.5.3+`](https://github.com/sstaub/Ethernet3) for W5500/WIZ550io/WIZ850io/USR-ES1 with Wiznet W5500 chip.
   - [`EthernetLarge library v2.0.0+`](https://github.com/OPEnSLab-OSU/EthernetLarge) for W5100, W5200 and W5500. ***Ready*** from v1.0.1.
   - [`UIPEthernet library v2.0.9+`](https://github.com/UIPEthernet/UIPEthernet) for ENC28J60.

---
---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for [**STM32_TimerInterrupt**](https://github.com/khoih-prog/STM32_TimerInterrupt), then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/STM32_TimerInterrupt.svg?)](https://www.ardu-badge.com/STM32_TimerInterrupt) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [**STM32_TimerInterrupt**](https://github.com/khoih-prog/STM32_TimerInterrupt) page.
2. Download the latest release `STM32_TimerInterrupt-master.zip`.
3. Extract the zip file to `STM32_TimerInterrupt-master` directory 
4. Copy whole `STM32_TimerInterrupt-master` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**STM32_TimerInterrupt** library](https://platformio.org/lib/show/11381/STM32_TimerInterrupt) by using [Library Manager](https://platformio.org/lib/show/11381/STM32_TimerInterrupt/installation). Search for **STM32_TimerInterrupt** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)

---
---

### Libraries' Patches

1. If your application requires 2K+ HTML page, the current [`Ethernet library`](https://www.arduino.cc/en/Reference/Ethernet) must be modified if you are using W5200/W5500 Ethernet shields. W5100 is not supported for 2K+ buffer. If you use boards requiring different CS/SS pin for W5x00 Ethernet shield, for example ESP32, ESP8266, nRF52, etc., you also have to modify the following libraries to be able to specify the CS/SS pin correctly.

2. To fix [`Ethernet library`](https://www.arduino.cc/en/Reference/Ethernet), just copy these following files into the [`Ethernet library`](https://www.arduino.cc/en/Reference/Ethernet) directory to overwrite the old files:
- [Ethernet.h](LibraryPatches/Ethernet/src/Ethernet.h)
- [Ethernet.cpp](LibraryPatches/Ethernet/src/Ethernet.cpp)
- [EthernetServer.cpp](LibraryPatches/Ethernet/src/EthernetServer.cpp)
- [w5100.h](LibraryPatches/Ethernet/src/utility/w5100.h)
- [w5100.cpp](LibraryPatches/Ethernet/src/utility/w5100.cpp)

3. To fix [`EthernetLarge library`](https://github.com/OPEnSLab-OSU/EthernetLarge), just copy these following files into the [`EthernetLarge library`](https://github.com/OPEnSLab-OSU/EthernetLarge) directory to overwrite the old files:
- [EthernetLarge.h](LibraryPatches/EthernetLarge/src/EthernetLarge.h)
- [EthernetLarge.cpp](LibraryPatches/EthernetLarge/src/EthernetLarge.cpp)
- [EthernetServer.cpp](LibraryPatches/EthernetLarge/src/EthernetServer.cpp)
- [w5100.h](LibraryPatches/EthernetLarge/src/utility/w5100.h)
- [w5100.cpp](LibraryPatches/EthernetLarge/src/utility/w5100.cpp)

4. To fix [`Ethernet2 library`](https://github.com/khoih-prog/Ethernet2), just copy these following files into the [`Ethernet2 library`](https://github.com/khoih-prog/Ethernet2) directory to overwrite the old files:

- [Ethernet2.h](LibraryPatches/Ethernet2/src/Ethernet2.h)
- [Ethernet2.cpp](LibraryPatches/Ethernet2/src/Ethernet2.cpp)

To add UDP Multicast support, necessary for this [**UPnP_Generic library**](https://github.com/khoih-prog/UPnP_Generic):

- [EthernetUdp2.h](LibraryPatches/Ethernet2/src/EthernetUdp2.h)
- [EthernetUdp2.cpp](LibraryPatches/Ethernet2/src/EthernetUdp2.cpp)

5. To fix [`Ethernet3 library`](https://github.com/sstaub/Ethernet3), just copy these following files into the [`Ethernet3 library`](https://github.com/sstaub/Ethernet3) directory to overwrite the old files:
- [Ethernet3.h](LibraryPatches/Ethernet3/src/Ethernet3.h)
- [Ethernet3.cpp](LibraryPatches/Ethernet3/src/Ethernet3.cpp)

6. **To be able to compile and run on nRF52 boards with ENC28J60 using UIPEthernet library**, you have to copy these following files into the UIPEthernet `utility` directory to overwrite the old files:

- For [UIPEthernet v2.0.8](https://github.com/UIPEthernet/UIPEthernet)

  - [UIPEthernet.h](LibraryPatches/UIPEthernet/UIPEthernet.h)
  - [UIPEthernet.cpp](LibraryPatches/UIPEthernet/UIPEthernet.cpp)
  - [Enc28J60Network.h](LibraryPatches/UIPEthernet/utility/Enc28J60Network.h)
  - [Enc28J60Network.cpp](LibraryPatches/UIPEthernet/utility/Enc28J60Network.cpp)

- For [UIPEthernet v2.0.9](https://github.com/UIPEthernet/UIPEthernet)

  - [UIPEthernet.h](LibraryPatches/UIPEthernet-2.0.9/UIPEthernet.h)
  - [UIPEthernet.cpp](LibraryPatches/UIPEthernet-2.0.9/UIPEthernet.cpp)
  - [Enc28J60Network.h](LibraryPatches/UIPEthernet-2.0.9/utility/Enc28J60Network.h)
  - [Enc28J60Network.cpp](LibraryPatches/UIPEthernet-2.0.9/utility/Enc28J60Network.cpp)

7. Check if you need to install the UIPthernet patch [new STM32 core F3/F4 compatibility](https://github.com/UIPEthernet/UIPEthernet/commit/c6d6519a260166b722b9cee5dd1f0fb2682e6782) to avoid errors `#include HardwareSPI.h` on some STM32 boards (Nucleo-32 F303K8, etc.)

---
---

### HOWTO Fix `Multiple Definitions` Linker Error

The current library implementation, using **xyz-Impl.h instead of standard xyz.cpp**, possibly creates certain `Multiple Definitions` Linker error in certain use cases. Although it's simple to just modify several lines of code, either in the library or in the application, the library is adding 2 more source directories

1. **scr_h** for new h-only files
2. **src_cpp** for standard h/cpp files

besides the standard **src** directory.

To use the **old standard cpp** way, locate this library' directory, then just 

1. **Delete the all the files in src directory.**
2. **Copy all the files in src_cpp directory into src.**
3. Close then reopen the application code in Arduino IDE, etc. to recompile from scratch.

To re-use the **new h-only** way, just 

1. **Delete the all the files in src directory.**
2. **Copy the files in src_h directory into src.**
3. Close then reopen the application code in Arduino IDE, etc. to recompile from scratch.

---
---


## More useful Information about STM32 Timers

The Timers of STM32s are numerous, yet very sophisticated and powerful.

In general, across the STM32 microcontrollers families, the timer peripherals that have the same name also have the same features set, but there are a few exceptions. 

For example, the **TIM1** timer peripheral is shared across the STM32F1 Series, STM32F2 Series and STM32F4 Series, but for the specific case of STM32F30x microcontrollers family, the TIM1 timer peripheral features a bit richer features set than the TIM1 present in the other families.

The general purpose timers embedded by the STM32 microcontrollers share the same backbone structure; they differ only on the level of features embedded by a given timer peripheral. 

The level of features integration for a given timer peripheral is decided based on the applications field that it targets.

The timer peripherals can be classified as:
• Advanced-configuration timers like TIM1 and TIM8 among others.
• General-purpose configuration timers like TIM2 and TIM3 among others
• Lite-configuration timers like TIM9, TIM10, TIM12 and TIM16 among others
• Basic-configuration timers like TIM6 and TIM7 among others.

For example, STM32F103C8T6 has one advance timer, while STM32F103VET6 has two advanced timers. Nucleo-144 STM32F767ZI boards have 14 Timers, TIM1-TIM14.

<p align="center">
    <img src="https://github.com/khoih-prog/STM32_TimerInterrupt/blob/main/pics/STM32Timers.png">
</p>


More information can be found at [Embedded-Lab STM32 TIMERS](http://embedded-lab.com/blog/stm32-timers/)

To be sure which Timer is available for the board you're using, check the Core Package's related files. For example, for F767ZI, check these files:
1. ~/.arduino15/packages/STM32/hardware/stm32/1.9.0/system/Drivers/CMSIS/Device/ST/STM32F7xx/Include/stm32f7xx.h
2. ~/.arduino15/packages/STM32/hardware/stm32/1.9.0/system/Drivers/CMSIS/Device/ST/STM32F7xx/Include/stm32f767xx.h

The information will be as follows:

```
typedef struct
{
  __IO uint32_t CR1;         /*!< TIM control register 1,              Address offset: 0x00 */
  __IO uint32_t CR2;         /*!< TIM control register 2,              Address offset: 0x04 */
  __IO uint32_t SMCR;        /*!< TIM slave mode control register,     Address offset: 0x08 */
  __IO uint32_t DIER;        /*!< TIM DMA/interrupt enable register,   Address offset: 0x0C */
  __IO uint32_t SR;          /*!< TIM status register,                 Address offset: 0x10 */
  __IO uint32_t EGR;         /*!< TIM event generation register,       Address offset: 0x14 */
  __IO uint32_t CCMR1;       /*!< TIM capture/compare mode register 1, Address offset: 0x18 */
  __IO uint32_t CCMR2;       /*!< TIM capture/compare mode register 2, Address offset: 0x1C */
  __IO uint32_t CCER;        /*!< TIM capture/compare enable register, Address offset: 0x20 */
  __IO uint32_t CNT;         /*!< TIM counter register,                Address offset: 0x24 */
  __IO uint32_t PSC;         /*!< TIM prescaler,                       Address offset: 0x28 */
  __IO uint32_t ARR;         /*!< TIM auto-reload register,            Address offset: 0x2C */
  __IO uint32_t RCR;         /*!< TIM repetition counter register,     Address offset: 0x30 */
  __IO uint32_t CCR1;        /*!< TIM capture/compare register 1,      Address offset: 0x34 */
  __IO uint32_t CCR2;        /*!< TIM capture/compare register 2,      Address offset: 0x38 */
  __IO uint32_t CCR3;        /*!< TIM capture/compare register 3,      Address offset: 0x3C */
  __IO uint32_t CCR4;        /*!< TIM capture/compare register 4,      Address offset: 0x40 */
  __IO uint32_t BDTR;        /*!< TIM break and dead-time register,    Address offset: 0x44 */
  __IO uint32_t DCR;         /*!< TIM DMA control register,            Address offset: 0x48 */
  __IO uint32_t DMAR;        /*!< TIM DMA address for full transfer,   Address offset: 0x4C */
  __IO uint32_t OR;          /*!< TIM option register,                 Address offset: 0x50 */
  __IO uint32_t CCMR3;       /*!< TIM capture/compare mode register 3,      Address offset: 0x54 */
  __IO uint32_t CCR5;        /*!< TIM capture/compare mode register5,       Address offset: 0x58 */
  __IO uint32_t CCR6;        /*!< TIM capture/compare mode register6,       Address offset: 0x5C */
  __IO uint32_t AF1;         /*!< TIM Alternate function option register 1, Address offset: 0x60 */
  __IO uint32_t AF2;         /*!< TIM Alternate function option register 2, Address offset: 0x64 */

} TIM_TypeDef;
```

and

```
#define PERIPH_BASE            0x40000000UL /*!< Base address of : AHB/ABP Peripherals   
/*!< Peripheral memory map */
#define APB1PERIPH_BASE        PERIPH_BASE

/*!< APB1 peripherals */
#define TIM2_BASE             (APB1PERIPH_BASE + 0x0000UL)
#define TIM3_BASE             (APB1PERIPH_BASE + 0x0400UL)
#define TIM4_BASE             (APB1PERIPH_BASE + 0x0800UL)
#define TIM5_BASE             (APB1PERIPH_BASE + 0x0C00UL)
#define TIM6_BASE             (APB1PERIPH_BASE + 0x1000UL)
#define TIM7_BASE             (APB1PERIPH_BASE + 0x1400UL)
#define TIM12_BASE            (APB1PERIPH_BASE + 0x1800UL)
#define TIM13_BASE            (APB1PERIPH_BASE + 0x1C00UL)
#define TIM14_BASE            (APB1PERIPH_BASE + 0x2000UL)

/*!< APB2 peripherals */
#define TIM1_BASE             (APB2PERIPH_BASE + 0x0000UL)
#define TIM8_BASE             (APB2PERIPH_BASE + 0x0400UL)
#define TIM9_BASE             (APB2PERIPH_BASE + 0x4000UL)
#define TIM10_BASE            (APB2PERIPH_BASE + 0x4400UL)
#define TIM11_BASE            (APB2PERIPH_BASE + 0x4800UL)

...

#define TIM2                ((TIM_TypeDef *) TIM2_BASE)
#define TIM3                ((TIM_TypeDef *) TIM3_BASE)
#define TIM4                ((TIM_TypeDef *) TIM4_BASE)
#define TIM5                ((TIM_TypeDef *) TIM5_BASE)
#define TIM6                ((TIM_TypeDef *) TIM6_BASE)
#define TIM7                ((TIM_TypeDef *) TIM7_BASE)
#define TIM12               ((TIM_TypeDef *) TIM12_BASE)
#define TIM13               ((TIM_TypeDef *) TIM13_BASE)
#define TIM14               ((TIM_TypeDef *) TIM14_BASE)
...
#define TIM1                ((TIM_TypeDef *) TIM1_BASE)
#define TIM8                ((TIM_TypeDef *) TIM8_BASE)
...
#define TIM9                ((TIM_TypeDef *) TIM9_BASE)
#define TIM10               ((TIM_TypeDef *) TIM10_BASE)
#define TIM11               ((TIM_TypeDef *) TIM11_BASE)

```

---
---

## New from v1.0.0

Now with these new `16 ISR-based timers` (while consuming only **1 hardware timer**), the maximum interval is practically unlimited (limited only by unsigned long miliseconds). The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers Therefore, their executions are not blocked by bad-behaving functions / tasks.
This important feature is absolutely necessary for mission-critical tasks. 

The `ISR_Timer_Complex` example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs of each type of timers.
Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet and Blynk services. You can also have many `(up to 16)` timers to use.
This non-being-blocked important feature is absolutely necessary for mission-critical tasks. 
You'll see blynkTimer Software is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task 
in loop(), using delay() function as an example. The elapsed time then is very unaccurate

---

## Supported Boards

- STM32

---

## Usage

Before using any Timer, you have to make sure the Timer has not been used by any other purpose.

### 1. Using only Hardware Timer directly

#### 1.1 Init Hardware Timer

```
// Init STM32 timer TIM1
STM32Timer ITimer0(TIM1);
```

#### 1.2 Set Hardware Timer Interval and attach Timer Interrupt Handler function

```
void TimerHandler0(void)
{
  // Doing something here inside ISR
}

#define TIMER0_INTERVAL_MS        1000      // 1s = 1000ms
void setup()
{
  ....
  
  // Interval in microsecs
  if (ITimer0.attachInterruptInterval(TIMER0_INTERVAL_MS * 1000, TimerHandler0))
    Serial.println("Starting  ITimer0 OK, millis() = " + String(millis()));
  else
    Serial.println("Can't set ITimer0. Select another freq. or timer");
}  
```

### 2. Using 16 ISR_based Timers from 1 Hardware Timers


#### 2.1 Init Hardware Timer and ISR-based Timer

```
// Init STM32 timer TIM1
STM32Timer ITimer(TIM1);

// Init STM32_ISR_Timer
// Each STM32_ISR_Timer can service 16 different ISR-based timers
STM32_ISR_Timer ISR_Timer;
```

#### 2.2 Set Hardware Timer Interval and attach Timer Interrupt Handler functions

```
void TimerHandler(void)
{
  ISR_Timer.run();
}

#define HW_TIMER_INTERVAL_MS          50L

#define TIMER_INTERVAL_2S             2000L
#define TIMER_INTERVAL_5S             5000L
#define TIMER_INTERVAL_11S            11000L
#define TIMER_INTERVAL_101S           101000L

// In STM32, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void doingSomething2s()
{
  // Doing something here inside ISR
}
  
void doingSomething5s()
{
  // Doing something here inside ISR
}

void doingSomething11s()
{
  // Doing something here inside ISR
}

void doingSomething101s()
{
  // Doing something here inside ISR
}

void setup()
{
  ....
  
  // Interval in microsecs
  if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 1000, TimerHandler))
  {
    lastMillis = millis();
    Serial.println("Starting  ITimer OK, millis() = " + String(lastMillis));
  }
  else
    Serial.println("Can't set ITimer correctly. Select another freq. or interval");

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_Timer
  ISR_Timer.setInterval(TIMER_INTERVAL_2S, doingSomething2s);
  ISR_Timer.setInterval(TIMER_INTERVAL_5S, doingSomething5s);
  ISR_Timer.setInterval(TIMER_INTERVAL_11S, doingSomething11s);
  ISR_Timer.setInterval(TIMER_INTERVAL_101S, doingSomething101s);
}  
```


---
---

### Examples: 

 1. [Argument_None](examples/Argument_None)
 2. [ISR_RPM_Measure](examples/ISR_RPM_Measure)
 3. [ISR_Timer_Complex](examples/ISR_Timer_Complex)
 4. [RPM_Measure](examples/RPM_Measure)
 5. [SwitchDebounce](examples/SwitchDebounce)
 6. [TimerInterruptTest](examples/TimerInterruptTest)
 7. [TimerInterruptLEDDemo](examples/TimerInterruptLEDDemo)


---
---

### Example [ISR_Timer_Complex](examples/ISR_Timer_Complex)

```
#if !( defined(STM32F0) || defined(STM32F1) || defined(STM32F2) || defined(STM32F3)  ||defined(STM32F4) || defined(STM32F7) || \
       defined(STM32L0) || defined(STM32L1) || defined(STM32L4) || defined(STM32H7)  ||defined(STM32G0) || defined(STM32G4) || \
       defined(STM32WB) || defined(STM32MP1) )
  #error This code is designed to run on STM32F/L/H/G/WB/MP1 platform! Please check your Tools->Board setting.
#endif

#define BLYNK_PRINT Serial

//#define BLYNK_DEBUG
#ifdef BLYNK_DEBUG
  #undef BLYNK_DEBUG
#endif

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#define USE_BUILTIN_ETHERNET    true
//  If don't use USE_BUILTIN_ETHERNET, and USE_UIP_ETHERNET => use W5x00 with Ethernet library
#define USE_UIP_ETHERNET        false

#if (USE_BUILTIN_ETHERNET)
  #define ETHERNET_NAME     "Built-in LAN8742A Ethernet"
#elif (USE_UIP_ETHERNET)
  #define ETHERNET_NAME     "ENC28J60 Ethernet Shield"
#else
  #define ETHERNET_NAME     "W5x00 Ethernet Shield"
#endif

#define BLYNK_NO_YIELD

#if USE_BUILTIN_ETHERNET
  #include <BlynkSimple_STM32BI_Ethernet.h>
#elif USE_UIP_ETHERNET
  #include <BlynkSimpleUIPEthernet.h>
#else
  #include <BlynkSimpleEthernet.h>
#endif

#define USE_LOCAL_SERVER      true

#if USE_LOCAL_SERVER
  char auth[] = "******";
  char server[] = "account.duckdns.org";
  //char server[] = "192.168.2.112";

#else
  char auth[] = "******";
  char server[] = "blynk-cloud.com";
#endif
  
  #define BLYNK_HARDWARE_PORT       8080

#if !(USE_BUILTIN_ETHERNET || USE_UIP_ETHERNET)
  #define W5100_CS  10
  #define SDCARD_CS 4
#endif

// These define's must be placed at the beginning before #include "STM32TimerInterrupt.h"
#define STM32_TIMER_INTERRUPT_DEBUG      1

#include "STM32TimerInterrupt.h"
#include "STM32_ISR_Timer.h"

#define TIMER_INTERVAL_MS         100
#define HW_TIMER_INTERVAL_MS      50

volatile uint32_t lastMillis = 0;

// Depending on the board, you can select STM32 Hardware Timer from TIM1-TIM22
// For example, F767ZI can select Timer from TIM1-TIM14
// If you select a Timer not correctly, you'll get a message from ci[ompiler
// 'TIMxx' was not declared in this scope; did you mean 'TIMyy'? 

// Init STM32 timer TIM1
STM32Timer ITimer(TIM1);

// Init STM32_ISR_Timer
// Each STM32_ISR_Timer can service 16 different ISR-based timers
STM32_ISR_Timer ISR_Timer;

// Ibit Blynk Timer
BlynkTimer blynkTimer;

#ifndef LED_BUILTIN
  #define LED_BUILTIN       PB0               // Pin 33/PB0 control on-board LED_GREEN on F767ZI
#endif

#define LED_TOGGLE_INTERVAL_MS        5000L

#define TIMER_INTERVAL_2S             2000L
#define TIMER_INTERVAL_5S             5000L
#define TIMER_INTERVAL_11S            11000L
#define TIMER_INTERVAL_101S           101000L

void TimerHandler(void)
{
  static bool toggle  = false;
  static bool started = false;
  static int timeRun  = 0;

  ISR_Timer.run();

  // Toggle LED every LED_TOGGLE_INTERVAL_MS = 5000ms = 5s
  if (++timeRun == (LED_TOGGLE_INTERVAL_MS / HW_TIMER_INTERVAL_MS) )
  {
    timeRun = 0;

    if (!started)
    {
      started = true;
      pinMode(LED_BUILTIN, OUTPUT);
    }

    //timer interrupt toggles pin LED_BUILTIN
    digitalWrite(LED_BUILTIN, toggle);
    toggle = !toggle;
  }
}

// In STM32, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void doingSomething2s()
{
  static unsigned long previousMillis = lastMillis;
  unsigned long deltaMillis = millis() - previousMillis;

#if (STM32_TIMER_INTERRUPT_DEBUG > 0)
  if (previousMillis > TIMER_INTERVAL_2S)
  {
    Serial.print("2s: Delta ms = ");
    Serial.println(deltaMillis);
  }
#endif

  previousMillis = millis();
}

// In STM32, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void doingSomething5s()
{
  static unsigned long previousMillis = lastMillis;
  unsigned long deltaMillis = millis() - previousMillis;

#if (STM32_TIMER_INTERRUPT_DEBUG > 0)
  if (previousMillis > TIMER_INTERVAL_5S)
  {
    Serial.print("5s: Delta ms = ");
    Serial.println(deltaMillis);
  }
#endif

  previousMillis = millis();
}

// In STM32, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void doingSomething11s()
{
  static unsigned long previousMillis = lastMillis;
  unsigned long deltaMillis = millis() - previousMillis;

#if (STM32_TIMER_INTERRUPT_DEBUG > 0)
  if (previousMillis > TIMER_INTERVAL_11S)
  {
    Serial.print("11s: Delta ms = ");
    Serial.println(deltaMillis);
  }
#endif

  previousMillis = millis();
}

// In STM32, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void doingSomething101s()
{
  static unsigned long previousMillis = lastMillis;
  unsigned long deltaMillis = millis() - previousMillis;

#if (STM32_TIMER_INTERRUPT_DEBUG > 0)
  if (previousMillis > TIMER_INTERVAL_101S)
  {
    Serial.print("101s: Delta ms = ");
    Serial.println(deltaMillis);
  }
#endif

  previousMillis = millis();
}

#define BLYNK_TIMER_MS        2000L

// Here is software Timer, you can do somewhat fancy stuffs without many issues.
// But always avoid
// 1. Long delay() it just doing nothing and pain-without-gain wasting CPU power.Plan and design your code / strategy ahead
// 2. Very long "do", "while", "for" loops without predetermined exit time.
void blynkDoingSomething2s()
{
  static unsigned long previousMillis = lastMillis;
  Serial.println("blynkDoingSomething2s: Delta programmed ms = " + String(BLYNK_TIMER_MS) + ", actual = " + String(millis() - previousMillis));
  previousMillis = millis();
}

void setup()
{
  Serial.begin(115200);
  while (!Serial);
  
  Serial.println("\nStarting ISR_Timer_Complex on " + String(BOARD_NAME));
  Serial.println("Version : " + String(STM32_TIMER_INTERRUPT_VERSION));

  // You need this timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary.
  blynkTimer.setInterval(BLYNK_TIMER_MS, blynkDoingSomething2s);

  // Interval in microsecs
  if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_MS * 1000, TimerHandler))
  {
    lastMillis = millis();
    Serial.println("Starting  ITimer OK, millis() = " + String(lastMillis));
  }
  else
    Serial.println("Can't set ITimer correctly. Select another freq. or interval");

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_Timer
  ISR_Timer.setInterval(TIMER_INTERVAL_2S, doingSomething2s);
  ISR_Timer.setInterval(TIMER_INTERVAL_5S, doingSomething5s);
  ISR_Timer.setInterval(TIMER_INTERVAL_11S, doingSomething11s);
  ISR_Timer.setInterval(TIMER_INTERVAL_101S, doingSomething101s);

#if !(USE_BUILTIN_ETHERNET || USE_UIP_ETHERNET)
  pinMode(SDCARD_CS, OUTPUT);
  digitalWrite(SDCARD_CS, HIGH); // Deselect the SD card
#endif

#if USE_LOCAL_SERVER
  Blynk.begin(auth, server, BLYNK_HARDWARE_PORT);
#else
  Blynk.begin(auth);
  // You can also specify server:
  //Blynk.begin(auth, server, BLYNK_HARDWARE_PORT);
#endif

  if (Blynk.connected())
  {
    Serial.print(F("IP = "));
    Serial.println(Ethernet.localIP());
  }
}

#define BLOCKING_TIME_MS      3000L

void loop()
{
  static bool needWiFiBegin = true;

  Blynk.run();

  // This unadvised blocking task is used to demonstrate the blocking effects onto the execution and accuracy to Software timer
  // You see the time elapse of ISR_Timer still accurate, whereas very unaccurate for Software Timer
  // The time elapse for 2000ms software timer now becomes 3000ms (BLOCKING_TIME_MS)
  // While that of ISR_Timer is still prefect.
  delay(BLOCKING_TIME_MS);

  // You need this Software timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary
  // You don't need to and never call ISR_Timer.run() here in the loop(). It's already handled by ISR timer.
  blynkTimer.run();
}
```
---
---

### Debug Terminal Output Samples

1. The following is the sample terminal output when running example [ISR_Timer_Complex](examples/ISR_Timer_Complex) to demonstrate the accuracy of ISR Hardware Timer, **especially when system is very busy**.  The ISR timer is **programmed for 2s, is activated exactly after 2.000s !!!**

While software timer, **programmed for 2s, is activated after 9.782s !!!**. Then in loop(), it's also activated **every 3s**.

```
Starting ISR_Timer_Complex on NUCLEO_F767ZI
Version : 1.0.0
STM32TimerInterrupt: Timer Input Freq (Hz) = 216000000, _fre = 1000000.00, _count = 50000
Starting  ITimer OK, millis() = 6
[9] MAC:FE-E1-88-EC-DD-95
2s: Delta ms = 2000
2s: Delta ms = 2000
[6626] IP:192.168.2.116
[6626] 
    ___  __          __
   / _ )/ /_ _____  / /__
  / _  / / // / _ \/  '_/
 /____/_/\_, /_//_/_/\_\
        /___/ v0.6.1 on STM32 NUCLEO_F767ZI

[6636] BlynkArduinoClient.connect: Connecting to account.duckdns.org:8080
[6721] Ready (ping: 6ms).
IP = 192.168.2.116
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 9782
2s: Delta ms = 2000
5s: Delta ms = 5000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
5s: Delta ms = 5000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
5s: Delta ms = 5000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
11s: Delta ms = 11000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
5s: Delta ms = 5000
2s: Delta ms = 2000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
2s: Delta ms = 2000
5s: Delta ms = 5000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
11s: Delta ms = 11000
blynkDoingSomething2s: Delta programmed ms = 2000, actual = 3000
2s: Delta ms = 2000
5s: Delta ms = 5000
2s: Delta ms = 2000
```

---

2. The following is the sample terminal output when running example [TimerInterruptTest](examples/TimerInterruptTest) to demonstrate how to start/stop Hardware Timers.

```

Starting TimerInterruptTest on NUCLEO_F767ZI
Version : 1.0.0
STM32TimerInterrupt: Timer Input Freq (Hz) = 216000000, _fre = 1000000.00, _count = 1000000
Starting  ITimer0 OK, millis() = 108
STM32TimerInterrupt: Timer Input Freq (Hz) = 108000000, _fre = 1000000.00, _count = 3000000
Starting  ITimer1 OK, millis() = 119
Stop ITimer0, millis() = 5001
Start ITimer0, millis() = 10002
Stop ITimer1, millis() = 15001
Stop ITimer0, millis() = 15003
Start ITimer0, millis() = 20004
Stop ITimer0, millis() = 25005
Start ITimer1, millis() = 30002
Start ITimer0, millis() = 30006
Stop ITimer0, millis() = 35007
Start ITimer0, millis() = 40008
Stop ITimer1, millis() = 45003
Stop ITimer0, millis() = 45009
Start ITimer0, millis() = 50010
Stop ITimer0, millis() = 55011
Start ITimer1, millis() = 60004
Start ITimer0, millis() = 60012
Stop ITimer0, millis() = 65013
Start ITimer0, millis() = 70014
Stop ITimer1, millis() = 75005
Stop ITimer0, millis() = 75015
Start ITimer0, millis() = 80016
Stop ITimer0, millis() = 85017
Start ITimer1, millis() = 90006
Start ITimer0, millis() = 90018
Stop ITimer0, millis() = 95019
Start ITimer0, millis() = 100020
Stop ITimer1, millis() = 105007
Stop ITimer0, millis() = 105021
Start ITimer0, millis() = 110022
Stop ITimer0, millis() = 115023
Start ITimer1, millis() = 120008
Start ITimer0, millis() = 120024
Stop ITimer0, millis() = 125025
Start ITimer0, millis() = 130026
Stop ITimer1, millis() = 135009
Stop ITimer0, millis() = 135027
Start ITimer0, millis() = 140028

```



```
Starting Argument_None on NUCLEO_F767ZI
Version : 1.0.0
STM32TimerInterrupt: Timer Input Freq (Hz) = 216000000, _fre = 1000000.00, _count = 1000000
Starting  ITimer0 OK, millis() = 106
STM32TimerInterrupt: Timer Input Freq (Hz) = 108000000, _fre = 1000000.00, _count = 5000000
Starting  ITimer1 OK, millis() = 117
STM32TimerInterrupt: Timer Input Freq (Hz) = 108000000, _fre = 1000000.00, _count = 20000000
Starting  ITimer2 OK, millis() = 129
ITimer0: millis() = 1106, delta = 1000
ITimer0: millis() = 2106, delta = 1000
ITimer0: millis() = 3106, delta = 1000
ITimer0: millis() = 4106, delta = 1000
ITimer0: millis() = 5106, delta = 1000
ITimer1: millis() = 5118, delta = 5000
ITimer0: millis() = 6106, delta = 1000
ITimer0: millis() = 7106, delta = 1000
ITimer0: millis() = 8106, delta = 1000
ITimer0: millis() = 9106, delta = 1000
ITimer0: millis() = 10106, delta = 1000
ITimer1: millis() = 10118, delta = 5000
ITimer0: millis() = 11106, delta = 1000
ITimer0: millis() = 12106, delta = 1000
ITimer0: millis() = 13106, delta = 1000
ITimer0: millis() = 14106, delta = 1000
ITimer0: millis() = 15106, delta = 1000
ITimer1: millis() = 15118, delta = 5000
ITimer0: millis() = 16106, delta = 1000
ITimer0: millis() = 17106, delta = 1000
ITimer0: millis() = 18106, delta = 1000
ITimer0: millis() = 19106, delta = 1000
ITimer0: millis() = 20106, delta = 1000
ITimer1: millis() = 20118, delta = 5000
ITimer2: millis() = 20129, delta = 19999
ITimer0: millis() = 21106, delta = 1000
ITimer0: millis() = 22106, delta = 1000
ITimer0: millis() = 23106, delta = 1000
ITimer0: millis() = 24106, delta = 1000
ITimer0: millis() = 25106, delta = 1000
ITimer1: millis() = 25118, delta = 5000
ITimer0: millis() = 26106, delta = 1000
ITimer0: millis() = 27106, delta = 1000
ITimer0: millis() = 28106, delta = 1000
ITimer0: millis() = 29106, delta = 1000
ITimer0: millis() = 30106, delta = 1000
ITimer1: millis() = 30118, delta = 5000
ITimer0: millis() = 31106, delta = 1000
ITimer0: millis() = 32106, delta = 1000
ITimer0: millis() = 33106, delta = 1000
ITimer0: millis() = 34106, delta = 1000
ITimer0: millis() = 35106, delta = 1000
ITimer1: millis() = 35118, delta = 5000
ITimer0: millis() = 36106, delta = 1000
ITimer0: millis() = 37106, delta = 1000

```
---
---

### Releases v1.0.0

1. Permit up to 16 super-long-time, super-accurate ISR-based timers to avoid being blocked
2. Using cpp code besides Impl.h code to use if Multiple-Definition linker error.

#### Supported Boards

1. **STM32 boards with built-in Ethernet LAN8742A** such as :

  - **Nucleo-144 (F429ZI, F767ZI)**
  - **Discovery (STM32F746G-DISCOVERY)**
  - **All STM32 boards (STM32F/L/H/G/WB/MP1) with 32K+ Flash, with Built-in Ethernet**
  - See [EthernetWebServer_STM32 Support and Test Results](https://github.com/khoih-prog/EthernetWebServer_STM32/issues/1)
  
2. **STM32F/L/H/G/WB/MP1 boards (with 32+K Flash) running W5x00 or ENC28J60 shields)**

- Nucleo-144
- Nucleo-64
- Discovery
- Generic STM32F0, STM32F1, STM32F2, STM32F3, STM32F4, STM32F7 (with 64+K Flash): x8 and up
- STM32L0, STM32L1, STM32L4
- STM32G0, STM32G4
- STM32H7
- STM32WB
- STM32MP1
- LoRa boards
- 3-D printer boards
- Generic Flight Controllers
- Midatronics boards

---
---

### Issues ###

Submit issues to: [STM32_TimerInterrupt issues](https://github.com/khoih-prog/STM32_TimerInterrupt/issues)

---

## TO DO

1. Search for bug and improvement.
2. Similar features for remaining Arduino boards such as SAMD21, SAMD51, SAM-DUE, nRF52


## DONE

For current version v1.0.0

1. Basic hardware timers for STM32.
2. More hardware-initiated software-enabled timers
3. Longer time interval

---
---

### Contributions and Thanks

Many thanks for everyone for bug reporting, new feature suggesting, testing and contributing to the development of this library.


---

## Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

### License

- The library is licensed under [MIT](https://github.com/khoih-prog/STM32_TimerInterrupt/blob/master/LICENSE)

---

## Copyright

Copyright 2020- Khoi Hoang


