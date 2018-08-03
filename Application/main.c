
/***************************** Include Files *********************************/
#include <DSP28x_Project.h>     // Device Headerfile and Examples Include File
#include <BSP.h>
#include <app.h>
#include <Timer.h>
#include <IQmathLib.h>
#include <console.h>


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



void main(void)
{
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

//    _iq percen;
//    uint16_t period;
//    int16_t duty;
//
//    period = 10000;
//    percen = _IQ(13.5);
//    duty = _IQint(_IQdiv(_IQmpy(percen, period), 100));

//    _iq sinArray[180];
//
//    long i, j = 0;
//    long rad = 0;
//    for(i = 0; i < 180; i++, j+= 2) {
//        rad = _IQmpy(_IQ(j), DEG);
//        sinArray[i] = _IQsin(rad);
//    }




    while(1) {
        DELAY_US(1000000);
        //App_Control(&sApp);
        //ServiceDog();
    }
}




