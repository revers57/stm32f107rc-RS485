#include "COMM_UART.h"

comm_uart comm_rx;
comm_uart comm_tx;
/*************************************
*	USART 1 For Communication and Debug 
* PA9  -> TX
* PA10 -> RX
* Baud Rate -> 230400
**************************************/
void Comm_UART_Init(void){
	//----USART 1 Initialization----//
	//Enable clock to Port A
	RCC->APB2ENR |= ((0x01)<< 2) | ((0x01)<<14);  //Enable GPIO Port A clock and usart1
	//Set PA9 as output (bcoz uart tx)
	GPIOA->CRH |= (0x30) | ((0x01)<<7); //configure as output mode(pushpull) and output mode 2MHz speed
	//Set PC2
	GPIOC->CRL |= (0x30) | ((0x01)<<2) 
	//Set PA10 as input(Because uart rx)
	//PA10 already by default set to input mode and floating input so no need to cofigure it
	//USART DIV value
  //USART1->BRR = 0x138; //BAUD RATE FOR 8MHZ APB2 BUS CLOCK Baud rate set to 230400
	USART1->BRR = 0x271;	//Baud rate set to 115200
	//USART1->BRR = 0x1D4C; //Baud rate set to 9600
	USART1->CR1 |= ((0x01)<<2) | ((0x01)<<3) | ((0x01)<<13) | ((0x01<<5)) | ((0x01)<<7); //TX RX AND USART ENABLE
	NVIC_EnableIRQ(USART1_IRQn);
}

/*******************************************
* Comm_UART_Flush function is used for 
* flushing the uart rx buffer
********************************************/
void Comm_UART_Flush(void){
	memset(comm_rx.buffer,'\0',COMM_BUFFER_SIZE);
	comm_rx.head = 0;
	comm_rx.tail = 0;
}

/*******************************************
* Comm_StoreInBuff function is used for 
* storing rx buffer from UART DATA Register
********************************************/
void Comm_StoreIn_rx_Buff(char data){
	int i = (comm_rx.head + 1) % COMM_BUFFER_SIZE;
	comm_rx.buffer[comm_rx.head] = data;
	comm_rx.head = i;
}

/*******************************************
* Comm_StoreIn_tx_Buff function is used for 
* storing tx ring buffer to send it to UART 
* DATA Register
********************************************/
void Comm_StoreIn_tx_Buff(char data){
	int i = (comm_tx.head + 1) % COMM_BUFFER_SIZE;
	comm_tx.buffer[comm_tx.head] = data;
	comm_tx.head = i;
	USART1->CR1 |= (USART_CR1_TXEIE); // Enable TX Data Register Empty Interrupt
}

/*******************************************
* Comm_ReadFrom_rx_Buff function is used for 
* reading rx ring buffer to send it to UART 
* DATA Register
********************************************/
char Comm_ReadFrom_rx_Buff(void){
	int i = (comm_rx.tail + 1) % COMM_BUFFER_SIZE;
	char data = comm_rx.buffer[comm_rx.tail];
	comm_rx.tail = i;
	return data;
}

/*******************************************
* Comm_SendStringIn_tx_Buff function is used 
* for storing string in tx buffer to send it
* to UART DATA Register
********************************************/
void Comm_SendStringIn_tx_Buff(char *data){
	while(* data){
		Comm_StoreIn_tx_Buff(*data++);
	}
}

/*******************************************
* Is_Comm_rx_Available function is used for 
* checking if anything is left to read.
********************************************/
uint16_t Is_Comm_rx_Available(void){
	return (uint16_t) ((COMM_BUFFER_SIZE -  (comm_rx.head - comm_rx.tail)) % COMM_BUFFER_SIZE);
}

/*******************************************
* USART1_IRQHandler function is an interrupt
* handler for usart1.
********************************************/
void USART1_IRQHandler(void){
	if(USART1->SR & USART_SR_RXNE){
		char c = USART1->DR;
		Comm_StoreIn_rx_Buff(c);
	}
	else if(USART1->SR & (USART_SR_TXE)){
		if(comm_tx.head == comm_tx.tail){
			USART1->CR1 &= ~(USART_CR1_TXEIE); //Disable TX Data Register Empty Interrupt
		}
		else{
			/*char j[] = "ERROR!";*/
			volatile int i = (comm_tx.tail +1) % COMM_BUFFER_SIZE;
			for (int m = 0; m < 141; m = m + 14) {
			if ((comm_tx.tail > 2)&&(comm_tx.buffer[3 + m] == '0')&&(comm_tx.buffer[4 + m] == '0')&&(comm_tx.buffer[5 + m] == '0')&&(comm_tx.buffer[6+m] == '0')&&(comm_tx.buffer[7+m] == '0')&&(comm_tx.buffer[8+m] == '0')&&(comm_tx.buffer[9+m] == '0')&&(comm_tx.buffer[10+m] == '0')) {
						/*for (int h = 0; h < 6; h++) {
						Comm_StoreIn_tx_Buff(j[h]);
					}*/
						GPIOC->CRL |= (1 << 2);
						if (m == 140) {
								m = 0;
					}
						WATCHDOG_kick();
			} }
			USART1->DR = comm_tx.buffer[comm_tx.tail];
			comm_tx.tail = i;
		}
	}
}

/*******************************************
* WATCHDOG_init function is a function for 
* initializing IWDG.
********************************************/
void WATCHDOG_init(void){
	IWDG->KR =  0x5555; //Writing the key value 5555h to enable access to the IWDG_PR and IWDG_RLR registers
	IWDG->PR =0x4;		//the IWDG Prescaler value
	IWDG->RLR =0x0271; 	//the IWDG Reload value
	IWDG->KR = 0xCCCC;//Writing the key value CCCCh starts the watchdog
}

/*******************************************
* WATCHDOG_kick function is a function for 
* initializing IWDG.
********************************************/
void WATCHDOG_kick(void){
	IWDG->KR |= 0xAAAA;//These bits must be written with the key value AAAAh,otherwise the watchdog generates a reset when the counter reaches 0
}
