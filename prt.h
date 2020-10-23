#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_
#include <stdint.h>
#include "prt_conf.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef prt_sz_t
#define prt_sz_t uint8_t
#endif

#define PRT_BROADCAST       255

typedef struct{
	uint8_t Master;         //Master =1 / Slave = 0
	uint8_t Addres;         //Slave addres (only for slave)
	const prt_sz_t TxLen;   //Tx buffer length
	uint8_t* Tx;            //Tx buffer 
	const prt_sz_t RxLen;   //Rx buffer length
	uint8_t* Rx;            //Tx buffer
    //Parser callback (called from ProtocolRx(...) when valid command found)
    //Addres - slave address 
    //cCode - command number
    //Data - pointer on data field content
    //Len - data field content length
	void (*Notyfy)(uint8_t Addres, uint8_t cCode, uint8_t* Data, prt_sz_t Len);
	//Tx callback (called from ProtocolTx(...) for send data)
    //Data - pointer on data for tx
    //Len - data for tx length
	void (*SendData)(uint8_t* Data, prt_sz_t Len);
	const uint8_t CmdLen;   //Command code quantity
	const uint8_t* CmdList; //Allowed command codes
    
	//private data for parse
	uint8_t RxIndex;
	uint8_t Stage;
	uint8_t Valid;
    uint8_t cCodeNum;
}protocol_t;


//Protocol initial define generic 
#define protocol_def(name,rxlen,txlen,notyfy_fn,send_fn,cmd_code,master,addres)\
typedef struct{\
  uint8_t Addres;\
  uint8_t Code;\
  uint8_t Data[rxlen>txlen ? rxlen : txlen];\
  uint8_t Len;\
}name##_PDU_t;\
uint8_t name##_rxbuf[rxlen+5];\
uint8_t name##_txbuf[txlen+5];\
extern void notyfy_fn(uint8_t Addres, uint8_t cCode, uint8_t* Data, prt_sz_t Len);\
extern void send_fn(uint8_t* Data, prt_sz_t Len);\
protocol_t name={\
	.Master=master,     .Addres=addres,\
    .TxLen=txlen+5,     .Tx=name##_txbuf,\
	.RxLen=rxlen+5,     .Rx=name##_rxbuf,\
	.Notyfy=notyfy_fn,  .SendData=send_fn,\
	.CmdLen=sizeof(cmd_code),    .CmdList=cmd_code,\
};

//Protocol initial define for master
#define protocol_master_def(name,rxlen,txlen,notyfy_fn,send_fn,cmd_code)\
protocol_def(name,rxlen,txlen,notyfy_fn,send_fn,cmd_code,1,0)

//Protocol initial define for slave
#define protocol_slave_def(name,rxlen,txlen,notyfy_fn,send_fn,cmd_code,addres)\
protocol_def(name,rxlen,txlen,notyfy_fn,send_fn,cmd_code,0,addres)

//Protocol init function - alternative for protocol_(master|slave)_def
void ProtocolIni(protocol_t * pr);

//Protocol Rx data (parser)
void ProtocolRx(protocol_t * pr,uint8_t data);

//Protocol Tx data (sender)
void ProtocolTx(protocol_t * pr,uint8_t Addres, uint8_t Code, uint8_t* Data, uint8_t Len);

#endif //_PROTOCOL_H_