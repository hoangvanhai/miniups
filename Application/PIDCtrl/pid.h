/** @FILE NAME:    pid.h
 *  @DESCRIPTION:  This file for ...
 *
 *  Copyright (c) 2018 EES Ltd.
 *  All Rights Reserved This program is the confidential and proprietary
 *  product of EES Ltd. Any Unauthorized use, reproduction or transfer
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
#ifndef APPLICATION_PIDCTRL_PID_H_
#define APPLICATION_PIDCTRL_PID_H_


/***************************** Include Files *********************************/
#include <IQmathlib.h>
#include <typedefs.h>
#include <AppConfig.h>
/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/
typedef struct SPID_ {
    _iq         KP;
    _iq         KI;
    _iq         KD;
    _iq         currErr;
    _iq         prevErr;
    _iq         Integration;
    _iq         Derative;
    _iq         Proportional;
    _iq         PIDOut;
}SPID;

/***************** Macros (Inline Functions) Definitions *********************/
#define PID_Reset(pid) {                        \
                        pid->currErr = 0;       \
                        pid->prevErr = 0;       \
                        pid->Integration = 0;   \
                        pid->Derative = 0;      \
                        pid->Proportional = 0;  \
                        pid->PIDOut = 0;        \
                      }


#define PID_ProcessM(pPid, setPoint, feedBack)                   {                      \
    (pPid)->currErr       = setPoint - feedBack;                                        \
    (pPid)->Proportional  = _IQ24mpy((pPid)->KP, (pPid)->currErr);                      \
    (pPid)->Integration  += _IQ24mpy((pPid)->KI, (pPid)->currErr + (pPid)->prevErr);    \
    (pPid)->Derative      = _IQ24mpy((pPid)->KD, (pPid)->currErr - (pPid)->prevErr);    \
    (pPid)->prevErr       = (pPid)->currErr;                        \
    (pPid)->PIDOut        = (pPid)->Proportional + (pPid)->Integration + (pPid)->Derative; \
}

/************************** Function Prototypes ******************************/
void PID_Init(SPID *pPid, _iq Kp, _iq Ki, _iq Kd, _iq T);
void PID_Process(SPID *pPid, _iq setPoint, _iq feedBack);

/************************** Variable Definitions *****************************/

/*****************************************************************************/



#endif /* APPLICATION_PIDCTRL_PID_H_ */
