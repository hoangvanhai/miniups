/*
 * inverter.c
 *
 *  Created on: Jul 17, 2018
 *      Author: PC
 */



/***************************** Include Files *********************************/
#include "inverter.h"
#include <SineGen.h>

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */

void Inv_Init(SInverter *pInv) {
    pInv->eState        = INV_IDLE;
    pInv->freq          = Base_Freq;
    pInv->period        = (uint16_t)((FTBCLK / Base_Freq) - 1);
    pInv->dutyv         = 0;
    pInv->pwm1Handle    = (PWM_REGS *)&EPwm1Regs;
    pInv->pwm2Handle    = (PWM_REGS *)&EPwm2Regs;
    PWM_Inv_Init(pInv->pwm1Handle, pInv->pwm2Handle, pInv->freq);
    pInv->pSinGen = &sSine1Phase;

    SINE1PHASE_RESET(pInv->pSinGen);
    pInv->pSinGen->cFreq   = 1000;
    pInv->pSinGen->freq    = Inverter_Output_Freq;

    //sSine1Phase.stepD = 360 / (Inverter_Update_Freq / Inverter_Output_Freq);

    pInv->pSinGen->stepD   = _IQ(Inverter_Step_Angle);    // Interrupt freq = 25Khz =>
                                        // freq of sin = 25k / 50 = 500 =>
                                        // 1 interrupt inc 0.72 degree
    pInv->pSinGen->gain    = _IQ(Inverter_Start_Mf);
    pInv->gainStep         = _IQ(Inverter_Step_Mf);         // set gain step is 0.01
}

/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
void Inv_Start(SInverter *pInv) {
    SINE1PHASE_RESET(pInv->pSinGen);
    pInv->pSinGen->gain    = _IQ(Inverter_Start_Mf);
    pInv->eState        = INV_RUNNING;
}

/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
void Inv_Stop(SInverter *pInv) {
    pInv->eState = INV_IDLE;
    PWM_2ChCntUpSetDutyFull(pInv->pwm1Handle, 1, 0, 1);
    PWM_2ChCntUpSetDutyFull(pInv->pwm2Handle, 1, 0, 1);
}

/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note @@@ maybe only one pwm module is master on whole chip so this two pwm
 *  module must config is slave
 */

#define TEST_PWM_DEAD_BAND

void PWM_Inv_Init(PWM_REGS * pwm1, PWM_REGS * pwm2, uint32_t freq) {

    EALLOW;
    // PWM1H
    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;    // Disable pull-up on GPIO0 (EPWM1A)
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // Configure GPIO0 as EPWM1A

    // PWM1L
    GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;    // Disable pull-up on GPIO1 (EPWM1B)
    GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 1;   // Configure GPIO1 as EPWM1B

    // PWM2H
    GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1;    // Disable pull-up on GPIO2 (EPWM2A)
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // Configure GPIO2 as EPWM2A

    // PWM2L
    GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;    // Disable pull-up on GPIO3 (EPWM2B)
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 1;   // Configure GPIO3 as EPWM2B
    EDIS;

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;

#if Inverter_Switching_Type == Inverter_Type_Open_Half
    PWM_2ChCntUpHalfCfg(pwm1, freq, 1, 0);
    PWM_2ChCntUpHalfCfg(pwm2, freq, 0, 0);
#elif Inverter_Switching_Type == Inverter_Type_Open_Full
    PWM_2ChCntUpDownFullCfg(pwm1, freq, 0, 0);
    PWM_2ChCntUpDownFullCfg(pwm2, freq, 0, 0);

#ifdef TEST_INV_PWM_SETTING
//    PWM_2ChCntUpSetDutyFull(pwm1, 1, 200, 0);
//    PWM_2ChCntUpSetDutyFull(pwm2, 2, 200, 0);
#endif

#endif

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;
}

/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
void Inv_Set(SInverter *pInv, uint16_t chan, _iq percen) {
    uint16_t other = chan == 1 ? 2 : 1;
    pInv->dutyv = _IQint(_IQmpy(percen, _IQ(pInv->period)));
    PWM_2ChCntUpSetDutyHalf(pInv->pwm1Handle, chan, pInv->dutyv);
    PWM_2ChCntUpSetDutyHalf(pInv->pwm2Handle, other, pInv->dutyv);
}

/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
void Inv_SetGain(SInverter *pInv, _iq gain) {
    if(gain > _IQ(1.0))
        return;
    pInv->pSinGen->gain = gain;
}

/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */


/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */



