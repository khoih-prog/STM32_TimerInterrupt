/****************************************************************************************************************************
   ISR_16_Timers_Array.ino
   For STM32 boards
   Written by Khoi Hoang

   Built by Khoi Hoang https://github.com/khoih-prog/STM32_TimerInterrupt
   Licensed under MIT license

   Now even you use all these new 16 ISR-based timers,with their maximum interval practically unlimited (limited only by
   unsigned long miliseconds), you just consume only one STM32 timer and avoid conflicting with other cores' tasks.
   The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
   Therefore, their executions are not blocked by bad-behaving functions / tasks.
   This important feature is absolutely necessary for mission-critical tasks.

   Based on SimpleTimer - A timer library for Arduino.
   Author: mromani@ottotecnica.com
   Copyright (c) 2010 OTTOTECNICA Italy

   Based on BlynkTimer.h
   Author: Volodymyr Shymanskyy

   Version: 1.0.1

   Version Modified By   Date      Comments
   ------- -----------  ---------- -----------
    1.0.0   K Hoang      30/10/2020 Initial coding
    1.0.1   K Hoang      06/11/2020 Add complicated example ISR_16_Timers_Array using all 16 independent ISR Timers.
*****************************************************************************************************************************/
/*
   Notes:
   Special design is necessary to share data between interrupt code and the rest of your program.
   Variables usually need to be "volatile" types. Volatile tells the compiler to avoid optimizations that assume
   variable can not spontaneously change. Because your function may change variables while your program is using them,
   the compiler needs this hint. But volatile alone is often not enough.
   When accessing shared variables, usually interrupts must be disabled. Even with volatile,
   if the interrupt changes a multi-byte variable between a sequence of instructions, it can be read incorrectly.
   If your data is multiple variables, such as an array and a count, usually interrupts need to be disabled
   or the entire sequence of your code which accesses the data.

   RPM Measuring uses high frequency hardware timer 1Hz == 1ms) to measure the time from of one rotation, in ms
   then convert to RPM. One rotation is detected by reading the state of a magnetic REED SW or IR LED Sensor
   Asssuming LOW is active.
   For example: Max speed is 600RPM => 10 RPS => minimum 100ms a rotation. We'll use 80ms for debouncing
   If the time between active state is less than 8ms => consider noise.
   RPM = 60000 / (rotation time in ms)

   We use interrupt to detect whenever the SW is active, set a flag then use timer to count the time between active state

   This example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs.
   Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet
   and Blynk services. You can also have many (up to 16) timers to use.
   This non-being-blocked important feature is absolutely necessary for mission-critical tasks.
   You'll see blynkTimer is blocked while connecting to WiFi / Internet / Blynk, and elapsed time is very unaccurate
   In this super simple example, you don't see much different after Blynk is connected, because of no competing task is
   written
*/

#if !( defined(STM32F0) || defined(STM32F1) || defined(STM32F2) || defined(STM32F3)  ||defined(STM32F4) || defined(STM32F7) || \
       defined(STM32L0) || defined(STM32L1) || defined(STM32L4) || defined(STM32H7)  ||defined(STM32G0) || defined(STM32G4) || \
       defined(STM32WB) || defined(STM32MP1) )
  #error This code is designed to run on STM32F/L/H/G/WB/MP1 platform! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "STM32TimerInterrupt.h"
// Don't define STM32_TIMER_INTERRUPT_DEBUG > 2. Only for special ISR debugging only. Can hang the system.
#define STM32_TIMER_INTERRUPT_DEBUG      1

#include "STM32TimerInterrupt.h"
#include "STM32_ISR_Timer.h"

#include <SimpleTimer.h>              // https://github.com/schinken/SimpleTimer

#ifndef LED_BUILTIN
  #define LED_BUILTIN       13
#endif

#ifndef LED_BLUE
  #define LED_BLUE          2
#endif

#ifndef LED_RED
  #define LED_RED           3
#endif

#define HW_TIMER_INTERVAL_US      100L

volatile uint32_t startMillis = 0;

// Depending on the board, you can select STM32 Hardware Timer from TIM1-TIM22
// For example, F767ZI can select Timer from TIM1-TIM14
// If you select a Timer not correctly, you'll get a message from ci[ompiler
// 'TIMxx' was not declared in this scope; did you mean 'TIMyy'? 

// Init STM32 timer TIM1
STM32Timer ITimer(TIM1);

// Init STM32_ISR_Timer
// Each STM32_ISR_Timer can service 16 different ISR-based timers
STM32_ISR_Timer ISR_Timer;

#define LED_TOGGLE_INTERVAL_MS        2000L

void TimerHandler(void)
{
  static bool toggle  = false;
  static bool started = false;
  static int timeRun  = 0;

  ISR_Timer.run();

  // Toggle LED every LED_TOGGLE_INTERVAL_MS = 5000ms = 5s
  if (++timeRun == ( (LED_TOGGLE_INTERVAL_MS * 1000 ) / HW_TIMER_INTERVAL_US) )
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

#define NUMBER_ISR_TIMERS         16

// You can assign any interval for any timer here, in milliseconds
uint32_t TimerInterval[NUMBER_ISR_TIMERS] =
{
  1000L,  2000L,  3000L,  4000L,  5000L,  6000L,  7000L,  8000L,
  9000L, 10000L, 11000L, 12000L, 13000L, 14000L, 15000L, 16000L
};

typedef void (*irqCallback)  (void);

#if (STM32_TIMER_INTERRUPT_DEBUG > 0)
void printStatus(uint16_t index, unsigned long deltaMillis, unsigned long currentMillis)
{
  Serial.print(TimerInterval[index]/1000);
  Serial.print("s: Delta ms = ");
  Serial.print(deltaMillis);
  Serial.print(", ms = ");
  Serial.println(currentMillis);
}
#endif

// In STM32, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void doingSomething0()
{
  static unsigned long previousMillis = startMillis;
  
  unsigned long currentMillis = millis();
  unsigned long deltaMillis   = currentMillis - previousMillis;

#if (STM32_TIMER_INTERRUPT_DEBUG > 0)
  printStatus(0, deltaMillis, currentMillis);
#endif

  previousMillis = currentMillis;
}

void doingSomething1()
{
  static unsigned long previousMillis = startMillis;
  
  unsigned long currentMillis = millis();
  unsigned long deltaMillis   = currentMillis - previousMillis;

#if (STM32_TIMER_INTERRUPT_DEBUG > 0)
  printStatus(1, deltaMillis, currentMillis);
#endif

  previousMillis = currentMillis;
}

void doingSomething2()
{
  static unsigned long previousMillis = startMillis;
  
  unsigned long currentMillis = millis();
  unsigned long deltaMillis   = currentMillis - previousMillis;

#if (STM32_TIMER_INTERRUPT_DEBUG > 0)
  printStatus(2, deltaMillis, currentMillis);
#endif

  previousMillis = currentMillis;
}

void doingSomething3()
{
  static unsigned long previousMillis = startMillis;
  
  unsigned long currentMillis = millis();
  unsigned long deltaMillis   = currentMillis - previousMillis;

#if (STM32_TIMER_INTERRUPT_DEBUG > 0)
  printStatus(3, deltaMillis, currentMillis);
#endif

  previousMillis = currentMillis;
}

void doingSomething4()
{
  static unsigned long previousMillis = startMillis;
  
  unsigned long currentMillis = millis();
  unsigned long deltaMillis   = currentMillis - previousMillis;

#if (STM32_TIMER_INTERRUPT_DEBUG > 0)
  printStatus(4, deltaMillis, currentMillis);
#endif

  previousMillis = currentMillis;
}

void doingSomething5()
{
  static unsigned long previousMillis = startMillis;
  
  unsigned long currentMillis = millis();
  unsigned long deltaMillis   = currentMillis - previousMillis;

#if (STM32_TIMER_INTERRUPT_DEBUG > 0)
  printStatus(5, deltaMillis, currentMillis);
#endif

  previousMillis = currentMillis;
}

void doingSomething6()
{
  static unsigned long previousMillis = startMillis;
  
  unsigned long currentMillis = millis();
  unsigned long deltaMillis   = currentMillis - previousMillis;

#if (STM32_TIMER_INTERRUPT_DEBUG > 0)
  printStatus(6, deltaMillis, currentMillis);
#endif

  previousMillis = currentMillis;
}

void doingSomething7()
{
  static unsigned long previousMillis = startMillis;
  
  unsigned long currentMillis = millis();
  unsigned long deltaMillis   = currentMillis - previousMillis;

#if (STM32_TIMER_INTERRUPT_DEBUG > 0)
  printStatus(7, deltaMillis, currentMillis);
#endif

  previousMillis = currentMillis;
}

void doingSomething8()
{
  static unsigned long previousMillis = startMillis;
  
  unsigned long currentMillis = millis();
  unsigned long deltaMillis   = currentMillis - previousMillis;

#if (STM32_TIMER_INTERRUPT_DEBUG > 0)
  printStatus(8, deltaMillis, currentMillis);
#endif

  previousMillis = currentMillis;
}

void doingSomething9()
{
  static unsigned long previousMillis = startMillis;
  
  unsigned long currentMillis = millis();
  unsigned long deltaMillis   = currentMillis - previousMillis;

#if (STM32_TIMER_INTERRUPT_DEBUG > 0)
  printStatus(9, deltaMillis, currentMillis);
#endif

  previousMillis = currentMillis;
}

void doingSomething10()
{
  static unsigned long previousMillis = startMillis;
  
  unsigned long currentMillis = millis();
  unsigned long deltaMillis   = currentMillis - previousMillis;

#if (STM32_TIMER_INTERRUPT_DEBUG > 0)
  printStatus(10, deltaMillis, currentMillis);
#endif

  previousMillis = currentMillis;
}

// In STM32, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void doingSomething11()
{
  static unsigned long previousMillis = startMillis;
  
  unsigned long currentMillis = millis();
  unsigned long deltaMillis   = currentMillis - previousMillis;

#if (STM32_TIMER_INTERRUPT_DEBUG > 0)
  printStatus(11, deltaMillis, currentMillis);
#endif

  previousMillis = currentMillis;
}

// In STM32, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void doingSomething12()
{
  static unsigned long previousMillis = startMillis;
  
  unsigned long currentMillis = millis();
  unsigned long deltaMillis   = currentMillis - previousMillis;

#if (STM32_TIMER_INTERRUPT_DEBUG > 0)
  printStatus(12, deltaMillis, currentMillis);
#endif

  previousMillis = currentMillis;
}

void doingSomething13()
{
  static unsigned long previousMillis = startMillis;
  
  unsigned long currentMillis = millis();
  unsigned long deltaMillis   = currentMillis - previousMillis;

#if (STM32_TIMER_INTERRUPT_DEBUG > 0)
  printStatus(13, deltaMillis, currentMillis);
#endif

  previousMillis = currentMillis;
}

void doingSomething14()
{
  static unsigned long previousMillis = startMillis;
  
  unsigned long currentMillis = millis();
  unsigned long deltaMillis   = currentMillis - previousMillis;

#if (STM32_TIMER_INTERRUPT_DEBUG > 0)
  printStatus(14, deltaMillis, currentMillis);
#endif

  previousMillis = currentMillis;
}

void doingSomething15()
{
  static unsigned long previousMillis = startMillis;
  
  unsigned long currentMillis = millis();
  unsigned long deltaMillis   = currentMillis - previousMillis;

#if (STM32_TIMER_INTERRUPT_DEBUG > 0)
  printStatus(15, deltaMillis, currentMillis);
#endif

  previousMillis = currentMillis;
}

irqCallback irqCallbackFunc[NUMBER_ISR_TIMERS] =
{
  doingSomething0,  doingSomething1,  doingSomething2,  doingSomething3, 
  doingSomething4,  doingSomething5,  doingSomething6,  doingSomething7, 
  doingSomething8,  doingSomething9,  doingSomething10, doingSomething11,
  doingSomething12, doingSomething13, doingSomething14, doingSomething15
};

////////////////////////////////////////////////


#define SIMPLE_TIMER_MS        2000L

// Init SimpleTimer
SimpleTimer simpleTimer;

// Here is software Timer, you can do somewhat fancy stuffs without many issues.
// But always avoid
// 1. Long delay() it just doing nothing and pain-without-gain wasting CPU power.Plan and design your code / strategy ahead
// 2. Very long "do", "while", "for" loops without predetermined exit time.
void simpleTimerDoingSomething2s()
{
  static unsigned long previousMillis = startMillis;
  Serial.println("simpleTimerDoingSomething2s: Delta programmed ms = " + String(SIMPLE_TIMER_MS) + ", actual = " + String(millis() - previousMillis));
  previousMillis = millis();
}

void setup()
{
  Serial.begin(115200);
  while (!Serial);
  
  Serial.println("\nStarting ISR_16_Timers_Array on " + String(BOARD_NAME));
  Serial.println("Version : " + String(STM32_TIMER_INTERRUPT_VERSION));
  Serial.println("CPU Frequency = " + String(F_CPU / 1000000) + " MHz");

  // Interval in microsecs
  if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_US, TimerHandler))
  {
    startMillis = millis();
    Serial.println("Starting  ITimer OK, millis() = " + String(startMillis));
  }
  else
    Serial.println("Can't set ITimer correctly. Select another freq. or interval");

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_Timer
  for (int i = 0; i < NUMBER_ISR_TIMERS; i++)
  {
    ISR_Timer.setInterval(TimerInterval[i], irqCallbackFunc[i]); 
  }

  // You need this timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary.
  simpleTimer.setInterval(SIMPLE_TIMER_MS, simpleTimerDoingSomething2s);
}

#define BLOCKING_TIME_MS      10000L

void loop()
{
  // This unadvised blocking task is used to demonstrate the blocking effects onto the execution and accuracy to Software timer
  // You see the time elapse of ISR_Timer still accurate, whereas very unaccurate for Software Timer
  // The time elapse for 2000ms software timer now becomes 3000ms (BLOCKING_TIME_MS)
  // While that of ISR_Timer is still prefect.
  delay(BLOCKING_TIME_MS);

  // You need this Software timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary
  // You don't need to and never call ISR_Timer.run() here in the loop(). It's already handled by ISR timer.
  simpleTimer.run();
}
