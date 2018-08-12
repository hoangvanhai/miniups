/** @FILE NAME:    main.c
 *  @DESCRIPTION:  This file for ...
 *
 *  Copyright (c) 2018 EES Ltd.
 *  All Rights Reserved This program is the confidential and proprietary
 *  product of EES Ltd. Any Unauthorized use, reproduction or transfer
 *  of this program is strictly prohibited.
 *
 *  @Author: HaiHoang
 *  @NOTE:   No Note at the moment
 *  @BUG:    No known bugs.
 *
 *<pre>
 *  MODIFICATION HISTORY:
 *
 *  Ver   Who       Date                Changes
 *  ----- --------- ------------------  ----------------------------------------
 *  1.00  HaiHoang  August 1, 2018      First release
 *
 *
 *</pre>
 ******************************************************************************/

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
    static uint16_t duty = 0;
    (void)timeTick;
    (void)p_arg;

    duty += 10;

    if(duty >= sApp.sBooster.pwmAHandle->TBPRD - 20) duty = 0;

    PWM_2ChUpDownBoostSetDuty(sApp.sBooster.pwmAHandle, duty);
    PWM_2ChUpDownBoostSetDuty(sApp.sBooster.pwmBHandle, duty);

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


//static long max  = -32767, min = 32767, tmp = 0;

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

    while(1) {
        DELAY_US(70000);
        //DELAY_US(15000);

        if(EPwm1Regs.TZFLG.bit.INT == 1) {
            EALLOW;
            EPwm1Regs.TZCLR.bit.INT = 1;
            EDIS;
        }

        if(EPwm3Regs.TZFLG.bit.INT == 1) {
            EALLOW;
            EPwm3Regs.TZCLR.bit.INT = 1;
            EDIS;
        }

//        GPIO_TOGGLE_DISP_BATT_LOW();
//        GPIO_TOGGLE_DISP_UPS_RUN();
//        GPIO_TOGGLE_DISP_ACIN();
//        GPIO_TOGGLE_CTRL_RELAY();
//        GPIO_TOGGLE_FAN_ON();

        //ServiceDog();
//        LREP("adc: %d voltage: %d - gain: 0.%d\r\n", (long)AdcResult.ADCRESULT1, _IQ18int(sApp.boostVolt.realValue),
//             _IQ20int(_IQ20mpyIQX(sApp.sInverter.sSine1Phase.gain, 24, _IQ20(1000), 20)));

//        LREP("voltage: %d - gain: 0.%d\r\n", _IQ18int(_IQ18mpy(sApp.boostVolt.realValue, _IQ18(100))),
//                     _IQ20int(_IQ20mpyIQX(sApp.sInverter.sSine1Phase.gain, 24, _IQ20(1000), 20)));

//        LREP("adc0: %d adc1: %d adc2: %d adc3: %d adc4: %d adc5: %d adc6: %d\r\n",
//             (long)AdcResult.ADCRESULT0, (long)AdcResult.ADCRESULT1,
//             (long)AdcResult.ADCRESULT2, (long)AdcResult.ADCRESULT3,
//             (long)AdcResult.ADCRESULT4, (long)AdcResult.ADCRESULT5,
//             (long)AdcResult.ADCRESULT6);

        LREP("adc4: %d - current: %d mA\r\n", (long)AdcResult.ADCRESULT4,
             (long)_IQ18int(_IQ18mpy(sApp.boostCurr.realValue, _IQ18(1000))));


//        LREP("ADC: %d - V: %d\r\n", (long)AdcResult.ADCRESULT1,
//             _IQ18int(sApp.boostVolt.realValue));

//        LREP("C: 0.%d - V: %d - G: 0.%d STAT: 0x%x\r\n",
//             _IQ18int(_IQ18mpy(sApp.inverterCurr.realValue, _IQ18(100))),
//             _IQ18int(sApp.boostVolt.realValue),
//             _IQ20int(_IQ20mpyIQX(sApp.sInverter.sSine1Phase.gain, 24, _IQ20(1000), 20)),
//             (long)sApp.eDevState);


//        LREP("TZSEL: %x - TZDCSEL: %x - TZCTL: %x - TZEINT: %x - TZFLG: %x - TZCLR: %x - TZFRC: %x\r\n",
//             (long)EPwm1Regs.TZSEL.all, (long)EPwm1Regs.TZDCSEL.all, (long)EPwm1Regs.TZCTL.all,
//             (long)EPwm1Regs.TZEINT.all, (long)EPwm1Regs.TZFLG.all, (long)EPwm1Regs.TZCLR.all, (long)EPwm1Regs.TZFRC.all);

        //LREP("COMPSTS %x - DACVAL: %x \r\n", (long)Comp1Regs.COMPSTS.bit.COMPSTS, (long)Comp1Regs.DACVAL.bit.DACVAL);
#if 0
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




