/*
 * app.h
 *
 *  Created on: Jul 17, 2018
 *      Author: PC
 */

#ifndef APPLICATION_APP_H_
#define APPLICATION_APP_H_


/***************************** Include Files *********************************/
#include <booster.h>
#include <inverter.h>
#include <SineGen.h>
#include <utils.h>
#include <console.h>
#include <MATH_EMAVG_IQ_C.h>
/************************** Constant Definitions *****************************/

#define ADC_NUM_CHAN_MAX        16
#define ADC_NUM_CHAN_USED       6

/**************************** Type Definitions *******************************/

typedef enum EDeviceState_ {
    DS_RUN_UPS           = 0x0001,
    DS_STATE_UPS         = 0x0002,
    DS_AC_AVAIL          = 0x0004,
    DS_BATT_VOLT_LOW     = 0x0008,
    DS_BATT_VOLT_HIGH    = 0x0010,
    DS_BOOST_VOLT_LOW    = 0x0020,
    DS_BOOST_VOLT_HIGH   = 0x0040,
    DS_ERR_BOOST_CURR    = 0x0080,
    DS_ERR_INV_CURR      = 0x0100,
    DS_ERR_LOAD_VOLT     = 0x0200,
    DS_ERR_UNKNOWN       = 0x0000
}EDeviceState;



typedef enum EBattStatus_ {
    BATT_NORMAL = 0,
    BATT_LOW_VOLT,
    BATT_OVER_VOLT
}EBattStatus;

typedef enum EAcInStatus_ {
    ACIN_LOSS = 0,
    ACIN_AVAIL
}EAcInStatus;

typedef enum ELoadStatus_ {
    LOAD_NORMAL = 0,
    LOAD_ERROR
}ELoadStatus;

typedef enum EDeviceSM_ {
    DSM_STOP_UPS = 0,
    DSM_STARTING_BOOSTER,
    DSM_STARTING_INVERTER,
    DSM_RUNNING_UPS
}EDeviceSM;

typedef struct SAdcValue_{
    uint16_t        count;
    uint16_t        maxCount;
    uint32_t        totalValue;
    int16_t         avgValue;
    uint16_t        currValue;
    uint16_t        offset;
    _iq             realValue;
    _iq             coeff;
    SMATH_EMAVG_IQ_C sEMA;
}SAdcValue;

typedef struct SApp_ {
    EDeviceState    eDevState;
    EDeviceSM       eDevSm;

    SBooster        sBooster;
    SInverter       sInverter;

    uint16_t        counterAdc;
    uint16_t        counterCtrl;

    SAdcValue       battVolt;
    _iq             lastBattVolt;
    _iq             maxBattVolt;
    _iq             minBattVolt;

    SAdcValue       boostVolt;
    _iq             maxBoostVolt;
    _iq             minBoostVolt;
    _iq             lastBoostVolt;

    SAdcValue       boostCurr;
    _iq             maxBoostCurr;

    SAdcValue       inverterCurr;
    _iq             maxInverterCurr;

    SAdcValue       lineDetectVolt;
    _iq             maxLineDetectVolt;

    SAdcValue       loadDetectVolt;
    _iq             maxLoadDetectVolt;

    void            *hTimer;
}SApp;

/***************** Macros (Inline Functions) Definitions *********************/
#define App_BoostStart(pApp)            Boost_Start(&((pApp)->sBooster))
#define App_BoostStop(pApp)             Boost_Stop(&((pApp)->sBooster))
#define App_InvStart(pApp)              Inv_Start(&((pApp)->sInverter))
#define App_InvStop(pApp)               Inv_Stop(&((pApp)->sInverter))

#define App_SetDevState(pApp, state)    (pApp)->eDevState |= (state)
#define App_ClearDevState(pApp, state)  (pApp)->eDevState &= ~(state)

#if 0
#define Adc_CalcValue(adc, value)          { \
                                            adc.count++; \
                                            adc.currValue = value;  \
                                            adc.totalValue += value; \
                                            if(adc.count >= adc.maxCount) \
                                            { \
                                                adc.avgValue = (adc.totalValue / adc.maxCount) - adc.offset; \
                                                adc.avgValue = MAX(0, adc.avgValue); \
                                                adc.realValue = _IQ18mpy(_IQ18(adc.avgValue), adc.coeff); \
                                                adc.count = 0; \
                                                adc.totalValue = 0;\
                                            } \
                                            }
#endif

#if 1
#define Adc_CalcValue(adc, value)          { \
                                            adc.sEMA.In = _IQ20(value); \
                                            MATH_EMAVG_IQ_C(adc.sEMA); \
                                            adc.realValue = _IQ18mpyIQX(adc.coeff, 18, adc.sEMA.Out, 20); \
                                            }

#endif

#define Adc_InitValue(adc, coe, cnt, offs)          { \
                                            (adc).count = 0; \
                                            (adc).currValue = 0; \
                                            (adc).totalValue = 0; \
                                            (adc).maxCount = cnt; \
                                            (adc).avgValue = 0; \
                                            (adc).offset = offs; \
                                            (adc).realValue = 0; \
                                            (adc).coeff = coe; \
                     MATH_EMAVG_IQ_C_INIT(adc.sEMA, _IQ30(2*PI* Adc_Noise_Cuttoff_Freq / Adc_Sampling_Freq)); \
                                            }

/************************** Function Prototypes ******************************/
void App_Init(SApp *pApp);
void App_ScanDigitalInput(SApp *pApp);
void App_ScanAnalogInput(SApp *pApp);
void App_Process(SApp *pApp);
void App_Control(SApp *pApp);

/************************** Variable Definitions *****************************/
extern SApp sApp;

extern int     ChSel[ADC_NUM_CHAN_MAX];
extern int     TrigSel[ADC_NUM_CHAN_MAX];
extern int     ACQPS[ADC_NUM_CHAN_MAX];
extern volatile uint16_t Result[ADC_NUM_CHAN_USED];
/*****************************************************************************/




#endif /* APPLICATION_APP_H_ */
