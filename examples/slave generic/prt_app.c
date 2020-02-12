#include "prt.h"
#include <string.h>

#include "lite_utils.h" //See https://github.com/llunohodl/lite_utils.git


//MCU depended macro
//Transmit data
#define UART_TRANSMIT()
//Disable TX
#define UART_DISABLE_TX
//Enable TX
#define UART_ENABLE_TX
//Disable TX IRQ
#define UART_DISABLE_TX_IRQ
//Enable TX IRQ
#define UART_ENABLE_TX_IRQ
//Check TX IRQ Flag
#define USART_IS_TX 1
//Reste RX IRQ Flag
#define USART_IRQ_RST_TX

//Recive data
#define UART_RECEIVE() 0
//Disable RX and IRQ
#define UART_DISABLE_RX
//Enable RX and IRQ
#define UART_ENABLE_RX
//Check RX IRQ Flag
#define USART_IS_RX 1
//Reste RX IRQ Flag
#define USART_IRQ_RST_RX

#define prt_name    example
#define prt_tx_len (prt_sz_t)20
#define prt_rx_len (prt_sz_t)20

//Function enum
enum {
    PRT_FUNC1,
    PRT_FUNC2,
    PRT_FUNC3,
    PRT_FUNC4,
    _PRT_CMD_QTY,
};

//For increased noise immunity
//command code must not match with slaves adreses
const uint8_t cmd_code[]={0x90,0xA2,0xB4,0xC6};
protocol_slave_def(prt_name,prt_tx_len,prt_rx_len,notyfy,sendDat,cmd_code,0x45);

// "lite_utils.h" binary fifo
fifo_bin_def(prt_name##_rx,(prt_sz_t)(2*prt_tx_len/3));
fifo_bin_def(prt_name##_tx,(prt_sz_t)(2*prt_tx_len/3));

// PDU instance
static lt_hb_PDU_t PDUin;


//Initialisation of serial bus
void prot_ini(){
    /*Place init  UART code here*/

    UART_DISABLE_TX_IRQ;
    UART_DISABLE_TX;
    UART_ENABLE_RX;
}

//main loop protocol parse
void prot_loop(){
    uint8_t byte=0;
    while(fifo_bin_get_byte(&prt_name##_rx,&byte)==0){
      ProtocolRx(&prt_name,byte);
    }
}

/* in main.c
extern void prot_ini();
extern void prot_loop();
...
int main( void ){
    prot_ini();
    __enable_irq();
	while (1){
      prot_loop();
	}
}
*/

//parse callback
void notyfy(uint8_t Addres, uint8_t cCode, uint8_t* Data, prt_sz_t Len){
  switch(cCode){
    case PRT_FUNC1:
      //Echo
      if(Addres!=PRT_BROADCAST){
        ProtocolTx(&prt_name,Addres,cCode,Data,Len);
      }
      break;
    case PRT_FUNC2:
      //Echo
      if(Addres!=PRT_BROADCAST){
        ProtocolTx(&prt_name,Addres,cCode,Data,Len);
      }
      break;
    case PRT_FUNC3:
      //Echo
      if(Addres!=PRT_BROADCAST){
        ProtocolTx(&prt_name,Addres,cCode,Data,Len);
      }
      break;
    case PRT_FUNC4:
      //Echo
      if(Addres!=PRT_BROADCAST){
        ProtocolTx(&prt_name,Addres,cCode,Data,Len);
      }
      break;
  }
}

//send data callback
void sendDat(uint8_t* Data,prt_sz_t Len){
  if(Len){
    UART_DISABLE_RX;
    UART_TRANSMIT(Data[0]);
    if(Len>1){
      Len--;
      fifo_bin_put(&lt_hb_tx,&Data[1],Len);
      UART_ENABLE_TX_IRQ;
      UART_ENABLE_TX;
    }
  }
}

//uart IRQs
void prt_usart_tx_irq(void){
  if(USART_IS_TX){
    USART_IRQ_RST_TX;
    uint8_t byte=0;
    if(fifo_bin_get_byte(&lt_hb_tx,&byte)==0){
      UART_TRANSMIT(byte);
      if (fifo_bin_count(&lt_hb_tx)==0){
        UART_DISABLE_TX_IRQ;
        UART_DISABLE_TX;
        UART_ENABLE_RX;
      }
    }else{
      UART_DISABLE_TX_IRQ;
      UART_DISABLE_TX;
      UART_ENABLE_RX;
    }
  }
}

void prt_usart_rx_irq(void){
  if(USART_IS_RX){
    USART_IRQ_RST_RX;
    fifo_bin_put_byte(&lt_hb_rx,UART_RECEIVE());
  }
}
