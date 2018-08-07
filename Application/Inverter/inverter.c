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
extern __interrupt void epwm1_tzint_isr(void);
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
    pInv->freq          = Inverter_Pwm_Freq;
    pInv->period        = (uint16_t)((FTBCLK / Inverter_Pwm_Freq));
    pInv->dutyv         = 0;
    pInv->pwm1Handle    = (PWM_REGS *)&EPwm1Regs;
    pInv->pwm2Handle    = (PWM_REGS *)&EPwm2Regs;
    PWM_Inv_Init(pInv->pwm1Handle, pInv->pwm2Handle, pInv->freq);
    pInv->genSinRatio   = Inverter_GenSin_Freq_Ratio;

    Sin_Init(&pInv->sSine1Phase, Inverter_Sin_Freq,
             Inverter_Pwm_Freq,
             _IQ24((Inverter_Sin_Freq * 2 * PI) / ((4 * Inverter_Pwm_Freq) / (5 * Inverter_GenSin_Freq_Ratio))),
             _IQ24(Inverter_Start_Mf), _IQ24(1.0));

    pInv->gainStep       = _IQ24(Inverter_Step_Mf);         // set gain step is 0.01
    pInv->gainMax        = _IQ24(Inverter_Max_Mf);
    pInv->aCoeff         = _IQ18(A_COEFF);
    pInv->bCoeff         = _IQ18(B_COEFF);
    pInv->currFbFact     = _IQ24(Inverter_Feedback_Curr_Rat);
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
    SINE1PHASE_RESET(&pInv->sSine1Phase);
    pInv->sSine1Phase.gain      = _IQ24(Inverter_Start_Mf);
    pInv->eState                = INV_RUNNING;
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
#if Inverter_Switching_Type == Inverter_Type_Open_Half
    PWM_2ChCntUpSetDutyHalf(pInv->pwm1Handle, 1, 0);
    PWM_2ChCntUpSetDutyHalf(pInv->pwm2Handle, 2, 0);
#elif Inverter_Switching_Type == Inverter_Type_Open_Full
    pInv->pwm1Handle->CMPA.half.CMPA = 0;
    pInv->pwm2Handle->CMPA.half.CMPA = 0;
#endif
    LREP("Stop Inverter\r\n");
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
    PWM_2ChCntUpDownFullCfg(pwm1, freq, 1, 0);
    PWM_2ChCntUpDownFullCfg(pwm2, freq, 0, 0);


    COMP_ModuleConfig((struct COMP_REGS*)&Comp1Regs,
                      _IQ18int(_IQ18(1023 * Inverter_SC_Protect_Value *
                                     Adc_Inverter_Shunt_Volt_Rat / Adc_Reference_Volt)));

    PWM_ModuleConfigTripZone(pwm1);
    PWM_ModuleConfigTripZone(pwm2);


    // Enable EPWM INTn in the PIE: Group 2 interrupt 1-3

    EALLOW;            // This is needed to write to EALLOW protected registers

    PieCtrlRegs.PIEIER2.bit.INTx1   = 1;
    PieVectTable.EPWM1_TZINT        = &epwm1_tzint_isr;
    IER |= M_INT2;
    EDIS;               // This is needed to disable write to EALLOW protected registers


#ifdef TEST_INV_PWM_SETTING
    PWM_2ChCntUpSetDutyFull(pwm1, 1, 300, 1);
    PWM_2ChCntUpSetDutyFull(pwm2, 2, 300, 1);
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
    if(_IQ24abs(gain) > _IQ24(1.0))
        return;
    pInv->sSine1Phase.gain = _IQ24abs(gain);
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



