/** @FILE NAME:    booster.c
 *  @DESCRIPTION:  This file for ...
 *
 *  Copyright (c) 2018 EES Ltd.
 *  All Rights Reserved This program is the confidential and proprietary
 *  product of ViettelR&D. Any Unauthorized use, reproduction or transfer
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
#include "booster.h"
#include <BSP.h>
#include <utils.h>

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/
extern __interrupt void epwm3_isr(void);
extern __interrupt void epwm4_isr(void);
/************************** Variable Definitions *****************************/

/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
BRet Boost_Init(SBooster *pBst) {
    pBst->eState        = BS_IDLE;
    pBst->freq          = Boost_SW_Freq;
    pBst->period        = Boost_SW_Period;
    pBst->periodIQ      = _IQ18(pBst->period);
    pBst->dutyv         = 0;
    pBst->dutyMax       = _IQ18div(_IQ(Boost_Duty_Coeff), _IQ(2));
    pBst->setVolt       = _IQ18(Boost_Volt_Output);
#if Boost_Method_Used == Boost_Method_Open_Loop
    pBst->aCoeff        = _IQ(BOOST_A_COEFF);
    pBst->bCoeff        = _IQ(BOOST_B_COEFF);
#elif Boost_Method_Used == Boost_Method_Close_Loop
    PID_Init(&pBst->sPid, _IQ(Boost_Kp), _IQ(Boost_Ki), _IQ(Boost_Kd), _IQ(0.001));
#endif
    pBst->pwmAHandle    = (PWM_REGS *)&EPwm3Regs;   //(PWM_REGS *)&EPwm1Regs;
    pBst->pwmBHandle    = (PWM_REGS *)&EPwm4Regs;   //(PWM_REGS *)&EPwm2Regs;
    pBst->phaseDiff     = Boost_SW_Period / 2;

    PWM_Boost_Init(pBst->pwmAHandle, pBst->pwmBHandle, pBst->freq, pBst->phaseDiff);
    return BR_OK;
}

/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
BRet Boost_Apply(SBooster *pBst) {
    if(PWM_2ChUpDownBoostSetDuty(pBst->pwmAHandle, pBst->dutyv) != PWM_SUCCESS) {
        return BR_FAIL;
    }
    if(PWM_2ChUpDownBoostSetDuty(pBst->pwmBHandle, pBst->dutyv) != PWM_SUCCESS) {
        return BR_FAIL;
    }
    return BR_OK;
}

#define PWM_SETTING_USE_PWM_ALL
#ifdef PWM_SETTING_USE_PWM_ALL
/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
void PWM_Boost_Init(PWM_REGS *pwmA, PWM_REGS *pwmB, uint32_t freq, uint16_t phase) {

    EALLOW;
    GpioCtrlRegs.GPAPUD.bit.GPIO4 = 1;    // Disable pull-up on GPIO4 (EPWM3A)
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // Configure GPIO4 as EPWM3A

    GpioCtrlRegs.GPAPUD.bit.GPIO5 = 1;    // Disable pull-up on GPIO5 (EPWM3B)
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 1;   // Configure GPIO5 as EPWM3B


    GpioCtrlRegs.GPAPUD.bit.GPIO6 = 1;    // Disable pull-up on GPIO6 (EPWM4A)
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;   // Configure GPIO6 as EPWM4A

    GpioCtrlRegs.GPAPUD.bit.GPIO7 = 1;    // Disable pull-up on GPIO6 (EPWM4B)
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;   // Configure GPIO6 as EPWM4A

    EDIS;

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;

    PWM_2ChCntUpDownBoostCfg(pwmA, freq, 0, 0, 1);   // PWM1 Inverter is setting as master,
                                                     // all others must set as slave
    PWM_2ChCntUpDownBoostCfg(pwmB, freq, 0, 0, 2);   // PWM1 Inverter is setting as master,

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;

}

#endif


/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
BRet Boost_Stop(SBooster *pBst) {
    pBst->eState = BS_IDLE;

    if(PWM_2ChUpDownBoostSetDuty(pBst->pwmAHandle, 0) != PWM_SUCCESS) {
        return BR_FAIL;
    }
    if(PWM_2ChUpDownBoostSetDuty(pBst->pwmBHandle, 0) != PWM_SUCCESS) {
        return BR_FAIL;
    }

    return BR_OK;
}
/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
BRet Boost_Set(SBooster *pBst, _iq percen) {
    pBst->dutyv = _IQint(_IQmpy(percen, pBst->periodIQ));
    return Boost_Apply(pBst);
}

/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
BRet Boost_Start(SBooster *pBst) {
    pBst->eState = BS_RUNNING;
    Boost_SetM(pBst, _IQ(Boost_Start_Percen_F));
    return BR_OK;
}

/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
BRet Boost_Process(SBooster *pBst, _iq measVoltage) {
    _iq duty = 0;
#if Boost_Method_Used == Boost_Method_Open_Loop
    duty = _IQdiv(pBst->bCoeff - measVoltage, pBst->aCoeff);
#elif Boost_Method_Used == Boost_Method_Close_Loop
    PID_ProcessM(&pBst->sPid, pBst->setVolt, measVoltage);
    duty = pBst->sPid.PIDOut;
#endif

    duty = MIN(duty, pBst->dutyMax);
    Boost_SetM(pBst, duty);
    return BR_OK;
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

/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */


