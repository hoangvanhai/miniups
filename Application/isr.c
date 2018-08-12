/** @FILE NAME:    isr.c
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



/***************************** Include Files *********************************/
#include <BSP.h>
#include <DSP28x_Project.h>
#include <Timer.h>
#include <app.h>
#include <SineGen.h>

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

_iq sinValue = 0;
_iq lastSinValue = 0;
uint16_t duty;
/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */

__interrupt void epwm1_isr(void)
{

    EPwm1Regs.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */

__interrupt void epwm2_isr(void)
{

    EPwm2Regs.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */

__interrupt void epwm3_isr(void)
{
    static uint16_t counter = 0;

    if(counter++ >= 4) {
        counter = 0;
        if(sApp.eDevState == DS_RUN_UPS) {
            //Boost_Process(&sApp.sBooster, sApp.boostVolt.realValue);
        }
        GPIO_TOGGLE_DISP_BATT_LOW();
    }

    EPwm3Regs.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}



/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */

__interrupt void epwm4_isr(void)
{

    EPwm4Regs.ETCLR.bit.INT = 1;
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}

/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
__interrupt void epwm1_tzint_isr(void)
{
    LREP("Trip Zone 1\r\n");
    /**
     * because use EPwmxRegs.TZCLR.bit.INT; cause other interrupt do not work properly
     * so not recommend use the statement frequently
     */

    EALLOW;
    EPwm1Regs.TZCLR.bit.CBC     = 1;
    //EPwm1Regs.TZCLR.bit.INT     = 1;
    EPwm1Regs.TZCLR.bit.DCAEVT2 = 1;
    EDIS;


    if(sApp.numTripOccurs++ >= APP_NUM_TRIP_TO_LOCK) {
        sApp.bDevLocked = TRUE;
        App_StopUps(&sApp);
    }

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;


}

/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
__interrupt void epwm2_tzint_isr(void)
{

    LREP("Trip Zone 2\r\n");
    EALLOW;
    EPwm2Regs.TZFRC.bit.CBC     = 1;
    //EPwm2Regs.TZCLR.bit.INT     = 1;
    EPwm2Regs.TZCLR.bit.DCAEVT2 = 1;
    EDIS;

    //App_StopUps(&sApp);


    PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
}

/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
__interrupt void epwm3_tzint_isr(void)
{
    LREP("Trip Zone 3\r\n");
    /**
     * because use EPwmxRegs.TZCLR.bit.INT; cause other interrupt do not work properly
     * so not recommend use the statement frequently
     */

    EALLOW;
    EPwm3Regs.TZCLR.bit.CBC     = 1;
    //EPwm3Regs.TZCLR.bit.INT     = 1;
    EPwm3Regs.TZCLR.bit.DCAEVT2 = 1;
    EDIS;


    PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
}
/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
__interrupt void epwm4_tzint_isr(void)
{
    LREP("Trip Zone 4\r\n");
    /**
     * because use EPwmxRegs.TZCLR.bit.INT; cause other interrupt do not work properly
     * so not recommend use the statement frequently
     */

    EALLOW;
    EPwm4Regs.TZCLR.bit.CBC     = 1;
    EPwm4Regs.TZCLR.bit.INT     = 1;
    EPwm4Regs.TZCLR.bit.DCAEVT2 = 1;
    EDIS;


    PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;


}

/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
__interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;
    sSysTick.u32SystemTickCount++;
    Timer_Update();
    // Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

/*****************************************************************************/
/** @brief for high voltage, low current application
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
__interrupt void cpu_timer1_isr(void)
{
    CpuTimer1.InterruptCount++;

    /* Control ups behaviours */
    App_Control(&sApp);
}

/*****************************************************************************/
/** @brief
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
__interrupt void cpu_timer2_isr(void)
{
    CpuTimer2.InterruptCount++;
}

/*****************************************************************************/
/** @brief read all adc value on channel, this routine triggered by cpu timer 0
 *
 *
 *  @param
 *  @return Void.
 *  @note
 */
__interrupt void adc_isr(void)
{
    // Control all UPS behaviour
    App_ProcessInput(&sApp);

    // Clear ADCINT1 flag reinitialize for next SOC
    AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;

    // Acknowledge interrupt to PIE
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}











/*
 * END OF FILE
 */


