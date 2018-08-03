/*
 * isr.c
 *
 *  Created on: Jul 16, 2018
 *      Author: PC
 */




/***************************** Include Files *********************************/
#include <BSP.h>
#include <DSP28x_Project.h>
#include <Timer.h>
#include <app.h>
#include <SineGen.h>

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

_iq sinValue = 0;
_iq lastSinValue = 0;
uint16_t duty;

/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */

__interrupt void epwm3_isr(void)
{
    if(sApp.sBooster.eState == BS_RUNNING &&
            EPwm3Regs.CMPA.half.CMPA > 0)
    {
        if(EPwm3Regs.ETSEL.bit.INTSEL == ET_CTR_ZERO) {
            GPIO_SET_HIGH_DRV_EN2();
            GPIO_SET_HIGH_DRV_EN1();
            EPwm3Regs.ETSEL.bit.INTSEL = ET_CTRU_CMPA;
        } else if(EPwm3Regs.ETSEL.bit.INTSEL == ET_CTRU_CMPA) {
            GPIO_SET_LOW_DRV_EN2();
            GPIO_SET_LOW_DRV_EN1();
            EPwm3Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;
        }
    }

    EPwm3Regs.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}



/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */

__interrupt void epwm4_isr(void)
{
    if(sApp.sBooster.eState == BS_RUNNING &&
            EPwm4Regs.CMPA.half.CMPA > 0)
    {
        if(EPwm4Regs.ETSEL.bit.INTSEL == ET_CTR_ZERO) {
            GPIO_SET_HIGH_DRV_EN2();
            GPIO_SET_HIGH_DRV_EN1();
            EPwm4Regs.ETSEL.bit.INTSEL = ET_CTRU_CMPA;
        } else if(EPwm4Regs.ETSEL.bit.INTSEL == ET_CTRU_CMPA) {
            GPIO_SET_LOW_DRV_EN2();
            GPIO_SET_LOW_DRV_EN1();
            EPwm4Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;
        }
    }

    EPwm4Regs.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}



/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
__interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;
    if(CpuTimer0.InterruptCount >= (APP_TIMER_PERIOD / Adc_Sampling_Period)) {
        sSysTick.u32SystemTickCount++;
        Timer_Update();
        CpuTimer0.InterruptCount = 0;
    }
    // Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

/*****************************************************************************/
/** @brief for high voltage, low current application
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
__interrupt void cpu_timer1_isr(void)
{
    CpuTimer1.InterruptCount++;

    if(sApp.sInverter.eState == INV_RUNNING) {

        Sin_GenValue(&sApp.sInverter.sSine1Phase);

        #if Inverter_Switching_Type == Inverter_Type_Open_Half
        if(lastSinValue > 0 && sinValue <= 0) {
            PWM_2ChCntUpSetDutyHalf(sApp.sInverter.pwm1Handle, 1, 0);
            PWM_2ChCntUpSetDutyHalf(sApp.sInverter.pwm2Handle, 2, 0);
        } else if(lastSinValue <= 0 && sinValue > 0){
            PWM_2ChCntUpSetDutyHalf(sApp.sInverter.pwm1Handle, 2, 0);
            PWM_2ChCntUpSetDutyHalf(sApp.sInverter.pwm2Handle, 1, 0);
        }

        duty = _IQint(_IQabs(_IQmpy(sApp.sInverter.sSine1Phase.sinPwmA,
                                    sApp.sInverter.periodIQ)));

        if(sinValue > 0) {
            PWM_2ChCntUpSetDutyHalf(sApp.sInverter.pwm1Handle, 1, duty);
            PWM_2ChCntUpSetDutyHalf(sApp.sInverter.pwm2Handle, 2, duty);
        } else {
            PWM_2ChCntUpSetDutyHalf(sApp.sInverter.pwm1Handle, 2, duty);
            PWM_2ChCntUpSetDutyHalf(sApp.sInverter.pwm2Handle, 1, duty);
        }

        lastSinValue = sinValue;

        #elif Inverter_Switching_Type == Inverter_Type_Open_Full

        sApp.sInverter.pwm1Handle->CMPA.half.CMPA =
                _IQ24mpy(sApp.sInverter.sSine1Phase.sinPwmA,
                         sApp.sInverter.pwm1Handle->TBPRD>>1);
        sApp.sInverter.pwm2Handle->CMPA.half.CMPA =
                _IQ24mpy(sApp.sInverter.sSine1Phase.sinPwmB,
                         sApp.sInverter.pwm2Handle->TBPRD>>1);

        #endif


    }

    //GPIO_TOGGLE_RUN();
}

/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
__interrupt void cpu_timer2_isr(void)
{
    CpuTimer2.InterruptCount++;
    if(sApp.eDevState == DS_RUN_UPS) {
        Boost_Process(&sApp.sBooster, sApp.boostVolt.realValue);
    }
}

/*****************************************************************************/
/** @brief read all adc value on channel, this routine triggered by cpu timer 0
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
__interrupt void adc_isr(void)
{
    //GPIO_TOGGLE_RUN();

    // Control all UPS behaviour
    App_Process(&sApp);

    // Clear ADCINT1 flag reinitialize for next SOC
    AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;

    // Acknowledge interrupt to PIE
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}












/*
 * END OF FILE
 */


