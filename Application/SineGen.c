/*
 * SineGen.c
 *
 *  Created on: Jul 25, 2018
 *      Author: PC
 */


/***************************** Include Files *********************************/
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
void Sin_GenValue(SSin1Phase *pSin) {

    pSin->angleA += pSin->angleRadUnit;

    if(pSin->angleA >= pSin->angle360) {
        pSin->angleA = 0;
    }

    pSin->angleB = pSin->angleA + pSin->angle120;

    if(pSin->angleB >= pSin->angle360) {
        pSin->angleB -= pSin->angle360;
    }

    pSin->angleC = pSin->angleA + pSin->angle240;

    if(pSin->angleC >= pSin->angle360) {
        pSin->angleC -= pSin->angle360;
    }

    pSin->sinPwmA = _IQ24sin(pSin->angleA);
    pSin->sinPwmA = pSin->offset + _IQ24mpy(pSin->sinPwmA, pSin->gain);

    pSin->sinPwmB = _IQ24sin(pSin->angleB);
    pSin->sinPwmB = pSin->offset + _IQ24mpy(pSin->sinPwmB, pSin->gain);

    pSin->sinPwmC = _IQ24sin(pSin->angleC);
    pSin->sinPwmC = pSin->offset + _IQ24mpy(pSin->sinPwmC, pSin->gain);
}


/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
void Sin_Init(SSin1Phase *pSin, uint32_t freqSin,
              uint32_t freqCarr, _iq stepRad, _iq gain, _iq offset) {
    SINE1PHASE_RESET(pSin);
    pSin->freqSin       = freqSin;
    pSin->freqGenSin   = freqCarr;
    pSin->gain          = gain;
    pSin->offset        = offset;
    pSin->angleRadUnit  = stepRad;
    pSin->angle120      = _IQ24(ANGLE_120);
    pSin->angle240      = _IQ24(ANGLE_240);
    pSin->angle360      = _IQ24(ANGLE_360);
}
