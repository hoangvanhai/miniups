
/***************************** Include Files *********************************/
#include <DSP28x_Project.h>     // Device Headerfile and Examples Include File
#include <BSP.h>
#include <app.h>
#include <Timer.h>
#include <IQmathLib.h>

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
    (void)timeTick;
    (void)p_arg;
//    static uint16_t count = 0;
//    count++;
//    if(count % 5 == 0) {
//        Inv_Start(&sApp.sInverter);
//    } else {
//        Inv_Stop(&sApp.sInverter);
//    }
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

#define PI      3.1415926535897932384626433832795
#define PI2     1.570796326794897
#define PIIQ    _IQ(PI)
#define PI2IQ   _IQ(PI2)
#define DEG     _IQ(0.0174532925199433)

void main(void)
{
    //_iq sinValue = 0;
    //int i = 0;

    //SINE1PHASE_RESET(sSine1Phase);
    //sSine1Phase.stepD = 20;


    BSP_Init();
    App_Init(&sApp);

//    for(; i < 18; i++) {
//        sinValue = Sin_GenValue(&sSine1Phase);
//        if(sinValue >= 0) {
//            Inv_Set(&sApp.sInverter, 2, 0);
//            Inv_Set(&sApp.sInverter, 1, sinValue);
//        } else {
//            Inv_Set(&sApp.sInverter, 1, 0);
//            Inv_Set(&sApp.sInverter, 2, sinValue);
//        }
//
//    }


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




