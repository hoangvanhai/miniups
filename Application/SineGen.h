/*
 * SineGen.h
 *
 *  Created on: Jul 25, 2018
 *      Author: PC
 */

#ifndef APPLICATION_SINEGEN_H_
#define APPLICATION_SINEGEN_H_


/***************************** Include Files *********************************/
#include <IQMathlib.h>
#include <typedefs.h>
/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

typedef struct Sine1Phase_ {
    uint32_t        freq;           // freq of sin wave
    uint32_t        cFreq;          // carier freq
    _iq             stepD;           // step of angle
    uint32_t        counter;        // counter of angle in degre
    _iq             angleD;          // angle in radian
    _iq             angleR;          // angle in radian
    _iq             gain;           // gain of value
    _iq             value;          // sin value at angle
}SSin1Phase;

/***************** Macros (Inline Functions) Definitions *********************/

#define SINE1PHASE_INIT(sin)  {    \
        sin->counter = 0;            \
        sin->angleD  = 0;              \
        sin->angleR  = 0;               \
        sin->gain    = 0;               \
        sin->value   = 0;}

#define SINE1PHASE_RESET(sin)  {        \
        sin->counter = 0;                \
        sin->angleD  = 0;                \
        sin->angleR  = 0;               \
        sin->value   = 0;               \
        sin->gain    = 0;}
/************************** Function Prototypes ******************************/

_iq Sin_GenValue(SSin1Phase *pSin);

/************************** Variable Definitions *****************************/
extern SSin1Phase sSine1Phase;
/*****************************************************************************/


#endif /* APPLICATION_SINEGEN_H_ */
