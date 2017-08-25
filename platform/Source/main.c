#include "system.h"

#include "FreeRTOS.h"
#include "timers.h"

static void AppTimerCallback( TimerHandle_t xTimer );

static void AppTimerCallback( TimerHandle_t xTimer )
{
    char val = 0;
    if (!read(3, &val)) {
        val = !val;
        write(3, &val);
    }
    if (!read(4, &val)) {
        val = !val;
        write(4, &val);
    }
}

static TimerHandle_t timer;

int main(void)
{
    static StaticTimer_t BlinkTimer;

    char val = 1;

    system_init();

    write(1, &val);
    write(2, &val);

    timer = xTimerCreateStatic("BlinkTimer", 1000 / portTICK_PERIOD_MS, pdTRUE, NULL, AppTimerCallback, &BlinkTimer);

    if (timer) {
        vTaskStartScheduler();
    }

    while (1);

    return 0;
}

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
    /* If the buffers to be provided to the Idle task are declared inside this
    function then they must be declared static - otherwise they will be allocated on
    the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
    state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

void vApplicationIdleHook( void )
{
    if (pdFALSE == xTimerIsTimerActive(timer)) {
        int val = 1;
        write(3, &val);
        write(4, &val);
        xTimerStart(timer, 0);
    }
}
