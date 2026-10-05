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
#define CAMERA_SI_PIN       (1U << 28)
#define CAMERA_CLK_PIN      (1U << 12)

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

    // Configure SI (PA28) and CLK (PA12) as outputs
    GPIOA->DOESET31_0 = CAMERA_SI_PIN | CAMERA_CLK_PIN;

    // Start SI and CLK low
    GPIOA->DOUTCLR31_0 = CAMERA_SI_PIN | CAMERA_CLK_PIN;


    IOMUX->SECCFG.PINCM[IOMUX_PINCM34] |=
        (IOMUX_PINCM34_PF_GPIOA_DIO12 | IOMUX_PINCM_PC_CONNECTED);

    IOMUX->SECCFG.PINCM[IOMUX_PINCM3] |=
        (IOMUX_PINCM3_PF_GPIOA_DIO28 | IOMUX_PINCM_PC_CONNECTED);


    // Initialize ADC
    ADC0_init();

    // 32 MHz / 100 kHz = 320
    // LOAD = 320 - 1 = 319
    TIMG0_init(319, 0);

    // 32 MHz * 0.0075 s = 240000
    // LOAD = 240000 - 1 = 239999
    TIMG6_init(7499, 31);


    // Make sure TIMG0 is disabled
    TIMG0->COUNTERREGS.CTRCTL =
        (TIMG0->COUNTERREGS.CTRCTL &
         ~GPTIMER_CTRCTL_EN_MASK) |
        GPTIMER_CTRCTL_EN_DISABLED;
}



// TIMG6 interrupt
// Starts a new camera reading
void TIMG6_IRQHandler(void)
{
    if (TIMG6->CPU_INT.MIS & GPTIMER_CPU_INT_MIS_Z_MASK)
    {
        // Clear the interrupt
        TIMG6->CPU_INT.ICLR =
            GPTIMER_CPU_INT_ICLR_Z_CLR;

        // Do not start if old data has not been read
        if (cameraData_complete)
        {
            return;
        }

        // Turn off the camera clock
        TIMG0->COUNTERREGS.CTRCTL =
            (TIMG0->COUNTERREGS.CTRCTL &
             ~GPTIMER_CTRCTL_EN_MASK) |
            GPTIMER_CTRCTL_EN_DISABLED;

        GPIOA->DOUTSET31_0 = CAMERA_SI_PIN;    // SI high
        GPIOA->DOUTSET31_0 = CAMERA_CLK_PIN;   // CLK rises while SI is high
        GPIOA->DOUTCLR31_0 = CAMERA_SI_PIN;    // SI low
        GPIOA->DOUTCLR31_0 = CAMERA_CLK_PIN;   // CLK low

        // Reset pixel counter
        pixelCounter = 0;

        // Start camera clock
        TIMG0->CPU_INT.ICLR =
            GPTIMER_CPU_INT_ICLR_Z_CLR;

        TIMG0->COUNTERREGS.CTRCTL =
            (TIMG0->COUNTERREGS.CTRCTL &
             ~GPTIMER_CTRCTL_EN_MASK) |
            GPTIMER_CTRCTL_EN_ENABLED;
    }
}


// TIMG0 interrupt
// Generates CLK and reads the ADC
void TIMG0_IRQHandler(void)
{
    if (TIMG0->CPU_INT.MIS & GPTIMER_CPU_INT_MIS_Z_MASK)
    {
        // Clear interrupt
        TIMG0->CPU_INT.ICLR =
            GPTIMER_CPU_INT_ICLR_Z_CLR;

        if (pixelCounter < 128)
        {
            // CLK high
            GPIOA->DOUTSET31_0 = CAMERA_CLK_PIN;

            // Read pixel
            cameraData[pixelCounter] = ADC0_getVal();
            pixelCounter++;

            // CLK low
            GPIOA->DOUTCLR31_0 = CAMERA_CLK_PIN;
        }

        // CLK low
        GPIOA->DOUTCLR31_0 = CAMERA_CLK_PIN;

        // 128 pixels collected
        if (pixelCounter >= 128)
        {
            cameraData_complete = true;

            TIMG0->COUNTERREGS.CTRCTL =
                (TIMG0->COUNTERREGS.CTRCTL &
                 ~GPTIMER_CTRCTL_EN_MASK) |
                GPTIMER_CTRCTL_EN_DISABLED;

            pixelCounter = 0;
        }
    }
}



// Check if camera data is ready
uint8_t Camera_isDataReady(void)
{
    return cameraData_complete;
}


// Get the camera data
uint16_t* Camera_getData(void)
{
    cameraData_complete = false;

    return cameraData;
}
