/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_lptmr.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define LPT_CLOCK_HZ 1000
#define LED2_PERIOD_S 3
#define LED2_PERIOD_TICKS (LPT_CLOCK_HZ*LED2_PERIOD_S)
#define LED2_FLASH_CNT 10
#define LED2_FLASH_FREQ 100
#define LED1_TOGGLE_CNT 65000

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

void init_timer(void) {
    struct _lptmr_config config;
    LPTMR_GetDefaultConfig(&config);
    config.enableFreeRunning = true;
    config.bypassPrescaler = true;
    config.prescalerClockSource = kLPTMR_PrescalerClock_1; // 1khz clock
    LPTMR_Init(LPTMR0, &config);
    LPTMR_SetTimerPeriod(LPTMR0, LED1_TOGGLE_CNT);
    LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
    LPTMR_StartTimer(LPTMR0);
}

void LPTMR0_IRQHandler(void) {
    LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
    GPIO_PortToggle(BOARD_LED_RED_GPIO, 1u << BOARD_LED_RED_GPIO_PIN);
}

/*!
 * @brief Main function
 */
int main(void)
{
    static uint16_t cnt = 0;
    static uint16_t cntCompare = 0;
    static uint16_t cntFlashCompare = 0;
    static uint16_t numFlash = LED2_FLASH_CNT;

    /* Board pin init */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    init_timer();
    NVIC_EnableIRQ(LPTMR0_IRQn);

    while (1)
    {
        LPTMR0->CNR = 0;
        cnt = LPTMR0->CNR;

        if ((uint16_t)(cnt - cntCompare) >= LED2_PERIOD_TICKS) {
            cntCompare = cnt;
            numFlash = 0;
        }
        if (numFlash < LED2_FLASH_CNT) {
            if ((uint16_t)(cnt - cntFlashCompare) >= LED2_FLASH_FREQ) {
                PRINTF("HERE2 %d\r\n", cnt);
                cntFlashCompare = cnt;
                if (numFlash % 2) {
                    GPIO_PortSet(BOARD_LED_GREEN_GPIO, 1u << BOARD_LED_GREEN_GPIO_PIN);
                } else {
                    GPIO_PortClear(BOARD_LED_GREEN_GPIO, 1u << BOARD_LED_GREEN_GPIO_PIN);
                }
                numFlash++;
            }
        }
    }
}
