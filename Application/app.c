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
int             ACQPS[ADC_NUM_CHAN_MAX] =   {28,28,28,28,28,28,28,28,28,28,28,28,28,28,28,28};
long max  = -32767, min = 32767, tmp = 0;

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

    devStateInit = DS_RUN_UPS;
//    devStateInit =  DS_RUN_UPS | DS_AC_AVAIL |
//                    DS_BATT_VOLT_LOW | DS_BOOST_VOLT_LOW |
//                    DS_ERR_BOOST_CURR | DS_ERR_INV_CURR;    // | DS_ERR_LOAD_VOLT;

    App_SetDevState(pApp, devStateInit);

    pApp->eDevSm = DSM_STOP_UPS;
    // init timer for control time event
    pApp->hTimer = Timer_Create(Clb_AppControl, pApp);
    Timer_SetRate(pApp->hTimer, Inverter_Fault_Clear_Time * 2);

    // init setting up
    Adc_InitValue(pApp->battVolt,
                  _IQ18(1.0),   /* coefficient for calculate realvalue */
                  10,           /* average counter */
                  10);          /* offset value */



    Adc_InitValue(pApp->boostVolt,      _IQ18(Boost_Voltage_Adc_Coeff), 50, 10);

    Adc_InitValue(pApp->boostCurr,      _IQ18(1.0),           10, 10);

    Adc_InitValue(pApp->inverterCurr,   _IQ18(1.0),           10, 10);

    Adc_InitValue(pApp->lineDetectVolt, _IQ18(1.0),           10, 10);

    Adc_InitValue(pApp->loadDetectVolt, _IQ18(1.0),           10, 10);


    pApp->maxBattVolt           = _IQ18(Batt_Over_Volt);
    pApp->minBattVolt           = _IQ18(Batt_Under_Volt);
    pApp->maxBoostVolt          = _IQ18(Inverter_Dcbus_Over);
    pApp->minBoostVolt          = _IQ18(Inverter_Dcbus_Under);

    pApp->maxBoostCurr          = _IQ18(Boost_Trip_Curr);
    pApp->maxInverterCurr       = _IQ18(Inverter_Output_Current_Trip);
    pApp->maxLineDetectVolt     = _IQ18(AC_Detect_Level);
    pApp->maxLoadDetectVolt     = _IQ18(AC_Detect_Level);

    pApp->lastBoostVolt         = 0;
    pApp->lastBattVolt          = 0;

#if Build_Option == Build_Boost_Only
    pApp->battVolt.realValue            = pApp->maxBattVolt - _IQ(1);
    pApp->boostCurr.realValue           = pApp->maxBoostCurr - _IQ(200) ;
    pApp->boostVolt.realValue           = pApp->maxBoostVolt + _IQ(5) ;
    pApp->inverterCurr.realValue        = pApp->maxInverterCurr - _IQ(10);
    pApp->lineDetectVolt.realValue      = pApp->maxLineDetectVolt + _IQ(1);
    pApp->loadDetectVolt.realValue      = pApp->maxLoadDetectVolt - _IQ(1)
#elif Build_Option == Build_Inverter_Fix
    pApp->battVolt.realValue            = pApp->maxBattVolt - _IQ18(1);
    pApp->boostCurr.realValue           = pApp->maxBoostCurr - _IQ18(200) ;
    pApp->boostVolt.realValue           = pApp->maxBoostVolt - _IQ18(5) ;
    pApp->inverterCurr.realValue        = pApp->maxInverterCurr - _IQ18(10);
    pApp->lineDetectVolt.realValue      = pApp->maxLineDetectVolt + _IQ18(1);
    //pApp->loadDetectVolt.realValue    = pApp->maxLoadDetectVolt - _IQ(1)

#elif Build_Option == Build_Inverter_All
    pApp->battVolt.realValue            = pApp->maxBattVolt - _IQ(1);
    pApp->boostCurr.realValue           = pApp->maxBoostCurr - _IQ(200) ;
    pApp->boostVolt.realValue           = pApp->maxBoostVolt + _IQ(5) ;
    pApp->inverterCurr.realValue        = pApp->maxInverterCurr - _IQ(10);
    pApp->lineDetectVolt.realValue      = pApp->maxLineDetectVolt + _IQ(1);
    pApp->loadDetectVolt.realValue      = pApp->maxLoadDetectVolt - _IQ(1)
#elif Build_Option == Build_Ups_All
    pApp->battVolt.realValue            = 0;
    pApp->boostCurr.realValue           = 0;
    pApp->boostVolt.realValue           = 0;
    pApp->inverterCurr.realValue        = 0;
    pApp->lineDetectVolt.realValue      = 0;
    pApp->loadDetectVolt.realValue      = 0;
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
    if(sApp.eDevState == DS_RUN_UPS) {
        sApp.eDevSm = DSM_STARTING_BOOSTER;
    }
    Timer_Stop(sApp.hTimer);
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
    Adc_CalcValue(sApp.boostVolt,       AdcResult.ADCRESULT1);
    //Adc_CalcValue(sApp.inverterCurr,    AdcResult.ADCRESULT2);
    //Adc_CalcValue(sApp.boostCurr,       AdcResult.ADCRESULT3);
    //Adc_CalcValue(sApp.lineDetectVolt,  AdcResult.ADCRESULT4);
    //Adc_CalcValue(sApp.loadDetectVolt,  AdcResult.ADCRESULT5);


#elif Build_Option == Build_Inverter_All // all feature regardless AC and LOAD
    Adc_CalcValue(sApp.battVolt,        AdcResult.ADCRESULT0);
    Adc_CalcValue(sApp.boostVolt,       AdcResult.ADCRESULT1);
    Adc_CalcValue(sApp.inverterCurr,    AdcResult.ADCRESULT2);
    Adc_CalcValue(sApp.boostCurr,       AdcResult.ADCRESULT3);
    //Adc_CalcValue(sApp.lineDetectVolt,  AdcResult.ADCRESULT4);
    //Adc_CalcValue(sApp.loadDetectVolt,  AdcResult.ADCRESULT5);

#elif Build_Option == Build_Ups_All      // all device feature
    Adc_CalcValue(sApp.battVolt,        AdcResult.ADCRESULT0);
    Adc_CalcValue(sApp.boostVolt,       AdcResult.ADCRESULT1);
    Adc_CalcValue(sApp.inverterCurr,    AdcResult.ADCRESULT2);
    Adc_CalcValue(sApp.boostCurr,       AdcResult.ADCRESULT3);
    Adc_CalcValue(sApp.lineDetectVolt,  AdcResult.ADCRESULT4);
    Adc_CalcValue(sApp.loadDetectVolt,  AdcResult.ADCRESULT5);
#endif



#if 0
        long value;
        value = _IQ18int(pApp->boostVolt.realValue); //AdcResult.ADCRESULT1;
        if(max <  value)
        {
            max = value;
        }
        else if( min > value)
        {
            min = value;
        }
        tmp++;
        if (tmp == 5000)
        {
            LREP("[Max %d - Min %d]\r\n", (long)max, (long)min);
            tmp = 0;
            max = -32767;
            min = 32767;
        }

        return;
#endif

#if 1
    /*
     * CHECK CONDITION AND CONTROL
     */
    /*
     * Check battery voltage
     * if volt < Batt_Under_Volt or volt > Batt_Over_Volt and booster is running -> stop Booster
     * if volt > Batt_Under_Volt + Batt_Hyst_Volt and booster is stopping -> start booster
     */
    if(pApp->battVolt.realValue <= pApp->minBattVolt) { // under low volt
        // Set error low voltage
        App_SetDevState(pApp, DS_BATT_VOLT_LOW);
        GPIO_SET_HIGH_DISP_BATT_LOW();
    } else if(pApp->battVolt.realValue > pApp->minBattVolt &&
            pApp->battVolt.realValue <= (pApp->minBattVolt + _IQ18(Batt_Hyst_Volt))) { // upper low volt but under Hyst

        if(pApp->eDevState == DS_RUN_UPS) { //device is running -> keep running
            // Clear all error flag

        } else { // Set error low volt
            App_SetDevState(pApp, DS_BATT_VOLT_LOW);
            GPIO_SET_HIGH_DISP_BATT_LOW();
        }

    } else if(pApp->battVolt.realValue > (pApp->minBattVolt+ _IQ18(Batt_Hyst_Volt)) &&
            pApp->battVolt.realValue <= pApp->maxBattVolt){
            // Clear battery error flag
            App_ClearDevState(pApp, DS_BATT_VOLT_HIGH);
            App_ClearDevState(pApp, DS_BATT_VOLT_LOW);

    } else if(pApp->battVolt.realValue > pApp->maxBattVolt) {
        // Set error over volt
        App_SetDevState(pApp, DS_BATT_VOLT_HIGH);
    }

    /*
     * Check AC available
     */
    if(pApp->lineDetectVolt.realValue > pApp->maxLineDetectVolt) { // Detect ac loss
        App_ClearDevState(pApp, DS_AC_AVAIL);
        GPIO_SET_LOW_DISP_ACIN();
    } else {    // Detect ac available
        App_SetDevState(pApp, DS_AC_AVAIL);
        GPIO_SET_HIGH_DISP_ACIN();
    }

    /*
     * Check boost voltage
     */
    if(pApp->boostVolt.realValue < pApp->minBoostVolt) { // Under voltage after boost
         App_SetDevState(pApp, DS_BOOST_VOLT_LOW);
         //LREP("L");
    } else if(pApp->boostVolt.realValue > pApp->maxBoostVolt) { // Over voltage after boost
        App_SetDevState(pApp, DS_BOOST_VOLT_HIGH);
        //LREP("H");
    } else { // Normal voltage
        App_ClearDevState(pApp, DS_BOOST_VOLT_LOW);
        App_ClearDevState(pApp, DS_BOOST_VOLT_HIGH);
    }

    /*
     * Check boost current
     */
    if(pApp->boostCurr.realValue < pApp->maxBoostCurr) { // Normal current
        App_ClearDevState(pApp, DS_ERR_BOOST_CURR);
    } else { // Over current after boost
        App_SetDevState(pApp, DS_ERR_BOOST_CURR);
    }

    /*
     * Check inverter current
     */
    if(pApp->inverterCurr.realValue < pApp->maxInverterCurr) { // Normal current
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
        // if not yet kick start UPS event
        if(pApp->eDevState == DS_RUN_UPS) {
            if(!Timer_IsRunning(pApp->hTimer)) {
                // kick start
                Timer_StartAt(pApp->hTimer, Inverter_Soft_Start_Time);
                LREP("Soft start inverter\r\n");
            }
        } else { // while waitting starting time, error event occours
            if(Timer_IsRunning(pApp->hTimer))
                Timer_Stop(pApp->hTimer);
        }
        /* move to callback timer event for delay 100 ms before start UPS */
        /*if(pApp->eDevState == DS_RUN_UPS) {
            Boost_Start(&pApp->sBooster);
            GPIO_SET_HIGH_CTRL_RELAY();
            pApp->eDevSm = DSM_STARTING_BOOSTER;
        }*/
        break;
    case DSM_STARTING_BOOSTER:
        if(pApp->eDevState == DS_RUN_UPS) {
            Boost_Start(&pApp->sBooster);
            GPIO_SET_HIGH_CTRL_RELAY();
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
            // calculate gain on starting up inverter
            _iq gain = _IQdiv(pApp->sInverter.bCoeff - pApp->boostVolt.realValue, pApp->sInverter.aCoeff);
            gain = MIN(gain, pApp->sInverter.gainMax);
            if(Inv_GetGain(&pApp->sInverter) < _IQ(Inverter_Max_Mf) && Inv_GetGain(&pApp->sInverter) < gain) {
                _iq currGain = Inv_GetGain(&pApp->sInverter) + Inv_GetGainStep(&pApp->sInverter);
                Inv_SetGain(&pApp->sInverter, currGain);
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
        if(pApp->eDevState != DS_RUN_UPS) {
            App_StopBstInv(pApp);
            GPIO_SET_LOW_DISP_UPS_RUN();
            GPIO_SET_LOW_CTRL_RELAY();
            pApp->eDevSm = DSM_STOP_UPS;
            return;
        }

        // update inverter sine value factor
        if(pApp->lastBoostVolt != pApp->boostVolt.realValue) {

            _iq gain = _IQdiv(pApp->sInverter.bCoeff - pApp->boostVolt.realValue,
                              pApp->sInverter.aCoeff);
//            _iq gain = _IQ(0.8);

#if Build_Option == Build_Inverter_All || Build_Option == Build_Ups_All
            gain += _IQmpy(pApp->sInverter.currFbFact, pApp->inverterCurr.realValue);
#endif
            gain = MIN(gain, pApp->sInverter.gainMax);
            Inv_SetGain(&pApp->sInverter, gain);
            pApp->lastBoostVolt = pApp->boostVolt.realValue;

            //LREP("adc: %d\r\n", (long)_IQ18int(pApp->boostVolt.realValue));
#if 0
        int16_t value;
        value = _IQ18int(pApp->boostVolt.realValue);
        if(max <  value)
        {
            max = value;
        }
        else if( min > value)
        {
            min = value;
        }
        tmp++;
        if (tmp == 100)
        {
            LREP("\r\n[Max %d - Min %d]", (long)max, (long)min);
            tmp = 0;
            max = -32767;
            min = 32767;
        }
#endif

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


