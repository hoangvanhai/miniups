/** @FILE NAME:    console.h
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

#ifndef CONSOLE_H_
#define CONSOLE_H_

/***************************** Include Files *********************************/
#include "typedefs.h"
#include "printf.h"
#include "utils.h"
#include <HardwareProfile.h>

/************************** Constant Definitions *****************************/
#define SHELL_PROMPT "SHELL> "
#define SHELL_CMDLINE_SIZE      256
#define SHELL_ARGS_MAX          16

#define SHELL_ERR_SYNTAX   ("Error: Invalid syntax for: %s\r\n")
#define SHELL_ERR_CMD      ("Error: No such command: %s\r\n")

#define SHELL_BACKSPACE    ((char)(0x08))  /* Backspace. */
#define SHELL_DELETE       ((char)(0x7F))  /* Delete. */
#define SHELL_CTRLC        ((char)(0x03))  /* Ctrl + C. */
#define SHELL_CR           ((char)(0x0D))  /* CR. */
#define SHELL_LF           ((char)(0x0A))  /* LF. */
#define SHELL_ESC          ((char)(0x1B))  /* Esc. */
#define SHELL_SPACE        ((char)(0x20))  /* Space. */

/**************************** Type Definitions *******************************/
typedef void(*shell_cmd_function_t)(int32_t argc, char **argv);

typedef struct shell_command
{
    char *name;          	/**< @brief Command name (null-terminated string). */
    int8_t                 	min_args;       /**< @brief Minimum number of arguments the command accepts.*/
    int8_t                 	max_args;       /**< @brief Maximum number of arguments the command accepts.*/
    shell_cmd_function_t   	cmd_ptr;        /**< @brief Pointer to the actual command function defined by
                                                *   the @ref shell_cmd_function_t type    */
    char *description;   	/**< @brief Brief description of the command (null-terminated string). @n
                                                * This field is used by the @ref shell_help() function.@n*/
    char *syntax;        	/**< @brief Syntax of the command (null-terminated string). @n
                                                * This field is used by the @ref shell_help() function.
                                                * The standard command line syntax information
                                                * which will be helpful to describe the possible command
                                                * line parameters in a help display is:
                                                * - @c [] = When a parameter is surrounded with square
                                                * brackets, this means the parameter is optional.
                                                * - @c <> = When a parameter is surrounded with angle
                                                * brackets, this means the parameter is required for
                                                * normal operations of command.
                                                * - @c | = The vertical bar means a choice between
                                                * parameter value is acceptable.*/
} shell_command_t;



/***************** Macros (Inline Functions) Definitions *********************/
#ifdef DEBUG
	
	#define ASSERT_NONVOID(con,ret)	{if(!(con))	{LREP("\r\nASSERT in file " __FILE__ " at line "  TOSTRING(__LINE__) "\r\n"); return ret;}	}
	#define ASSERT_VOID(con)        {if(!(con))	{LREP("\r\nASSERT in file " __FILE__ " at line "  TOSTRING(__LINE__) "\r\n"); return;	 }	}
	#define ASSERT(con)				{if(!(con))	{LREP("\r\nASSERT in file " __FILE__ " at line "  TOSTRING(__LINE__) "\r\n");			 }	}
		
	#define LREP	 				uart_printf
	
	#define RS485_DEBUG_TX_EN()		{_LATB8 = 1;}
	#define RS485_DEBUG_RX_EN()		{_LATB8 = 0;}
	
#else
	#define ASSERT_NONVOID(con,ret)
	#define ASSERT_VOID(con)
	#define ASSERT(con)	
	#define LREP(...)
#endif
	
#define		PRINT_STRING_MAX_SIZE	128
#define		DEBUG_CMD_QUEUE_SIZE	8

#define LREP_BUF 	buf_sprintf

/************************** Function Prototypes ******************************/


void 	Debug_Init(uint32_t uart_port_idx);
void 	Debug_Task(void *arg);
BOOL 	PushCommand(uint8_t ch);
void 	PopCommand(uint8_t*    pu8Command);
void 	Debug_TX_ISRHandler();
void 	Debug_RX_ISRHandler();

void 	shell_init(const shell_command_t *const cmd_table, void (*init)(void));
void 	shell_clear_buffer();
void 	shell_exec(void);
void 	shell_help(void);
void 	shell_set_command(const char *cmd, uint32_t size) ;


/************************** Variable Definitions *****************************/
extern uint32_t 	debug_port;


/*****************************************************************************/




#endif /* DEBUG_H_ */

