

/***************************** Include Files *********************************/
#include <console.h>
#include <string.h>
#include "AppConfig.h"

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/
uint32_t 	debug_port = NULL;
#ifdef CONSOLE

volatile BOOL isGetCmd = FALSE;
static const shell_command_t *shell_cmd_table;
uint32_t shell_cmdline_pos;
char shell_cmdline[SHELL_CMDLINE_SIZE];

/*****************************************************************************/

/** @brief 
 *		   
 *
 *  @param
 *  @return Void.
 *  @note
 */

void Debug_Init(uint32_t uart_port_idx)
{
//	XMC_UART_CH_CONFIG_t uart_config =
//	{
//		.baudrate = DEBUG_BAUDRATE,
//		.data_bits = 8,
//		.stop_bits = 1,
//	};
//
//	XMC_UART_CH_Init(UART[uart_port_idx], &uart_config);
//	// set pin for RXD
//	XMC_GPIO_SetMode(UART_RX_PORT[uart_port_idx], UART_RX_PIN[uart_port_idx],
//			XMC_GPIO_MODE_INPUT_TRISTATE);
//	// set pin for TXD
//	XMC_GPIO_SetMode(UART_TX_PORT[uart_port_idx], UART_TX_PIN[uart_port_idx],
//			XMC_GPIO_MODE_OUTPUT_PUSH_PULL | UART_TX_AF[uart_port_idx]);
//	// set input source pin
//	XMC_UART_CH_SetInputSource(UART[uart_port_idx],
//			XMC_UART_CH_INPUT_RXD, UART_RX_INPUT[uart_port_idx]);
//
//	XMC_USIC_CH_EnableEvent(UART[uart_port_idx], XMC_USIC_CH_EVENT_STANDARD_RECEIVE);                                            // enable interrupt RI
//	XMC_USIC_CH_SetInterruptNodePointer(UART[uart_port_idx],
//			XMC_USIC_CH_INTERRUPT_NODE_POINTER_RECEIVE,
//			UART_ISR_IDX[uart_port_idx]);             // set USIC0 SR5 for RI
//	XMC_USIC_CH_EnableEvent(UART[uart_port_idx],
//			XMC_USIC_CH_EVENT_ALTERNATIVE_RECEIVE);                                         // 'USIC_AI.007' -> AIR ON
//	XMC_USIC_CH_SetInterruptNodePointer(UART[uart_port_idx],
//			XMC_USIC_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE,
//			UART_ISR_IDX[uart_port_idx]);
//
//	XMC_USIC_CH_EnableEvent(UART[uart_port_idx], XMC_USIC_CH_EVENT_TRANSMIT_SHIFT);
//	XMC_USIC_CH_SetInterruptNodePointer(UART[uart_port_idx],
//			XMC_USIC_CH_INTERRUPT_NODE_POINTER_TRANSMIT_SHIFT, UART_ISR_IDX[uart_port_idx]);
//
//	NVIC_SetPriority(UART_IRQn[uart_port_idx],2);   // CMSIS function for NVIC control: NVIC_SetPriority(IRQn_t IRQn, uint32_t priority): priority=0..63
//	NVIC_EnableIRQ(UART_IRQn[uart_port_idx]);       // CMSIS function for NVIC control: enable IRQn
//
//	XMC_UART_CH_Start(UART[uart_port_idx]);

	debug_port = uart_port_idx;

	LREP("\r\n\r\nconsole setup done\r\n");
}

/*****************************************************************************/
/** @brief 
 *		   UARTx RX interrupt service routine
 *
 *  @param
 *  @return Void.
 *  @note
 */

void Debug_RX_ISRHandler(void)
{
	//putchar1((uint8_t)XMC_UART_CH_GetReceivedData(UART[debug_port]));
	//PushCommand((uint8_t)XMC_UART_CH_GetReceivedData(UART[debug_port]));
}

/*****************************************************************************/
/** @brief 
 *		   UARTx TX interrupt service routine
 *
 *  @param
 *  @return Void.
 *  @note
 */

void Debug_TX_ISRHandler()
{	
	
}	

void Debug_Task(void *arg) {
	(void)arg;
	if(isGetCmd) {
		shell_exec();
		shell_clear_buffer();
		isGetCmd = FALSE;
		LREP(SHELL_PROMPT);
	}
}

/*****************************************************************************/
/** @brief 
 *		   Push a command into the debug command queue
 *
 *  @param
 *  @return Void.
 *  @note
 */

BOOL PushCommand(uint8_t ch)
{
	if(ch != 0xd)
	{
		if (((char)ch != SHELL_CR) && (shell_cmdline_pos < SHELL_CMDLINE_SIZE))
		{
			switch(ch)
			{
			case SHELL_BACKSPACE:
			case SHELL_DELETE:
			if (shell_cmdline_pos > 0U)
			{
				shell_cmdline_pos -= 1U;
				putchar1(SHELL_BACKSPACE);
				putchar1(' ');
				putchar1(SHELL_BACKSPACE);
			}
			break;

			default:
			if ((shell_cmdline_pos + 1U) < SHELL_CMDLINE_SIZE)
			{
				/* Only printable characters. */
				if (((char)ch >= SHELL_SPACE) && ((char)ch <= SHELL_DELETE))
				{
					shell_cmdline[shell_cmdline_pos] = (char)ch;
					shell_cmdline_pos++;
					putchar1((char)ch);
				}
			}
			  break;
			}
		}
		else
		{
			shell_cmdline[shell_cmdline_pos] = '\0';
			putchar1(SHELL_CR);
			putchar1(SHELL_LF);
			isGetCmd = TRUE;
			return TRUE;
		}

		return FALSE;
	}

	return FALSE;
}
/*****************************************************************************/
/** @brief 
 *		   Pop a command out of the debug command queue
 *
 *  @param
 *  @return Debug command.
 *  @note
 */

void PopCommand(uint8_t*    pu8Command)
{

}





static int32_t shell_make_argv(char *cmdline, char *argv[])
{
	int32_t argc = 0;
	int32_t i;
	bool in_text_flag = false;

	if ((cmdline != NULL) && (argv != NULL)) {
		for (i = 0u; cmdline[i] != '\0'; ++i) {
			if (cmdline[i] == ' ') {
				in_text_flag = false;
				cmdline[i] = '\0';
			} else {
				if (argc < SHELL_ARGS_MAX) {
					if (in_text_flag == false) {
						in_text_flag = true;
						argv[argc] = &cmdline[i];
						argc++;
					}
				} else { /* Return argc.*/
				  break;
				}
			}
		}
		argv[argc] = 0;
	}
	return argc;
}

void shell_exec(void)
{
	char *argv[SHELL_ARGS_MAX + 1u]; /* One extra for 0 terminator.*/
	int32_t argc;
	argc = shell_make_argv(shell_cmdline, argv);
	if (argc != 0)
	{
		const shell_command_t *cur_command = shell_cmd_table;
		while (cur_command->name)
		{
			if (strcasecmp(cur_command->name, argv[0]) == 0) /* Command is found. */
			{
				if (((argc - 1u) >= cur_command->min_args) && ((argc - 1u) <= cur_command->max_args))
				{
					if (cur_command->cmd_ptr)
					{
					   ((void(*)(int32_t cmd_ptr_argc, char **cmd_ptr_argv))(cur_command->cmd_ptr))(argc, argv);
					}
				}
				else /* Wrong command syntax. */
				{
					LREP(SHELL_ERR_SYNTAX, argv[0]);
				}

				break;
			}
			cur_command++;
		}

		if (cur_command->name == 0)
		{
			LREP(SHELL_ERR_CMD, argv[0]);
		}
	}
}

void shell_help(void)
{
  const shell_command_t *cur_command = shell_cmd_table;

  while (cur_command->name)
  {
    LREP(">%7s %-32s- %s\r\n", cur_command->name,
                                      cur_command->syntax,
                                      cur_command->description);
    cur_command++;
  }
}


void shell_set_command(const char *cmd, uint32_t size) {
	if(size > SHELL_CMDLINE_SIZE) return;
	shell_cmdline_pos = size;
	memcpy((char*)shell_cmdline, (const char*)cmd, size);
	shell_cmdline[size] = '\0';
	isGetCmd = TRUE;
}

void shell_clear_buffer() {
	shell_cmdline_pos = 0u;
	shell_cmdline[0] = 0u;
}

void shell_init(const shell_command_t *const cmd_table, void (*init)(void))
{
	shell_clear_buffer();
	shell_cmd_table = cmd_table;
	init();
}

/*****************************************************************************/
#endif //BOARD != BOARD_SDR_ADAPTER
