/*
 * pid.h
 *
 *  Created on: Jul 31, 2018
 *      Author: PC
 */

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
                        pid->Integration = 0;  \
                        pid->Derative = 0;      \
                        pid->Proportional = 0;  \
                        pid->PIDOut = 0;        \
                      }


#define PID_ProcessM(pPid, setPoint, feedBack)                   {   \
    (pPid)->currErr       = setPoint - feedBack;                    \
    (pPid)->Proportional  = _IQmpy((pPid)->KP, (pPid)->currErr);    \
    (pPid)->Integration  += _IQmpy((pPid)->KI, (pPid)->currErr + (pPid)->prevErr); \
    (pPid)->Derative      = _IQmpy((pPid)->KD, (pPid)->currErr - (pPid)->prevErr); \
    (pPid)->prevErr       = (pPid)->currErr;                        \
    (pPid)->PIDOut        = (pPid)->Proportional + (pPid)->Integration + (pPid)->Derative; \
}

/************************** Function Prototypes ******************************/
void PID_Init(SPID *pPid, _iq Kp, _iq Ki, _iq Kd, _iq T);
void PID_Process(SPID *pPid, _iq setPoint, _iq feedBack);

/************************** Variable Definitions *****************************/

/*****************************************************************************/



#endif /* APPLICATION_PIDCTRL_PID_H_ */
