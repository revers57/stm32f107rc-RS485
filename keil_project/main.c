#include "main.h"
int main(void){
	WATCHDOG_init();
	Comm_UART_Init();
	while(1){
		if(Is_Comm_rx_Available()){
			char data = Comm_ReadFrom_rx_Buff();
			Comm_StoreIn_tx_Buff(data);
		}
	}
}

