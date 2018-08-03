/*
 * pid.c
 *
 *  Created on: Jul 31, 2018
 *      Author: PC
 */

/***************************** Include Files *********************************/
#include <pid.h>

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

void PID_Init(SPID *pPid, _iq Kp, _iq Ki, _iq Kd, _iq T) {
    PID_Reset(pPid);
    pPid->KP = Kp;
    pPid->KI = _IQmpy(Ki, _IQdiv(T, _IQ(2)));
    pPid->KD = _IQdiv(Kd, T);
}
/*****************************************************************************/
/** @brief
 *
 *  Error_value = setPoint - feedBack;
    P_part  = Kp * Error_value;                                 => KP = kp
    I_part += Ki * (Error_value + pre_Error_value) / 2 * T;     => KI = Ki * T / 2
    D_part  = Kd * (Error_value - pre_Error_value) / T;         => KD = Kd / T
    out    += P_part + I_part + D_part;

    pre_Error = Error


    P_part = Kp*(Error - pre_Error);
    I_part = 0.5*Ki*T*(Error + pre_Error);
    D_part = Kd/T*( Error - 2*pre_Error+ pre_pre_Error);
    Out = pre_out + P_part + I_part + D_part ;
    pre_pre_Error = pre_Error
    pre_Error = Error
    pre_Out = Out
 *
 *  @param
 *  @return Void.
 *  @note
 */

void PID_Process(SPID *pPid, _iq setPoint, _iq feedBack)     {
    (pPid)->currErr       = setPoint - feedBack;
    (pPid)->Proportional  = _IQmpy((pPid)->KP, (pPid)->currErr);
    (pPid)->Integration  += _IQmpy((pPid)->KI, (pPid)->currErr + (pPid)->prevErr);
    (pPid)->Derative      = _IQmpy((pPid)->KD, (pPid)->currErr - (pPid)->prevErr);
    (pPid)->prevErr       = (pPid)->currErr;
    (pPid)->PIDOut        = (pPid)->Proportional + (pPid)->Integration + (pPid)->Derative;
}

