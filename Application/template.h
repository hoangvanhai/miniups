#ifndef LED_H_
#define LED_H_



/***************************** Include Files *********************************/

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

#define Led_Toggle(Led) {Led ^= 1;}

#define Led_On(Led) 	{Led = 0;}

#define Led_Off(Led) 	{Led = 1;}

/************************** Function Prototypes ******************************/

void Led_init();


/************************** Variable Definitions *****************************/

/*****************************************************************************/


#endif /* LED_H_ */
