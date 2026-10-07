/**
 * ******************************************************************************
 * @file    : camera.c
 * @brief   : Line scan camera module
 * ******************************************************************************
 */

#include <stdint.h>
#include <ti/devices/msp/msp.h>

#include "lab5/camera.h"
#include "lab5/timers.h"
#include "lab5/adc12.h"

// Camera pins
// SI  = PA28
// CLK = PA12
#define CAMERA_SI_PIN       GPIO_DOESET31_0_DIO28_SET
#define CAMERA_CLK_PIN      GPIO_DOESET31_0_DIO12_SET

// Camera data
static uint16_t cameraData[128];
static uint32_t pixelCounter = 0;

static bool cameraData_complete = false;


// Initialize the camera
// integrationTime is in microseconds
void Camera_init(void)
{
    // Enable GPIOA
    if (!(GPIOA->GPRCM.PWREN & GPIO_PWREN_ENABLE_MASK))
    {
        GPIOA->GPRCM.RSTCTL |=
            (GPIO_RSTCTL_KEY_UNLOCK_W |
             GPIO_RSTCTL_RESETASSERT_ASSERT |
             GPIO_RSTCTL_RESETSTKYCLR_CLR);

        GPIOA->GPRCM.PWREN |=
            (GPIO_PWREN_KEY_UNLOCK_W |
             GPIO_PWREN_ENABLE_ENABLE);
    }


    IOMUX->SECCFG.PINCM[IOMUX_PINCM34] |=
        (IOMUX_PINCM34_PF_GPIOA_DIO12 | IOMUX_PINCM_PC_CONNECTED);

    IOMUX->SECCFG.PINCM[IOMUX_PINCM3] |=
        (IOMUX_PINCM3_PF_GPIOA_DIO28 | IOMUX_PINCM_PC_CONNECTED);


    // Configure SI (PA28) and CLK (PA12) as outputs
    GPIOA->DOESET31_0 |= (GPIO_DOESET31_0_DIO28_SET | GPIO_DOESET31_0_DIO12_SET);

    // Start SI and CLK low
   // GPIOA->DOUTCLR31_0 |= (CAMERA_SI_PIN | CAMERA_CLK_PIN);


    // Initialize ADC
    ADC0_init();
   
    //input in microseconds
    TIMG0_init(10, 0);

    //input in milliseconds
    TIMG6_init(8, 255);


    // Make sure TIMG0 is disabled
    TIMG0->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_EN_ENABLED;
}



// TIMG6 interrupt
// Starts a new camera reading
void TIMG6_IRQHandler(void)
{


    // Do not start if old data has not been read
    cameraData_complete = 0;

    // Turn off the camera clock
    TIMG0->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_EN_ENABLED;



    GPIOA->DOUTSET31_0 |= GPIO_DOESET31_0_DIO28_SET;    // SI high
    GPIOA->DOUTSET31_0 |= GPIO_DOESET31_0_DIO12_SET;   // CLK high while SI is high
    GPIOA->DOUTCLR31_0 |= GPIO_DOUTCLR31_0_DIO28_CLR;    // SI low
    GPIOA->DOUTCLR31_0 |= GPIO_DOUTCLR31_0_DIO12_CLR;   // CLK low
    //

    //starts the timer
    TIMG0->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED;
    
}


// TIMG0 interrupt
// Generates CLK and reads the ADC
void TIMG0_IRQHandler(void)
{


        // CLK high
        GPIOA->DOUTSET31_0 |= GPIO_DOESET31_0_DIO12_SET;

        // CLK low
        GPIOA->DOUTCLR31_0 |= GPIO_DOUTCLR31_0_DIO12_CLR;

        // Read pixel
        cameraData[pixelCounter] = ADC0_getVal();
        pixelCounter++;


    


    // 128 pixels collected
    if (pixelCounter >= 128)
    {
        cameraData_complete = 1;

        TIMG0->COUNTERREGS.CTRCTL &= ~GPTIMER_CTRCTL_EN_ENABLED;

        pixelCounter = 0;
    }
}



// Check if camera data is ready
uint8_t Camera_isDataReady(void)
{
    if(cameraData_complete)
    {
        cameraData_complete = 0; // dont use boolean use int 1 or 0
        return 1;
    }
    else
    {
        return 0;
    }
}


// Get the camera data
uint16_t* Camera_getData(void)
{
    //cameraData_complete = false;

    return cameraData;
}
