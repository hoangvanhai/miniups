/** @FILE NAME:    booster.h
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

#ifndef BOOSTER_BOOSTER_H_
#define BOOSTER_BOOSTER_H_


/***************************** Include Files *********************************/
#include <IQMathlib.h>
#include <typedefs.h>
#include <DSP28x_Project.h>
#include <BSP.h>
#include <pid.h>

/************************** Constant Definitions *****************************/
#define BOOST_A_COEFF       76.1249999999998
#define BOOST_B_COEFF       50.0
/**************************** Type Definitions *******************************/
typedef enum BoostRet_ {
    BR_OK = 0,
    BR_FAIL
}BRet;

typedef enum EBoostState_ {
    BS_IDLE = 0,
    BS_RUNNING,
    BS_ERROR
}EBoostState;



typedef struct SBooster_ {
    EBoostState eState;
    PWM_REGS    *pwmAHandle;
    PWM_REGS    *pwmBHandle;
    uint16_t    phaseDiff;
    uint32_t    freq;
    uint16_t    period;
    uint16_t    dutyv;          // duty value
    _iq         periodIQ;
    _iq         dutyMax;
    _iq         setVolt;
#if Boost_Method_Used == Boost_Method_Close_Loop
    SPID        sPid;
#elif Boost_Method_Used == Boost_Method_Open_Loop
    _iq         aCoeff;
    _iq         bCoeff;
#endif
}SBooster;
/***************** Macros (Inline Functions) Definitions *********************/
#define Boost_SetState(pBst, state)     (pBst)->eState = state

#define Boost_ApplyM(pBst) {\
    if(PWM_2ChUpDownBoostSetDuty((pBst)->pwmAHandle, (pBst)->dutyv) != PWM_SUCCESS) { \
        return BR_FAIL; \
    } \
    if(PWM_2ChUpDownBoostSetDuty((pBst)->pwmBHandle, (pBst)->dutyv) != PWM_SUCCESS) { \
        return BR_FAIL; \
    } \
}

#define Boost_SetM(pBst, percen) { \
    (pBst)->dutyv = _IQint(_IQmpy(percen, (pBst)->periodIQ)); \
    Boost_ApplyM(pBst); \
}

/************************** Function Prototypes ******************************/
BRet Boost_Init (SBooster *pBst);
BRet Boost_Apply(SBooster *pBst);
BRet Boost_Set(SBooster *pBst, _iq percen);
BRet Boost_Stop(SBooster *pBst);
BRet Boost_Start(SBooster *pBst);
BRet Boost_Process(SBooster *pBst, _iq measVoltage);
void PWM_Boost_Init(PWM_REGS * pwmA, PWM_REGS * pwmB, uint32_t freq, uint16_t phase);

/************************** Variable Definitions *****************************/

/*****************************************************************************/


#endif /* BOOSTER_BOOSTER_H_ */
