/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.hpp"
#include "hal.h"
#include "string.h"

#include "matrix_test.hpp"

using namespace chibios_rt;

#define rccEnableCCM(lp) rccEnableAHB1(RCC_AHB1ENR_CCMDATARAMEN, lp)
#define rccDisableCCM(lp) rccDisableAHB1(RCC_AHB1ENR_CCMDATARAMEN, lp)

memory_heap_t MatrixHeap;
#define MATRIX_HEAP_SIZE    65536
static void *matrix_heap_buf = (void *)0x10000000;


#define matrix_size (8 * 32 * 32)
MEMORYPOOL_DECL(matrix_pool, matrix_size, chCoreAllocI);

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  System::init();


  rccEnableCCM(false);
  memset(matrix_heap_buf, 0x55, MATRIX_HEAP_SIZE);
  chHeapObjectInit(&MatrixHeap, matrix_heap_buf, MATRIX_HEAP_SIZE);

  chPoolObjectInit(&matrix_pool, matrix_size, chCoreAllocI);

  /*
   * Serves timer events.
   */
  while (true) {
    //BaseThread::sleep(MS2ST(50));
    matrix_test();
  }

  return 0;
}
