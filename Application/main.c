
/***************************** Include Files *********************************/
#include <DSP28x_Project.h>     // Device Headerfile and Examples Include File
#include <BSP.h>
#include <app.h>
#include <Timer.h>
#include <IQmathLib.h>
#include <console.h>
#include <stdio.h>


/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/
TimerHandle testTimer = NULL;

/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
void Clb_Timer(uint32_t timeTick, void *p_arg) {
//    static uint16_t duty = 0;
    (void)timeTick;
    (void)p_arg;

//    duty += 10;
//
//    if(duty >= 800) duty = 0;
//
//    PWM_2ChUpDownBoostSetDuty(sApp.sBooster.pwmAHandle, duty);
//    PWM_2ChUpDownBoostSetDuty(sApp.sBooster.pwmBHandle, duty);

    GPIO_TOGGLE_RUN();
}

/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */


static long max  = -32767, min = 32767, tmp = 0;

void main(void)
{
    uint8_t buff[50];
    // Hardware Init
    BSP_Init();
    // Application Init
    App_Init(&sApp);

    LREP("\r\nHello world C2000 - MINIUPS application\r\n");
    LREP("*****************************************\r\n");

    testTimer = Timer_Create(Clb_Timer, NULL);
    Timer_SetRate(testTimer, 1000);
    Timer_Start(testTimer);

    // this start global interrupt for mcu
    BSP_Start();

    while(1) {
        DELAY_US(50000);
        //App_Control(&sApp);
        //ServiceDog();
        //LREP("adc: %d\r\n", (long)_IQ18int(_IQ18mpy(sApp.boostVolt.realValue, _IQ18(100))));

#if 1
        int16_t value;
        value = _IQ18int(sApp.boostVolt.realValue);
        if(max <  value)
        {
            max = value;
        }
        else if( min > value)
        {
            min = value;
        }
        tmp++;
        if (tmp == 10)
        {
            LREP("\r\n[Max %d - Min %d]", (long)max, (long)min);
            tmp = 0;
            max = -32767;
            min = 32767;
        }
#endif

    }
}




