/****************************************************************************************************************************
   STM32_ISR_Timer-Impl.h
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

   Version: 1.0.0

   Version Modified By   Date      Comments
   ------- -----------  ---------- -----------
   1.0.0   K Hoang      30/10/2020 Initial coding
*****************************************************************************************************************************/

#pragma once

//#include "STM32_ISR_Timer.h"
#include <string.h>

STM32_ISR_Timer::STM32_ISR_Timer()
  : numTimers (-1)
{
}

void STM32_ISR_Timer::init() 
{
  unsigned long current_millis = millis();   //elapsed();

  for (int i = 0; i < MAX_TIMERS; i++) 
  {
    memset((void*) &timer[i], 0, sizeof (timer_t));
    timer[i].prev_millis = current_millis;
  }

  numTimers = 0;
}

void STM32_ISR_Timer::run() 
{
  int i;
  unsigned long current_millis;

  // get current time
  current_millis = millis();   //elapsed();

  for (i = 0; i < MAX_TIMERS; i++) 
  {

    timer[i].toBeCalled = DEFCALL_DONTRUN;

    // no callback == no timer, i.e. jump over empty slots
    if (timer[i].callback != NULL) 
    {

      // is it time to process this timer ?
      // see http://arduino.cc/forum/index.php/topic,124048.msg932592.html#msg932592

      if ((current_millis - timer[i].prev_millis) >= timer[i].delay) 
      {
        unsigned long skipTimes = (current_millis - timer[i].prev_millis) / timer[i].delay;
        
        // update time
        timer[i].prev_millis += timer[i].delay * skipTimes;

        // check if the timer callback has to be executed
        if (timer[i].enabled) 
        {

          // "run forever" timers must always be executed
          if (timer[i].maxNumRuns == RUN_FOREVER) 
          {
            timer[i].toBeCalled = DEFCALL_RUNONLY;
          }
          // other timers get executed the specified number of times
          else if (timer[i].numRuns < timer[i].maxNumRuns) 
          {
            timer[i].toBeCalled = DEFCALL_RUNONLY;
            timer[i].numRuns++;

            // after the last run, delete the timer
            if (timer[i].numRuns >= timer[i].maxNumRuns) 
            {
              timer[i].toBeCalled = DEFCALL_RUNANDDEL;
            }
          }
        }
      }
    }
  }

  for (i = 0; i < MAX_TIMERS; i++) 
  {
    if (timer[i].toBeCalled == DEFCALL_DONTRUN)
      continue;

    if (timer[i].hasParam)
      (*(timer_callback_p)timer[i].callback)(timer[i].param);
    else
      (*(timer_callback)timer[i].callback)();

    if (timer[i].toBeCalled == DEFCALL_RUNANDDEL)
      deleteTimer(i);
  }
}


// find the first available slot
// return -1 if none found
int STM32_ISR_Timer::findFirstFreeSlot() 
{
  // all slots are used
  if (numTimers >= MAX_TIMERS) 
  {
    return -1;
  }

  // return the first slot with no callback (i.e. free)
  for (int i = 0; i < MAX_TIMERS; i++) 
  {
    if (timer[i].callback == NULL) 
    {
      return i;
    }
  }

  // no free slots found
  return -1;
}


int STM32_ISR_Timer::setupTimer(unsigned long d, void* f, void* p, bool h, unsigned n) 
{
  int freeTimer;

  if (numTimers < 0) 
  {
    init();
  }

  freeTimer = findFirstFreeSlot();
  if (freeTimer < 0) 
  {
    return -1;
  }

  if (f == NULL) 
  {
    return -1;
  }

  timer[freeTimer].delay = d;
  timer[freeTimer].callback = f;
  timer[freeTimer].param = p;
  timer[freeTimer].hasParam = h;
  timer[freeTimer].maxNumRuns = n;
  timer[freeTimer].enabled = true;
  timer[freeTimer].prev_millis = millis();

  numTimers++;

  return freeTimer;
}


int STM32_ISR_Timer::setTimer(unsigned long d, timer_callback f, unsigned n) 
{
  return setupTimer(d, (void *)f, NULL, false, n);
}

int STM32_ISR_Timer::setTimer(unsigned long d, timer_callback_p f, void* p, unsigned n) 
{
  return setupTimer(d, (void *)f, p, true, n);
}

int STM32_ISR_Timer::setInterval(unsigned long d, timer_callback f) 
{
  return setupTimer(d, (void *)f, NULL, false, RUN_FOREVER);
}

int STM32_ISR_Timer::setInterval(unsigned long d, timer_callback_p f, void* p) 
{
  return setupTimer(d, (void *)f, p, true, RUN_FOREVER);
}

int STM32_ISR_Timer::setTimeout(unsigned long d, timer_callback f) 
{
  return setupTimer(d, (void *)f, NULL, false, RUN_ONCE);
}

int STM32_ISR_Timer::setTimeout(unsigned long d, timer_callback_p f, void* p) 
{
  return setupTimer(d, (void *)f, p, true, RUN_ONCE);
}

bool STM32_ISR_Timer::changeInterval(unsigned numTimer, unsigned long d) 
{
  if (numTimer >= MAX_TIMERS) 
  {
    return false;
  }

  // Updates interval of existing specified timer
  if (timer[numTimer].callback != NULL) 
  {
    timer[numTimer].delay = d;
    timer[numTimer].prev_millis = millis();

    return true;
  }
  
  // false return for non-used numTimer, no callback
  return false;
}

void STM32_ISR_Timer::deleteTimer(unsigned timerId) 
{
  if (timerId >= MAX_TIMERS) 
  {
    return;
  }

  // nothing to delete if no timers are in use
  if (numTimers == 0) 
  {
    return;
  }

  // don't decrease the number of timers if the specified slot is already empty
  if (timer[timerId].callback != NULL) 
  {
    memset((void*) &timer[timerId], 0, sizeof (timer_t));
    timer[timerId].prev_millis = millis();

    // update number of timers
    numTimers--;
  }
}

// function contributed by code@rowansimms.com
void STM32_ISR_Timer::restartTimer(unsigned numTimer) 
{
  if (numTimer >= MAX_TIMERS) 
  {
    return;
  }

  timer[numTimer].prev_millis = millis();
}


bool STM32_ISR_Timer::isEnabled(unsigned numTimer) 
{
  if (numTimer >= MAX_TIMERS) 
  {
    return false;
  }

  return timer[numTimer].enabled;
}


void STM32_ISR_Timer::enable(unsigned numTimer) 
{
  if (numTimer >= MAX_TIMERS) 
  {
    return;
  }

  timer[numTimer].enabled = true;
}


void STM32_ISR_Timer::disable(unsigned numTimer) 
{
  if (numTimer >= MAX_TIMERS) 
  {
    return;
  }

  timer[numTimer].enabled = false;
}

void STM32_ISR_Timer::enableAll() 
{
  // Enable all timers with a callback assigned (used)

  for (int i = 0; i < MAX_TIMERS; i++) 
  {
    if (timer[i].callback != NULL && timer[i].numRuns == RUN_FOREVER) 
    {
      timer[i].enabled = true;
    }
  }
}

void STM32_ISR_Timer::disableAll() 
{
  // Disable all timers with a callback assigned (used)

  for (int i = 0; i < MAX_TIMERS; i++) 
  {
    if (timer[i].callback != NULL && timer[i].numRuns == RUN_FOREVER) 
    {
      timer[i].enabled = false;
    }
  }
}

void STM32_ISR_Timer::toggle(unsigned numTimer) 
{
  if (numTimer >= MAX_TIMERS) 
  {
    return;
  }

  timer[numTimer].enabled = !timer[numTimer].enabled;
}


unsigned STM32_ISR_Timer::getNumTimers() 
{
  return numTimers;
}
