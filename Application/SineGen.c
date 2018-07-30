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
#define PI          _IQ(3.1415926535897932384626433832795)
#define PI_TWO      _IQ(6.283185307179586)
#define DEG         _IQ(0.0174532925199433)

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/
SSin1Phase sSine1Phase;
/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
_iq Sin_GenValue(SSin1Phase *pSin) {
    pSin->angleD += pSin->stepD;
    pSin->angleR = _IQmpy(pSin->angleD, DEG);

    if(pSin->angleR >= PI_TWO) {
        pSin->angleD = 0;
        pSin->angleR = 0;
    }

    pSin->value = _IQmpy(_IQsin(pSin->angleR), pSin->gain);

    return pSin->value;
}

