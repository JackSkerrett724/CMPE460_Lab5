/**
 * ******************************************************************************
 * @file    : timers.c
 * @brief   : Timers module file
 * @details : Timers initialization and interaction
 * 
 * @author 
 * @date 
 * ******************************************************************************
*/

#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include <ti/devices/msp/peripherals/hw_gptimer.h>
#include "lab5/timers.h"

void TIMG0_init(uint32_t period, uint32_t prescaler)
{
    if (!(TIMG0->GPRCM.PWREN & GPTIMER_PWREN_ENABLE_MASK))
    {
        // Peripheral reset control
        TIMG0->GPRCM.RSTCTL |=
            (GPTIMER_RSTCTL_KEY_UNLOCK_W |
             GPTIMER_RSTCTL_RESETASSERT_ASSERT);

        TIMG0->GPRCM.PWREN |=
            (GPTIMER_PWREN_KEY_UNLOCK_W |
             GPTIMER_PWREN_ENABLE_ENABLE);
    }

    // Select BUSCLK
    TIMG0->CLKSEL |= GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE;

    // Clock divider = 1
    TIMG0->CLKDIV |= GPTIMER_CLKDIV_RATIO_DIV_BY_1;

    // Prescaler
    TIMG0->COMMONREGS.CPS |= prescaler;

    // Enable timer clock
    TIMG0->COMMONREGS.CCLKCTL |= GPTIMER_CCLKCTL_CLKEN_ENABLED;

    // Disable timer
    TIMG0->COUNTERREGS.CTRCTL = GPTIMER_CTRCTL_EN_DISABLED;

    // Repeat mode
    TIMG0->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_CM_MASK;

    TIMG0->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_REPEAT_REPEAT_1;

    TIMG0->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_CVAE_MASK;

    // Set timer period
    TIMG0->COUNTERREGS.LOAD = period;

    __disable_irq();

    // Clear Zero event interrupt
    TIMG0->CPU_INT.ICLR = GPTIMER_CPU_INT_ICLR_Z_CLR;

    // Enable Zero event interrupt
    TIMG0->CPU_INT.IMASK |= GPTIMER_CPU_INT_IMASK_Z_SET;

    // Enable TIMG0 interrupt
    NVIC_EnableIRQ(TIMG0_INT_IRQn);

    __enable_irq();

    // IMPORTANT:
    // Leave TIMG0 disabled.
    // Camera.c will enable it when a capture begins.
}


void TIMG6_init(uint32_t period, uint32_t prescaler){

	if (!(TIMG6->GPRCM.PWREN & GPTIMER_PWREN_ENABLE_MASK)){
		//Peripheral reset control
	TIMG6->GPRCM.RSTCTL |= (GPTIMER_RSTCTL_KEY_UNLOCK_W | GPTIMER_RSTCTL_RESETASSERT_ASSERT);
  TIMG6->GPRCM.PWREN  |= (GPTIMER_PWREN_KEY_UNLOCK_W  | GPTIMER_PWREN_ENABLE_ENABLE);
	}
	
	TIMG6->CLKSEL |= GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE;
	TIMG6->CLKDIV |= GPTIMER_CLKDIV_RATIO_DIV_BY_1; 
	TIMG6->COMMONREGS.CPS |= prescaler;
	TIMG6->COMMONREGS.CCLKCTL |= GPTIMER_CCLKCTL_CLKEN_ENABLED; 
	TIMG6->COUNTERREGS.CTRCTL = GPTIMER_CTRCTL_EN_DISABLED;
	
	TIMG6->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_CM_MASK;
	TIMG6->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_REPEAT_REPEAT_1;  
	TIMG6->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_CVAE_MASK;

	
	//uint64_t load = (((32000000*period)/((prescaler+1)*(1000000)))-1);
	TIMG6->COUNTERREGS.LOAD = period;
	
		__disable_irq();
	TIMG6->CPU_INT.ICLR |= GPTIMER_CPU_INT_ICLR_Z_CLR;
	TIMG6->CPU_INT.IMASK |= GPTIMER_CPU_INT_IMASK_Z_SET;
	TIMG6 ->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED;
	//NVIC_ClearPendingIRQ(TIMG6_INT_IRQn);
	NVIC_EnableIRQ(TIMG6_INT_IRQn);
	__enable_irq();

}


void TIMG12_init(uint32_t period){
	if (!(TIMG12->GPRCM.PWREN & GPTIMER_PWREN_ENABLE_MASK)) {
	TIMG12->GPRCM.RSTCTL |= (GPTIMER_RSTCTL_KEY_UNLOCK_W | GPTIMER_RSTCTL_RESETASSERT_ASSERT | GPTIMER_RSTCTL_RESETSTKYCLR_CLR);
  	TIMG12->GPRCM.PWREN  |= (GPTIMER_PWREN_KEY_UNLOCK_W  | GPTIMER_PWREN_ENABLE_ENABLE);
  	}
	
	TIMG12->CLKSEL = GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE;
	TIMG12->COMMONREGS.CCLKCTL |= GPTIMER_CCLKCTL_CLKEN_ENABLED ;
	TIMG12->COUNTERREGS.CTRCTL = GPTIMER_CTRCTL_EN_DISABLED;
	
	TIMG12->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_CM_MASK;
	TIMG12->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_REPEAT_REPEAT_1;  
	TIMG12->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_CVAE_MASK;
	
	//uint64_t load = (((32000000*period)/(1000000))-1);
	TIMG12->COUNTERREGS.LOAD = (uint32_t) period;
	
	__disable_irq();
	TIMG12->CPU_INT.ICLR  |= GPTIMER_CPU_INT_ICLR_Z_CLR;
  TIMG12->CPU_INT.IMASK |= GPTIMER_CPU_INT_IMASK_Z_SET;
	TIMG12->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED;
	
  	NVIC_EnableIRQ(TIMG12_INT_IRQn);
	__enable_irq();
}