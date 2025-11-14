/**
 * @file led_challenge.c
 * @brief Toggles LED1 (RED) every 65000 LPTMR counts and flashes LED2 (GREEN)
 *        five times Every 3 seconds using the same LPTMR low power timer.
 *
 * Provides:
 *   - LPTMR0 timer initialization with 1 kHz LPO clock
 *   - LED1 periodic toggling via LPTMR interrupt
 *   - LED2 flash sequence using software timing in the main loop
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
#define LED2_PERIOD_S 3        // Time in seconds to start the LED2 flash sequence
#define LED2_FLASH_CNT 10      // Number of LED2 flashes * 2 (On and Off Cycle)
#define LED2_FLASH_FREQ_HZ 100 // LED2 flash frequency in Hz
#define LED1_TOGGLE_CNT 65000  // LED1 toggle period for LPTMR interrupt

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void init_timer(void);
void LPTMR0_IRQHandler(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

/**
 * @brief Initialize the LPTMR0 free-running timer using the 1kHz LPO oscillator
 */
static void init_timer(void) {
    struct _lptmr_config config;

    // Retrieve default settings
    LPTMR_GetDefaultConfig(&config);

    // Set timer parameters
    config.enableFreeRunning = true; // free running
    config.bypassPrescaler = true; // No prescaler needed
    config.prescalerClockSource = kLPTMR_PrescalerClock_1; // 1khz LPO clock

    LPTMR_Init(LPTMR0, &config);

    // Set timer compare value for LED1 interrupt
    LPTMR_SetTimerPeriod(LPTMR0, LED1_TOGGLE_CNT);

    // Enable timer interrupts and start timer
    LPTMR_EnableInterrupts(LPTMR0, kLPTMR_TimerInterruptEnable);
    LPTMR_StartTimer(LPTMR0);
}

/**
 * @breif LPTMR0 interrupt handler to toggle LED1 when the compare value is reached
 */
void LPTMR0_IRQHandler(void) {
    LPTMR_ClearStatusFlags(LPTMR0, kLPTMR_TimerCompareFlag);
    GPIO_PortToggle(BOARD_LED_RED_GPIO, 1u << BOARD_LED_RED_GPIO_PIN);
}

/**
 * @brief Main function
 */
int main(void)
{
    static uint16_t cnt = 0;
    static uint16_t cntCompare = 0;
    static uint16_t cntFlashCompare = 0;
    static uint16_t numFlash = LED2_FLASH_CNT;

    /* Board init */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    init_timer();
    // Enable interrupts
    NVIC_EnableIRQ(LPTMR0_IRQn);

    // Mainloop
    while (1)
    {
        // read current timer count
        LPTMR0->CNR = 0;
        cnt = LPTMR0->CNR;

        // Check if LED2_PERIOD_S seconds have elapsed
        if ((uint16_t)(cnt - cntCompare) >= (CLOCK_GetFreq(kCLOCK_LpoClk)*LED2_PERIOD_S)) {
            cntCompare = cnt;
            numFlash = 0;
        }

        // Handle flash sequence using software counters
        if (numFlash < LED2_FLASH_CNT) {
            // Check for flash frequency elapsed time
            if ((uint16_t)(cnt - cntFlashCompare) >= LED2_FLASH_FREQ_HZ) {
                //PRINTF("Timer Cnt: %d\r\n", cnt); // debug print
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
