/*
 * inverter.h
 *
 *  Created on: Jul 17, 2018
 *      Author: PC
 */

#ifndef INVERTER_INVERTER_H_
#define INVERTER_INVERTER_H_


/***************************** Include Files *********************************/
#include <IQMathlib.h>
#include <typedefs.h>
#include <DSP28x_Project.h>
#include <BSP.h>
#include <SineGen.h>
/************************** Constant Definitions *****************************/


/**************************** Type Definitions *******************************/
typedef enum EInverterMode_ {
    INV_SET_HALF_HIGH = 0,
    INV_SET_HALF_LOW
}EInverterMode;

typedef enum EInverterState_ {
    INV_IDLE = 0,
    INV_RUNNING,
    INV_ERROR
}EInverterState;

typedef struct SInverter_ {
    EInverterState  eState;
    EInverterMode   eMode;
    PWM_REGS    *pwm1Handle;
    PWM_REGS    *pwm2Handle;
    SSin1Phase  *pSinGen;
    uint32_t    freq;
    uint16_t    period;
    uint16_t    dutyv;          // duty value
    _iq         gainStep;
}SInverter;

/***************** Macros (Inline Functions) Definitions *********************/
#define Inv_SetMode(pInv, mode)     (pInv)->eMode = mode
#define Inv_SetState(pInv, state)   (pInv)->eState = state
#define Inv_GetGain(pInv)           (pInv)->pSinGen->gain
#define Inv_GetGainStep(pInv)       (pInv)->gainStep

/************************** Function Prototypes ******************************/
void Inv_Init(SInverter *pInv);
void Inv_Start(SInverter *pInv);
void Inv_Stop(SInverter *pInv);
void Inv_Set(SInverter *pInv, uint16_t chan, _iq percen);
void Inv_SetGain(SInverter *pInv, _iq gain);
void Inv_Apply(SInverter *pInv);
void PWM_Inv_Init(PWM_REGS * pwm1, PWM_REGS * pwm2, uint32_t freq);


/************************** Variable Definitions *****************************/
extern SInverter *pThisInv;
/*****************************************************************************/


#endif /* INVERTER_INVERTER_H_ */
