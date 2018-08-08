/*
 * isr.c
 *
 *  Created on: Jul 16, 2018
 *      Author: PC
 */




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
    EALLOW;
    //EPwm1Regs.TZFRC.bit.CBC     = 1;
    EPwm1Regs.TZCLR.bit.INT     = 1;
    EPwm1Regs.TZCLR.bit.DCAEVT2 = 1;
    EDIS;

    sApp.numTripOccurs++;
    if(sApp.numTripOccurs < APP_NUM_TRIP_TO_LOCK) {
//        sApp.eDevSm = DSM_STARTING_INVERTER;
//        Inv_Start(&sApp.sInverter);
//        sApp.sInverter.pwm1Handle->CMPA.half.CMPA = 0;
//        sApp.sInverter.pwm2Handle->CMPA.half.CMPA = 0;
        SINE1PHASE_RESET(&sApp.sInverter.sSine1Phase);
    } else {
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
    //EPwm2Regs.TZFRC.bit.CBC     = 1;
    EPwm2Regs.TZCLR.bit.INT     = 1;
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
    //LREP("T2");
    if(sApp.eDevState == DS_RUN_UPS) {
        //Boost_Process(&sApp.sBooster, sApp.boostVolt.realValue);
    }
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


