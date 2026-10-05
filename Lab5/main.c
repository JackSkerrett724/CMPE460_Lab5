/**
 * ******************************************************************************
 * @file    : main.c
 * @brief   : Main run loop
 * 
 * @author  : Wil Salus
 * @date 		: 9/2/26
 * ******************************************************************************
*/
#include <stdint.h>
#include <stdio.h>
#include <ti/devices/msp/msp.h>
#include "lab2/uart.h"
#include "lab1/leds.h"
#include "lab5/timers.h"
#include "lab5/switches.h"

/* Enable/Disable definitions for LED1 states */
#define LED1_OFF     0
#define LED1_ON      1
#define LED1_TOGGLE  2

#define SW1_PIN_MASK  (1U << 18)  // PA18
#define SW2_PIN_MASK  (1U << 21)  // PB21

typedef enum {
    LED2_COLOR_OFF = 0,
    LED2_COLOR_RED = 1,
    LED2_COLOR_GREEN = 2,
    LED2_COLOR_BLUE = 3,
    LED2_COLOR_CYAN = 4,
    LED2_COLOR_MAGENTA = 5,
    LED2_COLOR_YELLOW = 6,
    LED2_COLOR_WHITE = 7,
} LED2_Color_t;

volatile int g_sw1_flashing_enabled = 0;
volatile bool g_sw1_flashing_on = true;
volatile LED2_Color_t g_led2_color_state = 0;
volatile uint32_t g_ms_counter = 0;
volatile bool g_sw2_timing_active = false;
volatile uint32_t g_start_time_ms = 0;
volatile int previous = 0;


//PART 1 MAIN

// int main(void) {
//     __disable_irq();

//     /* Initialize LEDs, Switches with Interrupts, and UART0 */
//     LED1_init();
//     LED2_init();

//     S1_init_interrupt();
//     S2_init_interrupt();
    
//     UART0_init();
	
//     TIMG6_init(62499, 255);

//     TIMG12_init(31999); 
// 	  LED1_set(1);
//     __enable_irq();

//     while (1) {
//         __WFI(); /* Wait For Interrupt */
//     }
// }


// void TIMG6_IRQHandler(void) 
// {
//     // Check if the Zero Event interrupt condition was met
//     if (TIMG6->CPU_INT.MIS & GPTIMER_CPU_INT_MIS_Z_MASK) {
//         // Clear the Zero event interrupt flag
//         TIMG6->CPU_INT.ICLR = GPTIMER_CPU_INT_ICLR_Z_CLR;
			
//         if (g_sw1_flashing_enabled) {
// 					LED1_set(2);
//         } else {
//           LED1_set(1);
//         }
//     }
// }


// void TIMG12_IRQHandler(void) 
// {
//     // Check if the Zero Event interrupt condition was met
//     if (TIMG12->CPU_INT.MIS & GPTIMER_CPU_INT_MIS_Z_MASK) {
//         // Clear the Zero event interrupt flag
//         TIMG12->CPU_INT.ICLR = GPTIMER_CPU_INT_ICLR_Z_CLR;

//         LED2_set(g_led2_color_state);

//         g_ms_counter++;
//     }
// }

// void GROUP1_IRQHandler(void) 
// {
// 		char uart_buf[64];
//     //Check SW1 on GPIOA
//     if (GPIOA->CPU_INT.MIS & GPIO_CPU_INT_MIS_DIO18_MASK) {
//         GPIOA->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO18_CLR; // Clear PA18 interrupt flag

//         if (g_sw1_flashing_enabled == 0) {
//             g_sw1_flashing_enabled = 1;
//         } else {
// 					g_sw1_flashing_enabled = 0;
// 				}
//     }

// 		if (GPIOB->CPU_INT.MIS & GPIO_CPU_INT_MIS_DIO21_MASK) {
//     GPIOB->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO21_CLR; // Clear PB21 interrupt flag
// 		static uint32_t previous_time = 0;
//     static bool is_timer_running = false;

//         uint32_t elapsed_time = g_ms_counter - previous_time;
//         previous_time = g_ms_counter; // Update start time for the next interval

// 				if (g_led2_color_state != LED2_COLOR_OFF){
// 					snprintf(uart_buf, sizeof(uart_buf), "Elapsed Time: %lu ms\r\n", (unsigned long)elapsed_time);
// 				} else {
// 					snprintf(uart_buf, sizeof(uart_buf), "LED OFF\r\n");
// 					is_timer_running = false;
// 				}
// 				UART0_put(uart_buf);
// 				g_led2_color_state = (g_led2_color_state + 1) % 8;
//         LED2_set(g_led2_color_state);
// 			}
// }





//PART 2 MAIN

int main(void)
{

    return 0;
}