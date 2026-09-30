/**
 * ******************************************************************************
 * @file    : adc12.c
 * @brief   : ADC module c file
 * @details : ADC initialization and interaction
 * @note    : ADC does not require IOMUX interaction
 * 
 * @author 
 * @date 
 * ******************************************************************************
*/

#include <stdint.h>
#include "adc12.h"
#include <ti/devices/msp/msp.h>
#include "sysctl.h"
#include "lab5/adc12.h"
#include <string.h>
#include <stdbool.h>


void ADC0_init(void)
{
    // Initialize the ADC0
    if (!(ADC0->ULLMEM.GPRCM.PWREN & ADC12_PWREN_ENABLE_MASK))
    {
		//Peripheral reset control
		ADC0->ULLMEM.GPRCM.RSTCTL |= (ADC12_RSTCTL_KEY_UNLOCK_W | ADC12_RSTCTL_RESETASSERT_ASSERT | ADC12_RSTCTL_RESETSTKYCLR_CLR);
		ADC0->ULLMEM.GPRCM.PWREN |= (ADC12_PWREN_ENABLE_ENABLE | ADC12_PWREN_KEY_UNLOCK_W);
	}

    //Set the ADC clock to use the ULPCLK
    ADC0->ULLMEM.GPRCM.CLKCFG = ADC12_CLKCFG_SAMPCLK_ULPCLK;

    //Set the ADC clock to highest possible frequency range
    ADC0->ULLMEM.CLKFREQ = ADC12_CLKFREQ_FRANGE_RANGE40TO48;


    //Set the ADC to not shut down between conversions, and clock divider to 8
    ADC0->ULLMEM.CTL0 = (ADC12_CTL0_SCLKDIV_DIV_BY_8|
                  ADC12_CTL0_PWRDN_MANUAL);

    //MEMCTL for channel 0, set to auto trigger next conversion and select channel 0
    ADC0->ULLMEM.MEMCTL[0] = (ADC12_MEMCTL_TRIG_AUTO_NEXT |
                       ADC12_MEMCTL_CHANSEL_CHAN_0);
    
    //Set the ADC to auto sample, single conversion, and software trigger
    ADC0->ULLMEM.CTL1 = (  ADC12_CTL1_SAMPMODE_AUTO |
                    ADC12_CTL1_CONSEQ_SINGLE |
                    ADC12_CTL1_SC_STOP | //stops conversion, set this to 1 later to start
                    ADC12_CTL1_TRIGSRC_SOFTWARE);
    

    //Set the start address for the ADC to store the conversion result
    ADC0->ULLMEM.CTL2 = ADC12_CTL2_STARTADD_ADDR_00;

    



}


uint32_t ADC0_getVal(void)
{
    return (uint32_t)0;
}