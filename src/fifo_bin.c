#include "fifo_bin.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>

//put data in fifo
int8_t fifo_bin_put_byte(fifo_bin_t* fifo, uint8_t byte){
  fifo->Buffer[fifo->NewI++]=byte;
  if(fifo->NewI >= fifo->Total_SZ){
     fifo->NewI=0;
  }
  fifo->Count=fifo->Count+1;
  if(fifo->Count > fifo->Total_SZ){
     fifo->Count=fifo->Total_SZ;
  }
  return 0;
}

//get data from fifo
int8_t fifo_bin_get_byte(fifo_bin_t* fifo, uint8_t* byte){
  if(fifo->Count==0){
    return -1;
  }
  *byte=fifo->Buffer[fifo->OldI++];
  if(fifo->OldI >= fifo->Total_SZ){
     fifo->OldI=0;
  }
  fifo->Count=fifo->Count-1;
  return 0;
}

//put data in fifo
int8_t fifo_bin_put(fifo_bin_t* fifo, uint8_t* byte,fifo_bin_index_t len){
  int8_t ret=0;
  if(len>fifo->Total_SZ){
    len=fifo->Total_SZ;
    ret=-1;
  }
  int8_t ret2=0;
  for(fifo_bin_index_t i=0; i<len; i++){
    ret2=fifo_bin_put_byte(fifo,byte[i]);
  }
  return ret+ret2;
}

//get data from fifo
int8_t fifo_bin_get(fifo_bin_t* fifo, uint8_t* byte,fifo_bin_index_t len){
  if(fifo->Count<len){
    return -1;
  }
  for(fifo_bin_index_t i=0; i<len; i++){
    if(fifo_bin_get_byte(fifo,&byte[i])!=0){
      return -2;
    }
  }
  return 0;
}

//delite all data in fifo
void fifo_bin_flash(fifo_bin_t* fifo){
  fifo->NewI=0;
  fifo->OldI=0;
  fifo->Count=0;
}

//get data count in fifo
fifo_bin_index_t fifo_bin_count(fifo_bin_t* fifo){
  return fifo->Count;
}


