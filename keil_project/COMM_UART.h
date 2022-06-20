#ifndef __COMM_UART_H
#define __COMM_UART_H
	#include "stm32f10x.h"
	#include "string.h"
	#include <stdarg.h>
	#define COMM_BUFFER_SIZE 256
	//variables declaration
	typedef struct{
		uint8_t buffer[COMM_BUFFER_SIZE];
		volatile uint8_t head;
		volatile uint8_t tail;
	}comm_uart;
	
	//function definations
	void Comm_UART_Init(void);
	void Comm_UART_Flush(void);
	uint16_t Is_Comm_rx_Available(void);
	void Comm_SendStringIn_tx_Buff(char *data);
	void Comm_StoreIn_tx_Buff(char data);
	char Comm_ReadFrom_rx_Buff(void);
	void WATCHDOG_init(void);
	void WATCHDOG_kick(void);
#endif

