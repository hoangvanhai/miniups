/*
 * BSP.h
 *
 *  Created on: Jul 9, 2018
 *      Author: MSI
 */

#ifndef APPLICATION_BSP_H_
#define APPLICATION_BSP_H_

/***************************** Include Files *********************************/
#include <HardwareProfile.h>
#include <stdint.h>
#include <typedefs.h>
#include "F2802x_PieVect.h"
/************************** Constant Definitions *****************************/

#define LSPCLK_FREQ         FTBCLK/4
#define SCI_BAUDRATE        115200
#define SCI_PRD             (LSPCLK_FREQ/(SCI_BAUDRATE*8))  //-1

/**************************** Type Definitions *******************************/

typedef struct EPWM_REGS PWM_REGS;

typedef enum {
    PWM_SUCCESS = 0,
    PWM_ERROR_MODE,
    PWM_ERROR_PERIOD,
    PWM_ERROR_DUTY,
    PWM_ERROR_PHASE,
    PWM_ERROR_HANDLE
}EPwmRet;


/***************** Macros (Inline Functions) Definitions *********************/

void BSP_Init(void);
void BSP_Start(void);
void UART_Init();
void UART_Send(int ch);
void UART_SendStr(char *msg);
void UART_SendNum(int num);
void PWM_Init();

void PWM_Inverter_Init();

void SPI_Init();
void ADC_Init();
void CAP_Init();
void I2C_Init();
void GPIO_Output_Init();
void GPIO_Input_Init();
void TIMER_Cpu0_Init(uint32_t period);
void TIMER_Cpu1_Init(uint32_t period);
void TIMER_Cpu2_Init(uint32_t period);


EPwmRet PWM_1ChCntUpCfg(struct EPWM_REGS* pwm, uint32_t freq, uint16_t dutyPercen,
                        int16_t mode, uint16_t phasePercen);

EPwmRet PWM_2ChCntUpDownBoostCfg(struct EPWM_REGS* pwm, uint32_t freq,
                                 int16_t mode, int16_t phase, int16_t channel);
EPwmRet PWM_2ChSetDutyBoost(struct EPWM_REGS* pwm, uint16_t duty);
EPwmRet PWM_2ChUpDownBoostSetDuty(struct EPWM_REGS* pwm, uint16_t duty);

EPwmRet PWM_1ChCntUpSetDuty(struct EPWM_REGS* pwm, uint16_t duty);
EPwmRet PWM_1ChCntUpSetPeriod(struct EPWM_REGS* pwm, uint16_t period);
EPwmRet PWM_1ChCntUpSetPhase(struct EPWM_REGS* pwm, uint16_t phase);
EPwmRet PWM_1ChCntUpSetEvent(struct EPWM_REGS* pwm, uint16_t event1, uint16_t event2);

EPwmRet PWM_2ChCntUpHalfCfg(struct EPWM_REGS *pwm, uint32_t freq, int16_t mode, int16_t phase);
EPwmRet PWM_2ChCntUpSetDutyHalf(struct EPWM_REGS* pwm, uint16_t channel, uint16_t duty);

EPwmRet PWM_2ChCntUpDownFullCfg(struct EPWM_REGS *pwm, uint32_t freq, int16_t mode, int16_t phase);
EPwmRet PWM_2ChCntUpSetDutyFull(struct EPWM_REGS* pwm, uint16_t channel,
                                uint16_t CMPA, uint16_t CMPB, uint16_t updateAQ);

void ADC_SocConfig(int ChSel[], int Trigsel[],
                   int ACQPS[], int IntChSel, int mode);



#define GPIO_TOGGLE_DRV_EN1()       GpioDataRegs.GPATOGGLE.bit.GPIO7 = 1
#define GPIO_SET_HIGH_DRV_EN1()     GpioDataRegs.GPASET.bit.GPIO7 = 1
#define GPIO_SET_LOW_DRV_EN1()      GpioDataRegs.GPACLEAR.bit.GPIO7 = 1

#define GPIO_TOGGLE_DRV_EN2()       GpioDataRegs.GPATOGGLE.bit.GPIO29 = 1
#define GPIO_SET_HIGH_DRV_EN2()     GpioDataRegs.GPASET.bit.GPIO29 = 1
#define GPIO_SET_LOW_DRV_EN2()      GpioDataRegs.GPACLEAR.bit.GPIO29 = 1


#define GPIO_TOGGLE_FAN_ON()        GpioDataRegs.GPATOGGLE.bit.GPIO5 = 1
#define GPIO_SET_HIGH_FAN_ON()      GpioDataRegs.GPASET.bit.GPIO5 = 1
#define GPIO_SET_LOW_FAN_ON()       GpioDataRegs.GPACLEAR.bit.GPIO5 = 1

#define GPIO_TOGGLE_RUN()           GpioDataRegs.GPBTOGGLE.bit.GPIO33 = 1
#define GPIO_SET_HIGH_RUN()         GpioDataRegs.GPBSET.bit.GPIO33 = 1
#define GPIO_SET_LOW_RUN()          GpioDataRegs.GPBCLEAR.bit.GPIO33 = 1


#define GPIO_TOGGLE_CTRL_RELAY()        GpioDataRegs.GPBTOGGLE.bit.GPIO32
#define GPIO_SET_HIGH_CTRL_RELAY()      GpioDataRegs.GPBSET.bit.GPIO32
#define GPIO_SET_LOW_CTRL_RELAY()       GpioDataRegs.GPBCLEAR.bit.GPIO32

#define GPIO_TOGGLE_DISP_ACIN()         GpioDataRegs.GPATOGGLE.bit.GPIO16
#define GPIO_SET_HIGH_DISP_ACIN()       GpioDataRegs.GPASET.bit.GPIO16
#define GPIO_SET_LOW_DISP_ACIN()        GpioDataRegs.GPACLEAR.bit.GPIO16

#define GPIO_TOGGLE_DISP_UPS_RUN()      GpioDataRegs.GPATOGGLE.bit.GPIO17
#define GPIO_SET_HIGH_DISP_UPS_RUN()    GpioDataRegs.GPASET.bit.GPIO17
#define GPIO_SET_LOW_DISP_UPS_RUN()     GpioDataRegs.GPACLEAR.bit.GPIO17

#define GPIO_TOGGLE_DISP_BATT_LOW()     GpioDataRegs.GPATOGGLE.bit.GPIO19
#define GPIO_SET_HIGH_DISP_BATT_LOW()   GpioDataRegs.GPASET.bit.GPIO19
#define GPIO_SET_LOW_DISP_BATT_LOW()    GpioDataRegs.GPACLEAR.bit.GPIO19



#define RS485_DEBUG_RECV_ENABLE
#define RS485_DEBUG_SEND_ENABLE


#endif /* APPLICATION_BSP_H_ */
