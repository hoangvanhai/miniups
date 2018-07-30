/*
 * booster.c
 *
 *  Created on: Jul 17, 2018
 *      Author: PC
 */


/***************************** Include Files *********************************/
#include "booster.h"
#include <BSP.h>

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
    pBst->dutyv         = 0;
    pBst->dutyp         = 0;
    pBst->pwmAHandle    = (PWM_REGS *)&EPwm3Regs;
    pBst->pwmBHandle    = (PWM_REGS *)&EPwm4Regs;
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
    if(PWM_1ChCntUpSetDuty(pBst->pwmAHandle, pBst->dutyv) != PWM_SUCCESS) {
        return BR_FAIL;
    }
    if(PWM_1ChCntUpSetDuty(pBst->pwmBHandle, pBst->dutyv) != PWM_SUCCESS) {
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
void PWM_Boost_Init(PWM_REGS *pwmA, PWM_REGS *pwmB, uint32_t freq, uint16_t phase) {

    EALLOW;
    GpioCtrlRegs.GPAPUD.bit.GPIO4 = 1;    // Disable pull-up on GPIO4 (EPWM3A)
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 1;   // Configure GPIO4 as EPWM3A

    GpioCtrlRegs.GPAPUD.bit.GPIO6 = 1;    // Disable pull-up on GPIO6 (EPWM4A)
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;   // Configure GPIO6 as EPWM4A
    EDIS;

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;
    EDIS;

    PWM_1ChCntUpCfg(pwmA, freq, 0, 1, 0);
    PWM_1ChCntUpCfg(pwmB, freq, 0, 0, phase);

    PWM_1ChCntUpSetEvent(pwmA, ET_CTR_ZERO, 0);
    PWM_1ChCntUpSetEvent(pwmB, ET_CTR_ZERO, 0);

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;
    EDIS;

    GPIO_SET_LOW_DRV_EN1();
    GPIO_SET_LOW_DRV_EN2();

    EALLOW;
    PieCtrlRegs.PIEIER3.bit.INTx3 = 1;
    PieCtrlRegs.PIEIER3.bit.INTx4 = 1;

    PieVectTable.EPWM3_INT = &epwm3_isr;
    PieVectTable.EPWM4_INT = &epwm4_isr;

    IER |= M_INT3;
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
BRet Boost_Stop(SBooster *pBst) {
    pBst->eState = BS_IDLE;

    if(PWM_1ChCntUpSetDuty(pBst->pwmAHandle, 0) != PWM_SUCCESS) {
        return BR_FAIL;
    }
    if(PWM_1ChCntUpSetDuty(pBst->pwmBHandle, 0) != PWM_SUCCESS) {
        return BR_FAIL;
    }

    GPIO_SET_LOW_DRV_EN1();
    GPIO_SET_LOW_DRV_EN2();

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

//    _iq mult = 0, div = 0;
//    div = _IQdiv(percen, _IQ(100.0));
//    mult = _IQmpy(div, _IQ(pBst->period));
//    pBst->dutyv = _IQint(mult);
    pBst->dutyp = percen;
    pBst->dutyv = _IQint(_IQmpy(_IQdiv(percen, _IQ(100.0)), _IQ(pBst->period)));
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
    Boost_Set(pBst, _IQ(Boost_Start_Percen_F));
    return BR_OK;
}







