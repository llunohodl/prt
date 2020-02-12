#include <stdint.h>
#include "prt.h"

/*
  Protocol data uinit structure:
  Byte 0: Addres 0..254 (255 - Broadcast)
  Byte 1: Command code (see CmdList in protocol_t)
  Byte 2: Data length (L) 0..255
  Byte 3: Command control summ = ((Byte 0 + Byte 1 + Byte 2) ^ prt_summ_xor ) & 0xFF
  if L > 0 (L = Data length):
    Byte 4 .. Byte 4+L-1: Data
    Byte 4+L: Data control summ = ((Byte 4 +... + Byte 4+L-1) ^ ~prt_summ_xor ) & 0xFF
*/

#ifndef prt_summ_xor
#define prt_summ_xor 0xAA
#endif

void ProtocolIni(protocol_t *pr){
    pr->RxIndex=0;
	pr->Stage=0;
    pr->Valid=0;
}

//TX parse function
void ProtocolTx(protocol_t *pr,uint8_t Addres, uint8_t Code, uint8_t* Data, uint8_t Len){
  if(Len>pr->TxLen-3){
      Len=pr->TxLen-3;
  }
  //Add head
  pr->Tx[0]=Addres;
  pr->Tx[1]=pr->CmdList[Code];
  pr->Tx[2]=Len;
  pr->Tx[3]=pr->Tx[0]+pr->Tx[1]+pr->Tx[2];
  pr->Tx[3]^=prt_summ_xor;
  uint8_t Index=4;
  if(Len){
    uint8_t Valid=0;
    do{
      //Add data
      pr->Tx[Index++]=*Data;
      Valid+=*Data;
      Data++;
      Len--;
    }while(Len);
    pr->Tx[Index++]=Valid^(~prt_summ_xor);
  }
  pr->SendData(pr->Tx,Index);
}

//RX parse function
void ProtocolRx(protocol_t *pr,uint8_t data){
    if(pr->Stage==0){
      //Parse Head (FIFO for Head)
      pr->Rx[0]=pr->Rx[1];
      pr->Rx[1]=pr->Rx[2];
      pr->Rx[2]=pr->Rx[3];
      pr->Rx[3]=data;
      //Check addres
      if((!pr->Master)&&(pr->Rx[0]!=pr->Addres)&&(pr->Rx[0]!=PRT_BROADCAST)){
         return;
      }
      //Check command code
      pr->cCodeNum=pr->CmdLen;
      for(uint8_t i=0;i<pr->CmdLen;i++){
        if(pr->CmdList[i]==pr->Rx[1]){
          pr->cCodeNum=i;
          break; //from for
        }
      }
      if(pr->cCodeNum==pr->CmdLen){
        return;
      }
      //Check data length
      if(pr->Rx[2]>pr->RxLen){
        return;
      }
      //Check header control summ
      pr->Valid=pr->Rx[0]+pr->Rx[1]+pr->Rx[2];
      pr->Valid^=prt_summ_xor;
      if(pr->Rx[3]!=pr->Valid){
        return;
      }
      if(pr->Rx[2]==0){ //No Data
        pr->Notyfy(pr->Rx[0],pr->cCodeNum,NULL,pr->Rx[2]);
        return;
      }
      pr->RxIndex=4;
      pr->Stage=1;
      pr->Valid=0;
      return;
    }else{

      if(pr->RxIndex<pr->Rx[2]+4){
        //Get data
        pr->Rx[pr->RxIndex++]=data;
        pr->Valid+=data;
        return;
      } else {
        //Check data control summ
        pr->Valid^=~prt_summ_xor;
        if(data==pr->Valid){
          pr->Notyfy(pr->Rx[0],pr->cCodeNum,&pr->Rx[4],pr->Rx[2]);
        }
      }
      pr->Stage=0;
      return;
    }
}
