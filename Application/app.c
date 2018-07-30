/*
 * app.c
 *
 *  Created on: Jul 17, 2018
 *      Author: PC
 */



/***************************** Include Files *********************************/
#include <app.h>
#include <BSP.h>
#include <Timer.h>
/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/
#define App_StartBstInv(pApp)       { \
                                        Inv_Start(&(pApp)->sInverter);      \
                                        Boost_Start(&(pApp)->sBooster);}

#define App_StopBstInv(pApp)        { \
                                        Boost_Stop(&(pApp)->sBooster);      \
                                        Inv_Stop(&(pApp)->sInverter); }

/************************** Function Prototypes ******************************/
static void Clb_AppControl(uint32_t tick, void *p_arg);
/************************** Variable Definitions *****************************/
SApp sApp;

int             ChSel[ADC_NUM_CHAN_MAX] =   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int             TrigSel[ADC_NUM_CHAN_MAX] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int             ACQPS[ADC_NUM_CHAN_MAX] =   {8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8};
volatile
uint16_t        Result[ADC_NUM_CHAN_USED];

/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
void App_Init(SApp *pApp) {
    // init state for device
    uint32_t devStateInit = 0;
    // application timer
    Timer_Init();
    // init booster
    Boost_Init(&pApp->sBooster);
    // init inverter
    Inv_Init(&pApp->sInverter);
    // init app instance
    pApp->counterAdc = 0;
    pApp->counterCtrl = 0;
    pApp->eDevState = DS_ERR_UNKNOWN;

    devStateInit = DS_READY_RUN;
//    devStateInit =  DS_READY_RUN | DS_AC_AVAIL |
//                    DS_BATT_VOLT_LOW | DS_BOOST_VOLT_LOW |
//                    DS_ERR_BOOST_CURR | DS_ERR_INV_CURR;    // | DS_ERR_LOAD_VOLT;

    App_SetDevState(pApp, devStateInit);

    pApp->eDevSm = DSM_STOP_UPS;
    // init timer for control time event
    pApp->hTimer = Timer_Create(Clb_AppControl, pApp);
    Timer_SetRate(pApp->hTimer, 100);

    Adc_InitValue(pApp->battVolt, _IQ(1.0), 10, 10);
    Adc_InitValue(pApp->boostVolt, _IQ(Boost_Voltage_Adc_Coeff), 10, 10);
    Adc_InitValue(pApp->boostCurr, _IQ(1.0), 10, 10);
    Adc_InitValue(pApp->inverterCurr, _IQ(1.0), 10, 10);
    Adc_InitValue(pApp->lineDetectVolt, _IQ(1.0), 10, 10);
    Adc_InitValue(pApp->loadDetectVolt, _IQ(1.0), 10, 10);

#if Build_Option == Build_Boost_Only

#elif Build_Option == Build_Inverter_Fix
    pApp->battVolt.realValue = Batt_Over_Volt - 1;
    pApp->boostCurr.realValue = Boost_Trip_Curr / 2;
    pApp->boostVolt.realValue = Inverter_Dcbus_Over - 10 ;
    pApp->inverterCurr.realValue = Inverter_Output_Current_Trip - 10;
    pApp->lineDetectVolt.realValue =  AC_Detect_Level + 1;
    //pApp->loadDetectVolt.realValue =  AC_Detect_Level + 1;

#elif Build_Option == Build_Inverter_All

#elif Build_Option == Build_Ups_All

#endif



}
/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
static void Clb_AppControl(uint32_t tick, void *p_arg) {
    (void)tick;
    (void)p_arg;

}
/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
void App_ScanDigitalInput(SApp *pApp) {
    (void)pApp;

}

/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
void App_ScanAnalogInput(SApp *pApp) {
    (void)pApp;
}

/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
void App_Process(SApp *pApp) {



#if Build_Option == Build_Boost_Only     // only boost

    Adc_CalcValue(sApp.battVolt,        AdcResult.ADCRESULT0);
    Adc_CalcValue(sApp.boostVolt,       AdcResult.ADCRESULT1);
    //Adc_CalcValue(sApp.inverterCurr,    AdcResult.ADCRESULT2);
    Adc_CalcValue(sApp.boostCurr,       AdcResult.ADCRESULT3);
    //Adc_CalcValue(sApp.lineDetectVolt,  AdcResult.ADCRESULT4);
    //Adc_CalcValue(sApp.loadDetectVolt,  AdcResult.ADCRESULT5);

#elif Build_Option == Build_Inverter_Fix  // only inverter
    //Adc_CalcValue(sApp.battVolt,        AdcResult.ADCRESULT0);
    //Adc_CalcValue(sApp.boostVolt,       AdcResult.ADCRESULT1);
    //Adc_CalcValue(sApp.inverterCurr,    AdcResult.ADCRESULT2);
    //Adc_CalcValue(sApp.boostCurr,       AdcResult.ADCRESULT3);
    //Adc_CalcValue(sApp.lineDetectVolt,  AdcResult.ADCRESULT4);
    //Adc_CalcValue(sApp.loadDetectVolt,  AdcResult.ADCRESULT5);

#elif Build_Option == Build_Inverter_All // all feature regardless AC and LOAD
    Adc_CalcValue(sApp.battVolt,        AdcResult.ADCRESULT0);
    Adc_CalcValue(sApp.boostVolt,       AdcResult.ADCRESULT1);
    Adc_CalcValue(sApp.inverterCurr,    AdcResult.ADCRESULT2);
    Adc_CalcValue(sApp.boostCurr,       AdcResult.ADCRESULT3);
    Adc_CalcValue(sApp.lineDetectVolt,  AdcResult.ADCRESULT4);
    Adc_CalcValue(sApp.loadDetectVolt,  AdcResult.ADCRESULT5);
#elif Build_Option == Build_Ups_All      // all device feature

    Adc_CalcValue(sApp.battVolt,        AdcResult.ADCRESULT0);
    Adc_CalcValue(sApp.boostVolt,       AdcResult.ADCRESULT1);
    Adc_CalcValue(sApp.inverterCurr,    AdcResult.ADCRESULT2);
    Adc_CalcValue(sApp.boostCurr,       AdcResult.ADCRESULT3);
    Adc_CalcValue(sApp.lineDetectVolt,  AdcResult.ADCRESULT4);
    Adc_CalcValue(sApp.loadDetectVolt,  AdcResult.ADCRESULT5);
#endif


#if 0
    /*
     * CHECK CONDITION AND CONTROL
     */
    /*
     * Check battery voltage
     * if volt < Batt_Under_Volt or volt > Batt_Over_Volt and booster is running -> stop Booster
     * if volt > Batt_Under_Volt + Batt_Hyst_Volt and booster is stopping -> start booster
     * if
     */
    if(pApp->battVolt.realValue <= Batt_Under_Volt) { // under low volt
        // Set error low voltage
        App_SetDevState(pApp, DS_BATT_VOLT_LOW);
        GPIO_SET_HIGH_DISP_BATT_LOW();
    } else if(pApp->battVolt.realValue > Batt_Under_Volt &&
            pApp->battVolt.realValue <= (Batt_Under_Volt + Batt_Hyst_Volt)) { // upper low volt but under Hyst

        if(pApp->eDevState == DS_RUN_UPS) { //device is running -> keep running
            // Clear all error flag

        } else { // Set error low volt
            App_SetDevState(pApp, DS_BATT_VOLT_LOW);
            GPIO_SET_HIGH_DISP_BATT_LOW();
        }

    } else if(pApp->battVolt.realValue > (Batt_Under_Volt + Batt_Hyst_Volt) &&
            pApp->battVolt.realValue <= Batt_Over_Volt){
            // Clear battery error flag
            App_ClearDevState(pApp, DS_BATT_VOLT_HIGH);
            App_ClearDevState(pApp, DS_BATT_VOLT_LOW);

    } else if(pApp->battVolt.realValue > Batt_Over_Volt) {
        // Set error over volt
        App_SetDevState(pApp, DS_BATT_VOLT_HIGH);
    }

    /*
     * Check AC available
     */
    if(pApp->lineDetectVolt.realValue> AC_Detect_Level) { // Detect ac loss
        App_ClearDevState(pApp, DS_AC_AVAIL);
        GPIO_SET_LOW_DISP_ACIN();
    } else {    // Detect ac available
        App_SetDevState(pApp, DS_AC_AVAIL);
        GPIO_SET_HIGH_DISP_ACIN();
    }

    /*
     * Check boost voltage
     */
    if(pApp->boostVolt.realValue < Inverter_Dcbus_Under) { // Under voltage after boost
         App_SetDevState(pApp, DS_BOOST_VOLT_LOW);
    } else if(pApp->boostVolt.realValue > Inverter_Dcbus_Over) { // Over voltage after boost
        App_SetDevState(pApp, DS_BOOST_VOLT_HIGH);
    } else { // Normal voltage
        App_ClearDevState(pApp, DS_BOOST_VOLT_LOW);
        App_ClearDevState(pApp, DS_BOOST_VOLT_HIGH);
    }

    /*
     * Check boost current
     */
    if(pApp->boostCurr.realValue < Boost_Trip_Curr) { // Normal current
        App_ClearDevState(pApp, DS_ERR_BOOST_CURR);
    } else { // Over current after boost
        App_SetDevState(pApp, DS_ERR_BOOST_CURR);
    }

    /*
     * Check inverter current
     */
    if(pApp->inverterCurr.realValue < Inverter_Output_Current_Trip) { // Normal current
        App_ClearDevState(pApp, DS_ERR_INV_CURR);
    } else { // Over current after Inverter
        App_SetDevState(pApp, DS_ERR_INV_CURR);
    }

    /*
     * Check Load out available
     */
//    if(pApp->loadDetectVolt.realValue > AC_Detect_Level) { // Not detect Load voltage
//        App_SetDevState(pApp, DS_ERR_LOAD_VOLT);
//    } else {
//        App_ClearDevState(pApp, DS_ERR_LOAD_VOLT);
//    }

#endif

    /*
     * Wrappup, with all status of device, perform convenient actions
     */
    App_Control(pApp);
}


/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
void App_Control(SApp *pApp) {

    switch (pApp->eDevSm) {
    case DSM_STOP_UPS:
        if(pApp->eDevState == DS_READY_RUN) {
            //Boost_Start(&pApp->sBooster);
            GPIO_SET_HIGH_CTRL_RELAY();
            pApp->eDevSm = DSM_STARTING_BOOSTER;
        }
        break;
    case DSM_STARTING_BOOSTER:
        if(pApp->eDevState == DS_READY_RUN) {
            Inv_Start(&pApp->sInverter);
            pApp->eDevSm = DSM_STARTING_INVERTER;
            Inv_SetGain(&pApp->sInverter, _IQ(Inverter_Start_Mf));
        } else {
            Boost_Stop(&pApp->sBooster);
            GPIO_SET_LOW_CTRL_RELAY();
            pApp->eDevSm = DSM_STOP_UPS;
        }
        break;
    case DSM_STARTING_INVERTER:
        if(pApp->eDevSm == DSM_STARTING_INVERTER) {
            if(Inv_GetGain(&pApp->sInverter) < _IQ(Inverter_Max_Mf)) {
                Inv_SetGain(&pApp->sInverter, Inv_GetGain(&pApp->sInverter) +
                            Inv_GetGainStep(&pApp->sInverter));
            } else {
                pApp->eDevSm = DSM_RUNNING_UPS;
            }
            GPIO_SET_HIGH_DISP_UPS_RUN();
        } else {
            App_StopBstInv(pApp);
            GPIO_SET_LOW_DISP_UPS_RUN();
            GPIO_SET_LOW_CTRL_RELAY();
            pApp->eDevSm = DSM_STOP_UPS;
        }
        break;
    case DSM_RUNNING_UPS:
        if(pApp->eDevState != DS_READY_RUN) {
            App_StopBstInv(pApp);
            GPIO_SET_LOW_DISP_UPS_RUN();
            GPIO_SET_LOW_CTRL_RELAY();
            pApp->eDevSm = DSM_STOP_UPS;
        }

        break;

    default:
        break;
    }

}


/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */


