/*
 * booster.h
 *
 *  Created on: Jul 17, 2018
 *      Author: PC
 */

#ifndef BOOSTER_BOOSTER_H_
#define BOOSTER_BOOSTER_H_


/***************************** Include Files *********************************/
#include <IQMathlib.h>
#include <typedefs.h>
#include <DSP28x_Project.h>
#include <BSP.h>
/************************** Constant Definitions *****************************/

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
    _iq         dutyp;          // duty percen
}SBooster;
/***************** Macros (Inline Functions) Definitions *********************/
#define Boost_SetState(pBst, state)     (pBst)->eState = state


/************************** Function Prototypes ******************************/
BRet Boost_Init(SBooster *pBst);
BRet Boost_Apply(SBooster *pBst);
BRet Boost_Set(SBooster *pBst, _iq percen);
BRet Boost_Stop(SBooster *pBst);
BRet Boost_Start(SBooster *pBst);
void PWM_Boost_Init(PWM_REGS * pwmA, PWM_REGS * pwmB, uint32_t freq, uint16_t phase);

/************************** Variable Definitions *****************************/

/*****************************************************************************/


#endif /* BOOSTER_BOOSTER_H_ */
